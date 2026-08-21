//! Serial transport: 1,000,000 baud, 0x02-prefixed one-shot commands,
//! next-standard-frame response matching.

use std::fmt;
use std::io::Read;
use std::io::Write;
use std::collections::VecDeque;

/// FreeWili USB vendor id (Intrepid Control Systems); PIDs vary per variant.
pub const FREEWILI_VID: u16 = 0x093C;

/// FTDI USB vendor id - the FreeWili 2 binary event port enumerates as an
/// FTDI serial port.
pub const FTDI_VID: u16 = 0x0403;

#[derive(Debug)]
pub enum OwError {
    NoDevice,
    MultipleDevices(Vec<String>),
    Io(String),
    Timeout(String),
    Failed(String),
    Protocol(String),
}

impl fmt::Display for OwError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            OwError::NoDevice => write!(f, "no FreeWili device found"),
            OwError::MultipleDevices(names) => {
                write!(f, "multiple FreeWili devices found: {names:?}; use connect_port()")
            }
            OwError::Io(m) => write!(f, "io error: {m}"),
            OwError::Timeout(m) => write!(f, "timeout {m}"),
            OwError::Failed(m) => write!(f, "command failed {m}"),
            OwError::Protocol(m) => write!(f, "protocol error: {m}"),
        }
    }
}

impl std::error::Error for OwError {}

/// Find the FreeWili serial port by USB VID. When several match, a port whose
/// product string contains "main" (case-insensitive) is preferred.
pub fn find_port() -> Result<String, OwError> {
    let ports = serialport::available_ports().map_err(|e| OwError::Io(e.to_string()))?;
    let mut matches: Vec<(String, String)> = Vec::new();
    for p in ports {
        if let serialport::SerialPortType::UsbPort(info) = &p.port_type {
            if info.vid == FREEWILI_VID {
                let product = info.product.clone().unwrap_or_default();
                matches.push((p.port_name.clone(), product));
            }
        }
    }
    match matches.len() {
        0 => Err(OwError::NoDevice),
        1 => Ok(matches.remove(0).0),
        _ => {
            if let Some(m) = matches
                .iter()
                .find(|(_, product)| product.to_lowercase().contains("main"))
            {
                return Ok(m.0.clone());
            }
            Err(OwError::MultipleDevices(
                matches.into_iter().map(|(name, _)| name).collect(),
            ))
        }
    }
}

/// Find the FreeWili binary (FTDI/WILI) port. Matched by FTDI VID only -
/// NEVER by the "FW2" product substring: the main text port's product
/// string is "FW2 v01" and would false-match.
pub fn find_binary_port() -> Result<String, OwError> {
    let ports = serialport::available_ports().map_err(|e| OwError::Io(e.to_string()))?;
    let mut matches: Vec<String> = Vec::new();
    for p in ports {
        if let serialport::SerialPortType::UsbPort(info) = &p.port_type {
            if info.vid == FTDI_VID {
                matches.push(p.port_name.clone());
            }
        }
    }
    match matches.len() {
        0 => Err(OwError::NoDevice),
        1 => Ok(matches.remove(0)),
        _ => Err(OwError::MultipleDevices(matches)),
    }
}

/// Guards on a frame held open while its closing token is awaited: an unclosed
/// '[' must neither grow without bound nor swallow the frames behind it.
pub const MAX_PENDING_FRAME_CHARS: usize = 4096;
pub const PENDING_FRAME_TIMEOUT: std::time::Duration = std::time::Duration::from_secs(1);

/// What routing one console line produced.
enum Routed {
    /// Nothing for the caller: chatter, an event (queued), or a frame still
    /// being accumulated.
    Nothing,
    /// A complete response frame.
    Frame(crate::framing::ResponseFrame),
    /// A line that closed a frame but does not parse. `call` reports it as a
    /// protocol error; the flush and poll paths ignore it, as they always did.
    Malformed(String),
}

pub struct Transport {
    port: Box<dyn serialport::SerialPort>,
    acc: Vec<u8>,
    /// Spontaneous text-event lines "[*<id> ...]" captured during calls/polls.
    pub(crate) events: VecDeque<String>,
    /// Lines of a frame whose closing token has not arrived yet, and whether it
    /// is an event rather than a response. At most one is held: a continuation
    /// line carries no identity, so with two open there would be no way to
    /// decide which one it belongs to.
    pending: Vec<String>,
    pending_is_event: bool,
    pending_chars: usize,
    pending_deadline: Option<std::time::Instant>,
}

impl Transport {
    pub fn open(name: &str) -> Result<Self, OwError> {
        let mut port = serialport::new(name, 1_000_000)
            .timeout(std::time::Duration::from_millis(100))
            .open()
            .map_err(|e| OwError::Io(e.to_string()))?;
        // USB-CDC devices ignore writes until DTR is asserted (the serialport
        // crate does not raise it by default on Windows -> os error 121).
        port.write_data_terminal_ready(true)
            .map_err(|e| OwError::Io(format!("DTR: {e}")))?;
        let mut t = Transport {
            port,
            acc: Vec::new(),
            events: VecDeque::new(),
            pending: Vec::new(),
            pending_is_event: false,
            pending_chars: 0,
            pending_deadline: None,
        };
        // Reset navigation to the root menu and enter quiet mode.
        t.port
            .write_all(&[0x02, b'\n'])
            .map_err(|e| OwError::Io(e.to_string()))?;
        Ok(t)
    }

    /// Route one console line: queues events, reassembles a frame the firmware
    /// split across physical lines by printing a newline into its payload, and
    /// hands back a response frame once one is complete. Single point of truth
    /// for every read path below.
    fn route(&mut self, line: &str) -> Routed {
        self.expire_pending();
        let is_event = crate::framing::is_event(line);
        let is_frame = crate::framing::is_frame(line);
        if !self.pending.is_empty() && !is_event && !is_frame {
            return self.continue_pending(line);
        }
        if is_event && crate::framing::is_frame_closed(line) {
            // A complete event may arrive between the lines of a response, so
            // this deliberately leaves `pending` alone.
            self.events.push_back(line.to_string());
            return Routed::Nothing;
        }
        if is_frame && crate::framing::is_frame_closed(line) {
            self.pending.clear(); // a held frame can no longer close
            return match crate::framing::parse(line) {
                Some(f) => Routed::Frame(f),
                None => Routed::Malformed(line.to_string()),
            };
        }
        if is_event || is_frame {
            self.pending.clear();
            self.pending.push(line.to_string());
            self.pending_is_event = is_event;
            self.pending_chars = line.len();
            self.pending_deadline = Some(std::time::Instant::now() + PENDING_FRAME_TIMEOUT);
        }
        Routed::Nothing
    }

    fn continue_pending(&mut self, line: &str) -> Routed {
        self.pending.push(line.to_string());
        self.pending_chars += line.len() + 1;
        if crate::framing::is_frame_closed(line) {
            let joined = crate::framing::join_frame_lines(&self.pending);
            let was_event = self.pending_is_event;
            self.pending.clear();
            if was_event {
                self.events.push_back(joined);
                return Routed::Nothing;
            }
            return match crate::framing::parse(&joined) {
                Some(f) => Routed::Frame(f),
                None => Routed::Malformed(joined),
            };
        }
        if self.pending_chars > MAX_PENDING_FRAME_CHARS {
            self.pending.clear();
        }
        Routed::Nothing
    }

    fn expire_pending(&mut self) {
        if self.pending.is_empty() {
            return;
        }
        if self.pending_deadline.is_some_and(|d| std::time::Instant::now() >= d) {
            self.pending.clear();
        }
    }

    /// Route every complete line buffered in `acc`. With `stop_at_frame`, stops
    /// as soon as one yields a response frame, leaving the rest buffered.
    fn drain_lines(&mut self, stop_at_frame: bool) -> Option<Routed> {
        let mut out = None;
        while let Some(nl) = self.acc.iter().position(|&b| b == b'\n') {
            let raw: Vec<u8> = self.acc.drain(..=nl).collect();
            let line = String::from_utf8_lossy(&raw);
            let line = line.trim_end_matches(['\r', '\n']);
            match self.route(line) {
                Routed::Nothing => {}
                r => {
                    if out.is_none() {
                        out = Some(r);
                    }
                    if stop_at_frame {
                        break;
                    }
                }
            }
        }
        out
    }

    /// Send one command (0x02 reset prefix + one-shot path) and wait for the
    /// next standard response frame (5 s deadline).
    pub fn call(&mut self, cmd: &str) -> Result<String, OwError> {
        let mut out = Vec::with_capacity(cmd.len() + 2);
        out.push(0x02u8);
        out.extend_from_slice(cmd.as_bytes());
        out.push(b'\n');
        // Pre-call flush (mirrors the C client): queue any complete event
        // lines still buffered from a previous call/poll, then drop stale
        // frames and partial bytes so they can't be matched as THIS
        // command's response.
        let _ = self.drain_lines(false);
        self.acc.clear();
        self.pending.clear(); // a half frame from an earlier call is not ours
        self.port
            .write_all(&out)
            .map_err(|e| OwError::Io(e.to_string()))?;
        let deadline = std::time::Instant::now() + std::time::Duration::from_secs(5);
        let mut buf = [0u8; 4096];
        loop {
            if std::time::Instant::now() > deadline {
                return Err(OwError::Timeout(format!("waiting for response to {cmd:?}")));
            }
            let n = match self.port.read(&mut buf) {
                Ok(n) => n,
                Err(ref e) if e.kind() == std::io::ErrorKind::TimedOut => 0,
                Err(e) => return Err(OwError::Io(e.to_string())),
            };
            self.acc.extend_from_slice(&buf[..n]);
            match self.drain_lines(true) {
                Some(Routed::Frame(f)) => {
                    return if f.success {
                        Ok(f.response)
                    } else {
                        Err(OwError::Failed(format!("{cmd:?}: {}", f.response)))
                    }
                }
                Some(Routed::Malformed(l)) => return Err(OwError::Protocol(l)),
                _ => {}
            }
        }
    }

    /// Next spontaneous text-event line, if any. Never blocks: drains the
    /// queue first, then does one read of whatever bytes are available.
    pub fn poll_text_event(&mut self) -> Result<Option<String>, OwError> {
        if let Some(l) = self.events.pop_front() {
            return Ok(Some(l));
        }
        let avail = self.port.bytes_to_read().map_err(|e| OwError::Io(e.to_string()))? as usize;
        if avail > 0 {
            let mut buf = vec![0u8; avail.min(4096)];
            let n = match self.port.read(&mut buf) {
                Ok(n) => n,
                Err(ref e) if e.kind() == std::io::ErrorKind::TimedOut => 0,
                Err(e) => return Err(OwError::Io(e.to_string())),
            };
            self.acc.extend_from_slice(&buf[..n]);
            // Frames outside a call have no waiter: routed, then dropped.
            let _ = self.drain_lines(false);
        }
        Ok(self.events.pop_front())
    }

    /// Byte-exact access to the serial port for callers (file transfers)
    /// that must read/write a payload directly, bypassing `call()`'s
    /// line-splitting + UTF-8 decoding -- necessary because a payload can
    /// contain arbitrary bytes, including embedded `\n`, and its length is
    /// known up front rather than delimited by a line ending.
    ///
    /// Compare this to the Python binding: its `Transport` runs a
    /// *background thread* that continuously decodes and newline-splits
    /// incoming bytes, so pulling the port out from under it for a raw
    /// exchange needs an explicit `pause_reader()`/`resume_reader()`
    /// handshake -- there is no language-level way to prove the thread has
    /// backed off. Rust's `Transport` has neither problem: it never touches
    /// the port except synchronously inside a method the caller invoked
    /// (`call()`, `poll_text_event()`), and `&mut self` here means the
    /// borrow checker guarantees exclusive access to `Transport` -- and
    /// therefore the port -- for the entire lifetime of the returned
    /// reference. Nothing else can call `call()`/`poll_text_event()`
    /// meanwhile, and there is no thread to park. The scoping a
    /// closure-based `raw_exchange` helper would buy you, the borrow
    /// checker already gives you for free.
    pub fn raw_port(&mut self) -> &mut dyn serialport::SerialPort {
        self.port.as_mut()
    }

    /// Bytes already pulled off the wire by a PREVIOUS `call()` (or
    /// `poll_text_event()`) but not yet consumed as a complete line
    /// (normally empty). `put`/`get`/`list` never route their OWN handshake
    /// through `call()` -- they talk to the raw port directly via
    /// `raw_port()` -- so this can only ever hold leftovers from an EARLIER,
    /// unrelated call (say, an `sd_host_select` a moment before), never
    /// anything belonging to the transfer that's about to start. Prepending
    /// those stale bytes onto a fresh raw exchange would splice garbage from
    /// a different command into this one's stream, so callers must drain
    /// and DISCARD whatever this returns before doing their own reads --
    /// mirroring the Python binding's `pause_reader()`, which drops
    /// whatever's sitting in its reader thread's buffer for the identical
    /// reason (see transport.py's docstring); the C runtime has no
    /// equivalent because every call gets a freshly zeroed `ow_files_rx`.
    pub fn take_buffered(&mut self) -> Vec<u8> {
        std::mem::take(&mut self.acc)
    }
}
