//! Raw Transceiver menu - generated from fwMenuNFCRaw. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct NfcRaw<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> NfcRaw<'a> {
    /// Begin. Initialize the ST25R3916 and take ownership of the NFC front-end. Wire: `w\n\k\b`
    pub fn begin(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(401 /* CMD_WIRELESS_NFC_RAW_BEGIN */, &a)?;
        Ok(())
    }

    /// End. Release the ST25R3916 back to the normal reader/writer state machine. Wire: `w\n\k\e`
    pub fn end(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(403 /* CMD_WIRELESS_NFC_RAW_END */, &a)?;
        Ok(())
    }

    /// Field. Turn the RF field on or off. Wire: `w\n\k\f`
    pub fn field(&mut self, on: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(on);
        let _r = crate::transport::call(404 /* CMD_WIRELESS_NFC_RAW_FIELD */, &a)?;
        Ok(())
    }

    /// Write Register. Write a single ST25R3916 register. Wire: `w\n\k\w`
    pub fn reg_write(&mut self, addr: u32, value: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(addr);
        a.u32(value);
        let _r = crate::transport::call(407 /* CMD_WIRELESS_NFC_RAW_REG_WRITE */, &a)?;
        Ok(())
    }

    /// Read Register. Read a single ST25R3916 register. Wire: `w\n\k\r`
    pub fn reg_read(&mut self, addr: u32) -> Result<u32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(addr);
        let mut _r = crate::transport::call(405 /* CMD_WIRELESS_NFC_RAW_REG_READ */, &a)?;
        Ok(_r.u32())
    }

    /// Send Command. Send a direct command to the ST25R3916. Wire: `w\n\k\c`
    pub fn cmd(&mut self, command: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(command);
        let _r = crate::transport::call(402 /* CMD_WIRELESS_NFC_RAW_CMD */, &a)?;
        Ok(())
    }

    /// Transceive. Transmit bytes and receive the response over the RF field. Wire: `w\n\k\t`
    pub fn transceive(&mut self, flags: u32, timeout_ms: i32, tx: &[u8]) -> Result<(u32, Vec<u8>), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(flags);
        a.i32(timeout_ms);
        a.bytes(tx);
        let mut _r = crate::transport::call(406 /* CMD_WIRELESS_NFC_RAW_TRANSCEIVE */, &a)?;
        Ok((_r.u32(), _r.bytes()))
    }
}
