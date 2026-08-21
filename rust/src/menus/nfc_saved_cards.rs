//! Saved Cards menu - generated from fwMenuNFCSavedCards. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct NfcSavedCards<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> NfcSavedCards<'a> {
    /// List Saved Cards. List all .nfc files in the saved cards directory. Wire: `w\n\s\l`
    pub fn list_saved_cards(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\s\\l");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Load Card. Load card data from .nfc file. Wire: `w\n\s\o`
    pub fn load_card(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\n\\s\\o");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Save Current Card. Save currently detected card to .nfc file. Wire: `w\n\s\s`
    pub fn save_current_card(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\n\\s\\s");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Play (Emulate) Card. Transmit (emulate) a saved card's NFC-A UID/ATQA/SAK. Wire: `w\n\s\e`
    pub fn emulate_card(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\n\\s\\e");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop Emulation. Stop NFC card emulation. Wire: `w\n\s\t`
    pub fn stop_emulation(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\s\\t");
        self.t.call(&cmd)?;
        Ok(())
    }
}
