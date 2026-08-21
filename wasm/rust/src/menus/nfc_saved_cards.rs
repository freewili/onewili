//! Saved Cards menu - generated from fwMenuNFCSavedCards. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct NfcSavedCards<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> NfcSavedCards<'a> {
    /// List Saved Cards. List all .nfc files in the saved cards directory. Wire: `w\n\s\l`
    pub fn list_saved_cards(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(411 /* CMD_WIRELESS_NFC_SAVED_CARDS_LIST_SAVED_CARDS */, &a)?;
        Ok(())
    }

    /// Load Card. Load card data from .nfc file. Wire: `w\n\s\o`
    pub fn load_card(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(412 /* CMD_WIRELESS_NFC_SAVED_CARDS_LOAD_CARD */, &a)?;
        Ok(())
    }

    /// Save Current Card. Save currently detected card to .nfc file. Wire: `w\n\s\s`
    pub fn save_current_card(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(413 /* CMD_WIRELESS_NFC_SAVED_CARDS_SAVE_CURRENT_CARD */, &a)?;
        Ok(())
    }

    /// Play (Emulate) Card. Transmit (emulate) a saved card's NFC-A UID/ATQA/SAK. Wire: `w\n\s\e`
    pub fn emulate_card(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(501 /* CMD_WIRELESS_NFC_SAVED_CARDS_EMULATE_CARD */, &a)?;
        Ok(())
    }

    /// Stop Emulation. Stop NFC card emulation. Wire: `w\n\s\t`
    pub fn stop_emulation(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(502 /* CMD_WIRELESS_NFC_SAVED_CARDS_STOP_EMULATION */, &a)?;
        Ok(())
    }
}
