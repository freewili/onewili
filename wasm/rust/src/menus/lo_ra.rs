//! LoRa menu - generated from fwMenuLoRa. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct LoRa<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> LoRa<'a> {
    /// Configure. LoRa modem params:. Wire: `w\l\c`
    pub fn configure(&mut self, freq_hz: i32, sf: i32, bw_enc: i32, cr: i32, power: i32, preamble: i32, sync: u8) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(freq_hz);
        a.i32(sf);
        a.i32(bw_enc);
        a.i32(cr);
        a.i32(power);
        a.i32(preamble);
        a.u8(sync);
        let _r = crate::transport::call(353 /* CMD_WIRELESS_LO_RA_CONFIGURE */, &a)?;
        Ok(())
    }

    /// Send. Transmits a LoRa packet. Wire: `w\l\s`
    pub fn send_payload(&mut self, data: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(data);
        let _r = crate::transport::call(354 /* CMD_WIRELESS_LO_RA_SEND_PAYLOAD */, &a)?;
        Ok(())
    }

    /// RX Enable. RX control:. Wire: `w\l\r`
    pub fn rx_enable(&mut self, mode: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(mode);
        let _r = crate::transport::call(355 /* CMD_WIRELESS_LO_RA_RX_ENABLE */, &a)?;
        Ok(())
    }

    /// Status. WIO-E5 bridge status (a 'lora' STATUS event):. Wire: `w\l\t`
    pub fn status(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(356 /* CMD_WIRELESS_LO_RA_STATUS */, &a)?;
        Ok(())
    }

    /// Raw Frame. Sends a raw framed command to the bridge (advanced). Wire: `w\l\f`
    pub fn raw_frame(&mut self, cmd_: u8, payload: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(cmd_);
        a.bytes(payload);
        let _r = crate::transport::call(357 /* CMD_WIRELESS_LO_RA_RAW_FRAME */, &a)?;
        Ok(())
    }
}
