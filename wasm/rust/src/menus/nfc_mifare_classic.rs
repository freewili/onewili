//! MIFARE Classic menu - generated from fwMenuNFCMifareClassic. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct NfcMifareClassic<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> NfcMifareClassic<'a> {
    /// Read with Keys. Authenticate and read sectors using known keys. Wire: `w\n\m\r`
    pub fn read_with_keys(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(409 /* CMD_WIRELESS_NFC_MIFARE_CLASSIC_READ_WITH_KEYS */, &a)?;
        Ok(())
    }

    /// Dictionary Attack. Try keys from dictionary file to recover unknown keys. Wire: `w\n\m\a`
    pub fn dictionary_attack(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(408 /* CMD_WIRELESS_NFC_MIFARE_CLASSIC_DICTIONARY_ATTACK */, &a)?;
        Ok(())
    }

    /// Dump Card. Read all sectors with known keys and display contents. Wire: `w\n\m\u`
    pub fn dump_card(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(410 /* CMD_WIRELESS_NFC_MIFARE_CLASSIC_DUMP_CARD */, &a)?;
        Ok(())
    }
}
