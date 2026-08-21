//! File transfer and directory listing over the FreeWili filesystem menu.
//!
//! This is a port of core/runtime_c/ow_files.h, not a binding -- this crate
//! has no C dependency -- so keep the two diffable rather than clever.
//!
//! Ground truth is firmware MenuX/fwMenuFileSystem.cpp, by way of
//! `fwMenuX::callSubFunction` (rmpLib/fwMenuX.cpp), which opens a normal menu
//! RESPONSE FRAME before calling the handler and closes it right after
//! (neither handler defers `m_bFinishRepsonse`). So the handshake text below
//! is each the BODY of a framed response, not a bare line on its own -- the
//! real put handshake line reads `[h\x\f <hexTimestampNs> <seq> Send File Now
//! 1]`, not just `Send File Now`. The handshake frame's tag is the full
//! navigation prefix to that menu (`h\x\f` for put, `h\x\u` for get) -- a
//! DIFFERENT tag from the later completion frame (`x\f`/`x\u` below), which
//! comes from a direct `printMenuResponse` call with a fixed literal, not
//! through `callSubFunction`. So handshake matching below is tag-tolerant
//! (`framing::parse`, body content only, via the `body` local); completion
//! matching (`tagged()`) pins to the exact literal tag, since that one really
//! is fixed by the firmware source. Both handshake lines are emitted with
//! `printOutAlways`, so quiet mode never suppresses them.
//!
//! PUT (getFileFromPC / doDownload):
//!     host   -> "h\nx\nf\n<path> <size> <crc>\n"
//!     device -> framed body "Send File Now" (proceed) or "Invalid" (do NOT
//!               stream a byte)
//!     host   -> payload bytes
//!     device -> framed "x\f" trailer: "success <N> bytes" (ok) or
//!               "Failed checksum" (ok=0 -- the device deletes the file)
//!
//! GET (sendFileToPC / doUpload):
//!     host   -> "h\nx\nu\n<path> \n"   ('/' -> '\\', trailing space kept)
//!     device -> framed body "RxFile <size>" (size only, no crc yet) or
//!               "Invalid" / "CantOpenFile"
//!     device -> payload bytes
//!     device -> framed "x\u" trailer: "success <N> bytes <CRC> crc" -- the
//!               crc arrives HERE, after the payload, not in the handshake
//!
//! LIST: send "h\nx\nl\n<path>\n", then collect "[*fdir ...]" event lines
//! until an "end" record or silence.
//!
//! ## Why this module needs no pause/resume dance
//!
//! Unlike the Python port, Rust's `Transport` runs no background reader
//! thread -- it only ever touches the serial port synchronously, inside a
//! method the caller explicitly invoked (`call()`, `poll_text_event()`).
//! `Transport::raw_port()` hands this module `&mut dyn
//! serialport::SerialPort`; the borrow checker guarantees exclusive access
//! to the port for as long as that reference lives, which is *stronger*
//! than the Python binding's opt-in `pause_reader()`/`resume_reader()`
//! runtime handshake, not weaker -- there is nothing else in this process
//! that could touch the port while a transfer holds the borrow. The Rust
//! port is genuinely simpler than the Python one here, by construction:
//! there is no concurrency hazard being glossed over, because there is no
//! concurrency.
//!
//! `Transport::take_buffered()` is still required, though: a `read()`
//! inside `call()` can pull the handshake line AND the leading bytes of
//! the payload that immediately follows it off the wire in the same
//! chunk, and `call()` only consumes up to the first `\n`. `put`/`get`/
//! `list` all drain it first, before touching the port themselves, so
//! those leading bytes are never dropped.
//!
//! `sd_host_select` deliberately does not reference `crate::menus::*`
//! directly: this file is a static blob shared by every generated crate,
//! compiled unconditionally, and not every firmware tree has a hardware /
//! file-system menu (a synthetic or partial model, for instance). Instead
//! `Files` carries a small `fn` pointer that `OneWili::files()` (generated
//! per firmware model, in lib.rs) wires up to the real
//! `hardware().file_system().set_sd_card_host()` binding when that command
//! exists, or to a stub returning a clear error when it doesn't -- so this
//! module always compiles, and callers on a firmware tree without the
//! command get a runtime error instead of the crate failing to build.

use crate::transport::{OwError, Transport};
use std::time::Duration;

// No `use std::io::{Read, Write}` needed: the port parameter's static type
// is `&mut dyn serialport::SerialPort`, and method calls on a trait object
// resolve directly against that trait (and its supertraits, which is where
// `read`/`write_all` actually live) without importing them separately.

const CHUNK: usize = 64; // payload bytes per write -- matches fwSerial / ow_files.h
const IDLE: Duration = Duration::from_millis(500); // per-read silence budget (ow_files.h: OW_FILES_IDLE_MS)
const EMPTY_READS_SILENCE: u32 = 2; // consecutive empty reads == silence (ow_files.h: OW_FILES_EMPTY_READS)
const RESET: [u8; 2] = [0x03, b'\n']; // Ctrl-C: full reset incl. verbose echo -- NOT the 0x02 quiet reset `call()` uses

/// One entry from `Files::list`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct DirEntry {
    pub name: String,
    pub is_dir: bool,
    pub size: u32,
}

/// CRC-32 (IEEE 802.3 / zlib, reflected polynomial 0xEDB88320), ported bit
/// for bit from `ow_files_crc32*` (ow_files.h). Hand-rolled rather than a
/// crate dependency, per the same constraint the C runtime is built under.
fn crc32(data: &[u8]) -> u32 {
    let mut crc: u32 = 0xFFFFFFFF;
    for &byte in data {
        let mut octet = byte as u32;
        for _ in 0..8 {
            crc = if (crc ^ octet) & 1 != 0 { (crc >> 1) ^ 0xEDB88320 } else { crc >> 1 };
            octet >>= 1;
        }
    }
    !crc
}

fn build_put_header(dev_path: &str, size: u32, crc: u32) -> String {
    format!("h\nx\nf\n{dev_path} {size} {crc}\n")
}

fn build_get_header(dev_path: &str) -> String {
    // The device wants backslash-separated paths on this command, and the
    // proven implementation emits a space before the newline.
    format!("h\nx\nu\n{} \n", dev_path.replace('/', "\\"))
}

/// `line` parsed as a framed response/event whose path token is exactly
/// `tag` (e.g. "x\\f", "x\\u", "*fdir"), or `None` if it isn't one. Reuses
/// `framing::parse`, which already strips the timestamp/sequence/ok
/// wrapper generically -- `frame.response` is already exactly the payload
/// text the trailer parsers / `parse_fdir_entry` expect.
///
/// Used for COMPLETION frames only (put's "x\\f" trailer, get's "x\\u"
/// trailer, list's "*fdir" events): those tags are fixed literals the
/// firmware source hardcodes at the call site, not derived from menu
/// navigation, so pinning to them exactly is safe. The put/get HANDSHAKE
/// loops (above `put`/`get`, below) call `framing::parse` directly instead,
/// without a tag check, because the handshake frame's tag is the full
/// navigation prefix to that menu ("h\\x\\f"/"h\\x\\u") -- a different,
/// context-dependent value this function's exact-match would never see.
fn tagged(line: &str, tag: &str) -> Option<crate::framing::ResponseFrame> {
    let frame = crate::framing::parse(line)?;
    if frame.path == tag { Some(frame) } else { None }
}

enum Fdir {
    Entry(DirEntry),
    End,
}

/// One fdir payload -- "dir <name> <size>" / "fil <name> <size>" /
/// "end <count>" -- already stripped of its timestamp/sequence/ok wrapper
/// by `tagged()`. `None` for anything unrecognised (keep listening).
///
/// Mirrors `ow_files_parse_fdir`: the size is the payload's LAST token,
/// taken only when it is all-digits, so names may contain spaces. Firmware
/// from before the listing fix omits the size field entirely, which is
/// where ow_files.h documents an unavoidable, wire-level ambiguity for
/// legacy multi-word names ending in a numeric word; see that file for the
/// full explanation. Current firmware always sends the size field.
fn parse_fdir_entry(payload: &str) -> Option<Fdir> {
    if payload == "end" || payload.starts_with("end ") {
        return Some(Fdir::End);
    }
    for (prefix, is_dir) in [("dir ", true), ("fil ", false)] {
        let rest = match payload.strip_prefix(prefix) {
            Some(r) => r.trim(),
            None => continue,
        };
        if rest.is_empty() {
            return None;
        }
        if let Some((name, last)) = rest.rsplit_once(' ') {
            if !name.is_empty() && !last.is_empty() && last.bytes().all(|b| b.is_ascii_digit()) {
                if let Ok(size) = last.parse::<u32>() {
                    return Some(Fdir::Entry(DirEntry { name: name.to_string(), is_dir, size }));
                }
            }
        }
        return Some(Fdir::Entry(DirEntry { name: rest.to_string(), is_dir, size: 0 }));
    }
    None
}

/// A `get` trailer's payload is "success <N> bytes <CRC> crc"
/// (fwMenuFileSystem.cpp's doUpload). Right-anchored: the last token is
/// checked as the literal word "crc" so a wrong-shaped payload can't be
/// silently misread as a crc value.
fn extract_trailer_crc(response: &str) -> Option<u32> {
    let toks: Vec<&str> = response.split_whitespace().collect();
    if toks.len() < 2 || toks[toks.len() - 1] != "crc" {
        return None;
    }
    toks[toks.len() - 2].parse().ok()
}

/// Byte-buffered reader/writer against a `Transport`'s raw port. Ports
/// `ow_files_rx` (ow_files.h) closely enough to diff by eye. Restores the
/// port's original read timeout on drop, whichever way the transfer ends.
struct RawIo<'a> {
    port: &'a mut dyn serialport::SerialPort,
    original_timeout: Duration,
    buf: Vec<u8>,
    pos: usize,
}

impl<'a> RawIo<'a> {
    /// Starts with an empty buffer: any bytes `Transport::take_buffered()`
    /// returns belong to an earlier, unrelated exchange (see its doc
    /// comment) and must be discarded, not fed in here as if they were the
    /// start of this transfer.
    fn new(port: &'a mut dyn serialport::SerialPort) -> Result<Self, OwError> {
        let original_timeout = port.timeout();
        port.set_timeout(IDLE).map_err(|e| OwError::Io(e.to_string()))?;
        Ok(RawIo { port, original_timeout, buf: Vec::new(), pos: 0 })
    }

    fn write(&mut self, data: &[u8]) -> Result<(), OwError> {
        self.port.write_all(data).map_err(|e| OwError::Io(e.to_string()))
    }

    /// Refill when drained. Ok(true) = bytes available, Ok(false) = silence
    /// (two consecutive empty reads, mirroring OW_FILES_EMPTY_READS).
    fn fill(&mut self) -> Result<bool, OwError> {
        if self.pos < self.buf.len() {
            return Ok(true);
        }
        let mut empties = 0u32;
        while empties < EMPTY_READS_SILENCE {
            let mut chunk = [0u8; 4096];
            match self.port.read(&mut chunk) {
                Ok(0) => empties += 1,
                Ok(n) => {
                    self.buf = chunk[..n].to_vec();
                    self.pos = 0;
                    return Ok(true);
                }
                Err(ref e) if e.kind() == std::io::ErrorKind::TimedOut => empties += 1,
                Err(e) => return Err(OwError::Io(e.to_string())),
            }
        }
        Ok(false)
    }

    fn read_byte(&mut self) -> Result<Option<u8>, OwError> {
        if !self.fill()? {
            return Ok(None);
        }
        let b = self.buf[self.pos];
        self.pos += 1;
        Ok(Some(b))
    }

    /// One '\n'-terminated line ('\r' dropped, terminator dropped). `None`
    /// on silence with nothing partial buffered (mirrors ow_files_read_line).
    fn read_line(&mut self) -> Result<Option<String>, OwError> {
        let mut out = Vec::new();
        loop {
            match self.read_byte()? {
                None => {
                    if out.is_empty() {
                        return Ok(None);
                    }
                    break;
                }
                Some(b'\n') => break,
                Some(b'\r') => {}
                Some(b) => out.push(b),
            }
        }
        Ok(Some(String::from_utf8_lossy(&out).into_owned()))
    }

    /// Exactly `n` raw payload bytes.
    fn read_payload(&mut self, n: usize) -> Result<Vec<u8>, OwError> {
        let mut out = Vec::with_capacity(n);
        while out.len() < n {
            if !self.fill()? {
                return Err(OwError::Timeout(format!("{} more payload byte(s)", n - out.len())));
            }
            let take = (n - out.len()).min(self.buf.len() - self.pos);
            out.extend_from_slice(&self.buf[self.pos..self.pos + take]);
            self.pos += take;
        }
        Ok(out)
    }
}

impl<'a> Drop for RawIo<'a> {
    fn drop(&mut self) {
        let _ = self.port.set_timeout(self.original_timeout);
    }
}

/// File transfer and directory listing over the FreeWili filesystem menu.
pub struct Files<'a> {
    pub(crate) t: &'a mut Transport,
    /// Wired up by the generated `OneWili::files()` (see this module's doc
    /// comment for why it's a plain `fn` pointer rather than a direct
    /// `crate::menus::*` reference).
    pub(crate) sd_host_select_impl: fn(&mut Transport, i32) -> Result<(), OwError>,
}

impl<'a> Files<'a> {
    /// Byte-exact handle for one raw exchange: drains and discards any
    /// bytes an earlier, unrelated `call()` left buffered (they cannot
    /// belong to the transfer about to start -- see
    /// `Transport::take_buffered()`), then borrows the port. See
    /// `Transport::raw_port()` for why the borrow alone is enough here.
    fn raw(&mut self) -> Result<RawIo<'_>, OwError> {
        let _stale = self.t.take_buffered();   // discarded: see take_buffered's doc comment
        RawIo::new(self.t.raw_port())
    }

    /// Upload `data` to `dev_path` on the device.
    pub fn put(&mut self, dev_path: &str, data: &[u8]) -> Result<(), OwError> {
        let header = build_put_header(dev_path, data.len() as u32, crc32(data));
        let mut io = self.raw()?;
        io.write(&RESET)?;
        io.write(header.as_bytes())?;

        // Framed handshake (tag-tolerant, body-matched -- see the module
        // doc comment): "Send File Now" clears us to stream; "Invalid" (a
        // rejected path/size/crc line) means not one payload byte should go
        // out.
        loop {
            let line = io.read_line()?
                .ok_or_else(|| OwError::Timeout(format!("put {dev_path:?}: waiting for handshake")))?;
            let body = match crate::framing::parse(&line) {
                Some(f) => f.response,
                None => continue,   // not a framed response: event, chatter, etc.
            };
            if body == "Send File Now" {
                break;
            }
            if body == "Invalid" {
                return Err(OwError::Failed(format!(
                    "put {dev_path:?}: device rejected the request (Invalid)"
                )));
            }
            // some other framed response: unrelated, keep waiting
        }

        let mut sent = 0usize;
        while sent < data.len() {
            let end = (sent + CHUNK).min(data.len());
            io.write(&data[sent..end])?;
            sent = end;
        }

        // The device verifies its own CRC after the payload lands and
        // reports the result as a framed x\f response: ok on
        // "success <N> bytes", failure (and file deletion) on
        // "Failed checksum".
        loop {
            let line = io.read_line()?
                .ok_or_else(|| OwError::Timeout(format!("put {dev_path:?}: waiting for completion")))?;
            let frame = match tagged(&line, "x\\f") {
                Some(f) => f,
                None => continue,
            };
            return if frame.success {
                Ok(())
            } else {
                Err(OwError::Failed(format!(
                    "put {dev_path:?}: device reported failure: {}",
                    frame.response
                )))
            };
        }
    }

    /// Download `dev_path` from the device.
    pub fn get(&mut self, dev_path: &str) -> Result<Vec<u8>, OwError> {
        let header = build_get_header(dev_path);
        let mut io = self.raw()?;
        io.write(&RESET)?;
        io.write(header.as_bytes())?;

        // Framed handshake (tag-tolerant, body-matched -- see the module
        // doc comment): "RxFile <size>" clears us to read the payload --
        // size ONLY, no crc yet; the crc arrives in the trailer, AFTER the
        // payload. "Invalid" (bad path syntax) or "CantOpenFile" (no such
        // file) both mean there is no payload.
        let size: usize;
        loop {
            let line = io.read_line()?
                .ok_or_else(|| OwError::Timeout(format!("get {dev_path:?}: waiting for handshake")))?;
            let body = match crate::framing::parse(&line) {
                Some(f) => f.response,
                None => continue,   // not a framed response: event, chatter, etc.
            };
            if body == "Invalid" || body == "CantOpenFile" {
                return Err(OwError::Failed(format!("get {dev_path:?}: device reported {body}")));
            }
            if let Some(rest) = body.strip_prefix("RxFile ") {
                // str::parse::<usize>() is ASCII-only by construction (a
                // non-ASCII digit like '²' is simply not a valid digit to
                // it), so unlike Python this needs no separate ascii guard.
                size = rest.trim().parse().map_err(|_| {
                    OwError::Protocol(format!("get {dev_path:?}: unparsable handshake {body:?}"))
                })?;
                break;
            }
            // some other framed response: unrelated, keep waiting
        }

        let payload = io.read_payload(size)?;

        // The crc lives ONLY in this trailer, a framed x\u response with
        // body "success <N> bytes <CRC> crc". No trailer, or one we can't
        // parse, means we do not have a crc to trust.
        loop {
            let line = io.read_line()?.ok_or_else(|| {
                OwError::Protocol(format!("get {dev_path:?}: timeout waiting for the crc trailer"))
            })?;
            let frame = match tagged(&line, "x\\u") {
                Some(f) => f,
                None => continue,
            };
            let crc = extract_trailer_crc(&frame.response).ok_or_else(|| {
                OwError::Protocol(format!(
                    "get {dev_path:?}: trailer missing a crc: {:?}",
                    frame.response
                ))
            })?;
            if crc32(&payload) != crc {
                return Err(OwError::Protocol(format!("get {dev_path:?}: crc mismatch")));
            }
            return Ok(payload);
        }
    }

    /// Entries of `path` ("" for the current directory).
    pub fn list(&mut self, path: &str) -> Result<Vec<DirEntry>, OwError> {
        let cmd = format!("h\nx\nl\n{path}\n");
        let mut io = self.raw()?;
        io.write(&RESET)?;
        io.write(cmd.as_bytes())?;

        let mut entries = Vec::new();
        // silence (read_line() -> None) ends the loop: legacy firmware, no end record
        while let Some(line) = io.read_line()? {
            let frame = match tagged(&line, "*fdir") {
                Some(f) => f,
                None => continue, // the command's own response, unrelated chatter, etc.
            };
            match parse_fdir_entry(&frame.response) {
                Some(Fdir::Entry(e)) => entries.push(e),
                Some(Fdir::End) => break,
                None => {}
            }
        }
        Ok(entries)
    }

    /// Connect the SD card to the USB reader / PC (`true`) or the main CPU
    /// (`false`). No payload to protect from the text decoder, so -- like
    /// `ow_sd_host_select` in the C runtime -- this is just an ordinary
    /// framed menu command, not a raw exchange: it delegates to the
    /// generated binding (wired up by `OneWili::files()`) instead of
    /// re-emitting wire framing here.
    pub fn sd_host_select(&mut self, to_pc: bool) -> Result<(), OwError> {
        (self.sd_host_select_impl)(self.t, if to_pc { 1 } else { 0 })
    }

    /// Upload the contents of the host file at `host_path` to `dev_path`.
    pub fn put_file(&mut self, host_path: &std::path::Path, dev_path: &str) -> Result<(), OwError> {
        let data = std::fs::read(host_path).map_err(|e| OwError::Io(e.to_string()))?;
        self.put(dev_path, &data)
    }

    /// Download `dev_path` and write it to the host file at `host_path`.
    pub fn get_file(&mut self, dev_path: &str, host_path: &std::path::Path) -> Result<(), OwError> {
        let data = self.get(dev_path)?;
        std::fs::write(host_path, &data).map_err(|e| OwError::Io(e.to_string()))
    }
}
