//! MIFARE Classic menu - generated from fwMenuNFCMifareClassic. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct NfcMifareClassic<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> NfcMifareClassic<'a> {
    /// Read with Keys. Authenticate and read sectors using known keys. Wire: `w\n\m\r`
    pub fn read_with_keys(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\m\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Dictionary Attack. Try keys from dictionary file to recover unknown keys. Wire: `w\n\m\a`
    pub fn dictionary_attack(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\n\\m\\a");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Dump Card. Read all sectors with known keys and display contents. Wire: `w\n\m\u`
    pub fn dump_card(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\m\\u");
        self.t.call(&cmd)?;
        Ok(())
    }
}
