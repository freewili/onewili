//! CDC Serial Performance menu - generated from fwMenuCdcPerf. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct CdcPerf<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> CdcPerf<'a> {
    /// Blast. Streams Bytes of deterministic XORshift32 pattern data device-to-host as fast as possible in Chunk-sized writes. Prints the line <<<BLAST>>> before the raw binary begins. Returns bytes,elapsed_us,crc32 (CRC-32 of the payload). Wire: `i\y\b`
    pub fn cdc_perf_blast(&mut self, bytes: i32, chunk: i32) -> Result<(i32, i32, u32), OwError> {
        let mut cmd = String::from("i\\y\\b");
        encoding::push_int(&mut cmd, bytes as i64);
        encoding::push_int(&mut cmd, chunk as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let bytes = encoding::tok_int(&mut toks)? as i32;
        let elapsed_us = encoding::tok_int(&mut toks)? as i32;
        let crc32 = encoding::tok_hex(&mut toks)? as u32;
        Ok((bytes, elapsed_us, crc32))
    }

    /// Sink. Receives exactly Bytes of raw binary host-to-device and CRC-32-accumulates them. Prints the line <<<SINK>>> when ready to receive. A 10 second inactivity timeout aborts with failure. Returns bytes,elapsed_us,crc32. Wire: `i\y\s`
    pub fn cdc_perf_sink(&mut self, bytes: i32) -> Result<(i32, i32, u32), OwError> {
        let mut cmd = String::from("i\\y\\s");
        encoding::push_int(&mut cmd, bytes as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let bytes = encoding::tok_int(&mut toks)? as i32;
        let elapsed_us = encoding::tok_int(&mut toks)? as i32;
        let crc32 = encoding::tok_hex(&mut toks)? as u32;
        Ok((bytes, elapsed_us, crc32))
    }

    /// Echo. Per round reads exactly Chunk raw bytes from the host then writes them back verbatim, Rounds times. Prints the line <<<ECHO>>> when ready for round 1. A 10 second inactivity timeout aborts with failure. Returns rounds,elapsed_us. Wire: `i\y\e`
    pub fn cdc_perf_echo(&mut self, rounds: i32, chunk: i32) -> Result<(i32, i32), OwError> {
        let mut cmd = String::from("i\\y\\e");
        encoding::push_int(&mut cmd, rounds as i64);
        encoding::push_int(&mut cmd, chunk as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let rounds = encoding::tok_int(&mut toks)? as i32;
        let elapsed_us = encoding::tok_int(&mut toks)? as i32;
        Ok((rounds, elapsed_us))
    }
}
