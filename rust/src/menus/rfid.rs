//! RFID Functions menu - generated from fwMenuRFID. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Rfid<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Rfid<'a> {
    /// Enable Reader. Start or stop the 125 kHz carrier and tag reader. Wire: `w\p\r`
    pub fn enable_reader(&mut self, enable: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\p\\r");
        encoding::push_int(&mut cmd, enable as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Status. Reader state, carrier frequency and live envelope. Wire: `w\p\g`
    pub fn get_status(&mut self) -> Result<(i32, u8, i32, i32, i32, i32, i32, i32), OwError> {
        let cmd = String::from("w\\p\\g");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let state = encoding::tok_int(&mut toks)? as i32;
        let flags = encoding::tok_hex(&mut toks)? as u8;
        let carrier_hz = encoding::tok_int(&mut toks)? as i32;
        let env_min = encoding::tok_int(&mut toks)? as i32;
        let env_max = encoding::tok_int(&mut toks)? as i32;
        let threshold = encoding::tok_int(&mut toks)? as i32;
        let frames = encoding::tok_int(&mut toks)? as i32;
        let tags = encoding::tok_int(&mut toks)? as i32;
        Ok((state, flags, carrier_hz, env_min, env_max, threshold, frames, tags))
    }

    /// Read Tag. Block until one tag is decoded or the timeout expires. Wire: `w\p\t`
    pub fn read_tag(&mut self, timeout_ms: i32) -> Result<(i32, i32, Vec<u8>), OwError> {
        let mut cmd = String::from("w\\p\\t");
        encoding::push_int(&mut cmd, timeout_ms as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let format = encoding::tok_int(&mut toks)? as i32;
        let modulation = encoding::tok_int(&mut toks)? as i32;
        let id = encoding::rest_bytes(&mut toks)?;
        Ok((format, modulation, id))
    }

    /// Stream Tags. Push each decoded tag to the host as an event. Wire: `w\p\s`
    pub fn stream_tags(&mut self, enable: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\p\\s");
        encoding::push_int(&mut cmd, enable as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clear Stats. Zero the frame and tag counters. Wire: `w\p\c`
    pub fn clear_stats(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\p\\c");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Tune Constant. Set a demodulator constant live, without reflashing. Wire: `w\p\u`
    pub fn tune(&mut self, param: i32, value: i32) -> Result<(i32, i32), OwError> {
        let mut cmd = String::from("w\\p\\u");
        encoding::push_int(&mut cmd, param as i64);
        encoding::push_int(&mut cmd, value as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let param = encoding::tok_int(&mut toks)? as i32;
        let value = encoding::tok_int(&mut toks)? as i32;
        Ok((param, value))
    }

    /// Raw Bits. Raw bits of the last assembled frame. Wire: `w\p\b`
    pub fn raw_bits(&mut self) -> Result<(i32, i32, Vec<u8>), OwError> {
        let cmd = String::from("w\\p\\b");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let modulation = encoding::tok_int(&mut toks)? as i32;
        let length = encoding::tok_int(&mut toks)? as i32;
        let bits = encoding::rest_bytes(&mut toks)?;
        Ok((modulation, length, bits))
    }

    /// Write Tag. Write one 32-bit block to a T5577/T5557 tag. Wire: `w\p\w`
    pub fn write_tag(&mut self, block: i32, value: u32) -> Result<(i32, i32), OwError> {
        let mut cmd = String::from("w\\p\\w");
        encoding::push_int(&mut cmd, block as i64);
        encoding::push_hex(&mut cmd, value as u64, 1);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let result = encoding::tok_int(&mut toks)? as i32;
        let block = encoding::tok_int(&mut toks)? as i32;
        Ok((result, block))
    }

    /// Carrier Info. Measured carrier and PSK front-end telemetry. Wire: `w\p\i`
    pub fn carrier_info(&mut self) -> Result<(i32, bool, i32, i32, i32, bool, i32, i32, i32, i32), OwError> {
        let cmd = String::from("w\\p\\i");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let carrier_hz = encoding::tok_int(&mut toks)? as i32;
        let psk_active = encoding::tok_bool(&mut toks)?;
        let psk_events = encoding::tok_int(&mut toks)? as i32;
        let poll_count = encoding::tok_int(&mut toks)? as i32;
        let clk_hz = encoding::tok_int(&mut toks)? as i32;
        let clock_ok = encoding::tok_bool(&mut toks)?;
        let env_samples = encoding::tok_int(&mut toks)? as i32;
        let overruns = encoding::tok_int(&mut toks)? as i32;
        let restarts = encoding::tok_int(&mut toks)? as i32;
        let psk_period = encoding::tok_int(&mut toks)? as i32;
        Ok((carrier_hz, psk_active, psk_events, poll_count, clk_hz, clock_ok, env_samples, overruns, restarts, psk_period))
    }

    /// Enroll ID. Write a caller-supplied EM4100 ID onto the card in the field. Wire: `w\p\n`
    pub fn enroll_id(&mut self, id: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("w\\p\\n");
        encoding::push_bytes(&mut cmd, id);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clone Capture. Read a card and hold its ID for a later clone write. Wire: `w\p\k`
    pub fn clone_capture(&mut self, timeout_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\p\\k");
        encoding::push_int(&mut cmd, timeout_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clone Write. Write the captured ID onto the card now on the coil. Wire: `w\p\j`
    pub fn clone_write(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\p\\j");
        self.t.call(&cmd)?;
        Ok(())
    }
}
