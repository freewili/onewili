//! LoRa menu - generated from fwMenuLoRa. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct LoRa<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> LoRa<'a> {
    /// Configure. LoRa modem params:. Wire: `w\l\c`
    pub fn configure(&mut self, freq_hz: i32, sf: i32, bw_enc: i32, cr: i32, power: i32, preamble: i32, sync: u8) -> Result<(), OwError> {
        let mut cmd = String::from("w\\l\\c");
        encoding::push_int(&mut cmd, freq_hz as i64);
        encoding::push_int(&mut cmd, sf as i64);
        encoding::push_int(&mut cmd, bw_enc as i64);
        encoding::push_int(&mut cmd, cr as i64);
        encoding::push_int(&mut cmd, power as i64);
        encoding::push_int(&mut cmd, preamble as i64);
        encoding::push_hex(&mut cmd, sync as u64, 2);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Send. Transmits a LoRa packet. Wire: `w\l\s`
    pub fn send_payload(&mut self, data: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("w\\l\\s");
        encoding::push_bytes(&mut cmd, data);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// RX Enable. RX control:. Wire: `w\l\r`
    pub fn rx_enable(&mut self, mode: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\l\\r");
        encoding::push_int(&mut cmd, mode as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Status. WIO-E5 bridge status (a 'lora' STATUS event):. Wire: `w\l\t`
    pub fn status(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\l\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Raw Frame. Sends a raw framed command to the bridge (advanced). Wire: `w\l\f`
    pub fn raw_frame(&mut self, cmd_: u8, payload: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("w\\l\\f");
        encoding::push_hex(&mut cmd, cmd_ as u64, 2);
        encoding::push_bytes(&mut cmd, payload);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("lora", false, "data=string", "LoRa RX / status / event line (free-form text)"),
];
