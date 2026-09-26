//! CDC Serial Performance menu - generated from fwMenuCdcPerf. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct CdcPerf<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> CdcPerf<'a> {
    /// Blast. Streams Bytes of deterministic XORshift32 pattern data device-to-host as fast as possible in Chunk-sized writes. Prints the line <<<BLAST>>> before the raw binary begins. Returns bytes,elapsed_us,crc32 (CRC-32 of the payload). Wire: `i\y\b`
    pub fn cdc_perf_blast(&mut self, bytes: i32, chunk: i32) -> Result<(i32, i32, u32), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(bytes);
        a.i32(chunk);
        let mut _r = crate::transport::call(540 /* CMD_IO_CDC_PERF_CDC_PERF_BLAST */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.u32()))
    }

    /// Sink. Receives exactly Bytes of raw binary host-to-device and CRC-32-accumulates them. Prints the line <<<SINK>>> when ready to receive. A 10 second inactivity timeout aborts with failure. Returns bytes,elapsed_us,crc32. Wire: `i\y\s`
    pub fn cdc_perf_sink(&mut self, bytes: i32) -> Result<(i32, i32, u32), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(bytes);
        let mut _r = crate::transport::call(542 /* CMD_IO_CDC_PERF_CDC_PERF_SINK */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.u32()))
    }

    /// Echo. Per round reads exactly Chunk raw bytes from the host then writes them back verbatim, Rounds times. Prints the line <<<ECHO>>> when ready for round 1. A 10 second inactivity timeout aborts with failure. Returns rounds,elapsed_us. Wire: `i\y\e`
    pub fn cdc_perf_echo(&mut self, rounds: i32, chunk: i32) -> Result<(i32, i32), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(rounds);
        a.i32(chunk);
        let mut _r = crate::transport::call(541 /* CMD_IO_CDC_PERF_CDC_PERF_ECHO */, &a)?;
        Ok((_r.i32(), _r.i32()))
    }
}
