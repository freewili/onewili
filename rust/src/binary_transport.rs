//! Binary (FTDI/WILI) event port. Poll-based, no threads: `poll_raw` reads
//! whatever bytes are available (never blocks) and returns the next frame.
//! Find the port with `transport::find_binary_port()` (FTDI VID 0x0403 -
//! NEVER the "FW2" name substring; the main text port is "FW2 v01").

use crate::binary_framing::{Parser, RawFrame};
use crate::transport::OwError;
use std::collections::VecDeque;
use std::io::Read;

pub struct BinaryTransport {
    port: Box<dyn serialport::SerialPort>,
    parser: Parser,
    pending: VecDeque<RawFrame>,
    pub unknown_frames: u32,
    pub size_mismatches: u32,
}

impl BinaryTransport {
    pub fn open(name: &str) -> Result<Self, OwError> {
        let port = serialport::new(name, 1_000_000)
            .timeout(std::time::Duration::from_millis(100))
            .open()
            .map_err(|e| OwError::Io(e.to_string()))?;
        Ok(BinaryTransport {
            port,
            parser: Parser::new(),
            pending: VecDeque::new(),
            unknown_frames: 0,
            size_mismatches: 0,
        })
    }

    /// Next raw frame, if one is already buffered or completes from the
    /// bytes currently available. Never blocks (`bytes_to_read` gate).
    pub fn poll_raw(&mut self) -> Result<Option<RawFrame>, OwError> {
        if let Some(f) = self.pending.pop_front() {
            return Ok(Some(f));
        }
        let avail = self.port.bytes_to_read().map_err(|e| OwError::Io(e.to_string()))? as usize;
        if avail == 0 {
            return Ok(None);
        }
        let mut buf = vec![0u8; avail.min(4096)];
        let n = match self.port.read(&mut buf) {
            Ok(n) => n,
            Err(ref e) if e.kind() == std::io::ErrorKind::TimedOut => 0,
            Err(e) => return Err(OwError::Io(e.to_string())),
        };
        for f in self.parser.feed(&buf[..n]) {
            self.pending.push_back(f);
        }
        Ok(self.pending.pop_front())
    }
}
