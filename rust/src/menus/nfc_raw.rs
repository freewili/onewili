//! Raw Transceiver menu - generated from fwMenuNFCRaw. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct NfcRaw<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> NfcRaw<'a> {
    /// Begin. Initialize the ST25R3916 and take ownership of the NFC front-end. Wire: `w\n\k\b`
    pub fn begin(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\k\\b");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// End. Release the ST25R3916 back to the normal reader/writer state machine. Wire: `w\n\k\e`
    pub fn end(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\k\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Field. Turn the RF field on or off. Wire: `w\n\k\f`
    pub fn field(&mut self, on: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\n\\k\\f");
        encoding::push_int(&mut cmd, on as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Write Register. Write a single ST25R3916 register. Wire: `w\n\k\w`
    pub fn reg_write(&mut self, addr: u32, value: u32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\n\\k\\w");
        encoding::push_hex(&mut cmd, addr as u64, 1);
        encoding::push_hex(&mut cmd, value as u64, 1);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read Register. Read a single ST25R3916 register. Wire: `w\n\k\r`
    pub fn reg_read(&mut self, addr: u32) -> Result<u32, OwError> {
        let mut cmd = String::from("w\\n\\k\\r");
        encoding::push_hex(&mut cmd, addr as u64, 1);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let value = encoding::tok_hex(&mut toks)? as u32;
        Ok(value)
    }

    /// Send Command. Send a direct command to the ST25R3916. Wire: `w\n\k\c`
    pub fn cmd(&mut self, command: u32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\n\\k\\c");
        encoding::push_hex(&mut cmd, command as u64, 1);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Transceive. Transmit bytes and receive the response over the RF field. Wire: `w\n\k\t`
    pub fn transceive(&mut self, flags: u32, timeout_ms: i32, tx: &[u8]) -> Result<(u32, Vec<u8>), OwError> {
        let mut cmd = String::from("w\\n\\k\\t");
        encoding::push_hex(&mut cmd, flags as u64, 1);
        encoding::push_int(&mut cmd, timeout_ms as i64);
        encoding::push_bytes(&mut cmd, tx);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let status = encoding::tok_hex(&mut toks)? as u32;
        let rx = encoding::rest_bytes(&mut toks)?;
        Ok((status, rx))
    }
}
