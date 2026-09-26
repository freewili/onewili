//! WILI binary-frame parser (FTDI event stream). Incremental: state persists
//! across `feed` calls; on a bad marker or bogus length the parser drops one
//! byte and rescans (`resyncs`). Stream format per frame:
//! marker "WILI" (4) + repeat_count (LE u16) + header_type (LE u16) +
//! errorbit<<31|payload_len (LE u32) + payload bytes.

pub const HEADER_SIZE: usize = 12;
pub const MAX_PAYLOAD: usize = (1 << 20) + 2048 + 44;

#[derive(Debug, Clone, PartialEq)]
pub struct RawFrame {
    pub header_type: u16,
    pub repeat_count: u16,
    pub payload: Vec<u8>,
    pub error: bool,
}

#[derive(Default)]
pub struct Parser {
    buf: Vec<u8>,
    pub resyncs: u32,
}

impl Parser {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn reset(&mut self) {
        self.buf.clear();
    }

    pub fn feed(&mut self, data: &[u8]) -> Vec<RawFrame> {
        self.buf.extend_from_slice(data);
        let mut out = Vec::new();
        loop {
            if self.buf.len() < 4 {
                break;
            }
            if &self.buf[..4] != b"WILI" {
                match self.buf.windows(4).position(|w| w == b"WILI") {
                    Some(p) => {
                        self.resyncs += p as u32;
                        self.buf.drain(..p);
                    }
                    None => {
                        // keep the last 3 bytes: they may start a marker
                        let drop = self.buf.len() - 3;
                        self.resyncs += drop as u32;
                        self.buf.drain(..drop);
                        break;
                    }
                }
            }
            if self.buf.len() < HEADER_SIZE {
                break;
            }
            let repeat_count = u16::from_le_bytes([self.buf[4], self.buf[5]]);
            let header_type = u16::from_le_bytes([self.buf[6], self.buf[7]]);
            let lw = u32::from_le_bytes([self.buf[8], self.buf[9], self.buf[10], self.buf[11]]);
            let error = lw & 0x8000_0000 != 0;
            let plen = (lw & 0x7FFF_FFFF) as usize;
            if plen > MAX_PAYLOAD {
                self.buf.drain(..1);   // bogus length: not a real header
                self.resyncs += 1;
                continue;
            }
            if self.buf.len() < HEADER_SIZE + plen {
                break;
            }
            let payload = self.buf[HEADER_SIZE..HEADER_SIZE + plen].to_vec();
            self.buf.drain(..HEADER_SIZE + plen);
            out.push(RawFrame { header_type, repeat_count, payload, error });
        }
        out
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn frame(t: u16, payload: &[u8], error: bool) -> Vec<u8> {
        let mut f = b"WILI".to_vec();
        f.extend_from_slice(&1u16.to_le_bytes());
        f.extend_from_slice(&t.to_le_bytes());
        let lw = payload.len() as u32 | if error { 0x8000_0000 } else { 0 };
        f.extend_from_slice(&lw.to_le_bytes());
        f.extend_from_slice(payload);
        f
    }

    #[test]
    fn clean_frame() {
        let mut p = Parser::new();
        let got = p.feed(&frame(7, &[1, 2, 3], false));
        assert_eq!(got.len(), 1);
        assert_eq!(got[0].header_type, 7);
        assert_eq!(got[0].payload, vec![1, 2, 3]);
        assert!(!got[0].error);
        assert_eq!(p.resyncs, 0);
    }

    #[test]
    fn split_feed() {
        let f = frame(3, &[9, 8, 7, 6, 5], false);
        for split in 1..f.len() {
            let mut p = Parser::new();
            let mut got = p.feed(&f[..split]);
            got.extend(p.feed(&f[split..]));
            assert_eq!(got.len(), 1, "split={split}");
            assert_eq!(got[0].payload, vec![9, 8, 7, 6, 5]);
        }
    }

    #[test]
    fn garbage_resync() {
        let mut p = Parser::new();
        let mut bytes = vec![0x00, 0xFF, b'W', b'X'];
        bytes.extend(frame(1, &[0xAA], false));
        let got = p.feed(&bytes);
        assert_eq!(got.len(), 1);
        assert!(p.resyncs > 0);
    }

    #[test]
    fn oversize_resync() {
        let mut p = Parser::new();
        let mut bogus = frame(1, &[], false);
        bogus[8] = 0xFF; bogus[9] = 0xFF; bogus[10] = 0xFF; bogus[11] = 0x7F;
        bogus.extend(frame(2, &[0x55], false));
        let got = p.feed(&bogus);
        assert_eq!(got.len(), 1);
        assert_eq!(got[0].header_type, 2);
        assert!(p.resyncs > 0);
    }

    #[test]
    fn error_bit() {
        let mut p = Parser::new();
        let got = p.feed(&frame(5, &[1, 2], true));
        assert_eq!(got.len(), 1);
        assert!(got[0].error);
        assert_eq!(got[0].payload.len(), 2);
    }

    #[test]
    fn maximum_capture_and_unknown_frame_preserve_payload() {
        let mut p = Parser::new();
        let payload = vec![0xaa; MAX_PAYLOAD];
        let wire = frame(65535, &payload, true);
        let mut got = Vec::new();
        for part in wire.chunks(509) { got.extend(p.feed(part)); }
        assert_eq!(got.len(), 1);
        assert_eq!(got[0].header_type, 65535);
        assert_eq!(got[0].payload, payload);
        assert!(got[0].error);
    }
}
