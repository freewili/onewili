//! NFC Functions menu - generated from fwMenuNFC. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Nfc<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Nfc<'a> {
    /// Saved Cards sub-menu.
    pub fn saved_cards(self) -> super::nfc_saved_cards::NfcSavedCards<'a> {
        super::nfc_saved_cards::NfcSavedCards { t: self.t }
    }

    /// MIFARE Classic sub-menu.
    pub fn mifare_classic(self) -> super::nfc_mifare_classic::NfcMifareClassic<'a> {
        super::nfc_mifare_classic::NfcMifareClassic { t: self.t }
    }

    /// Raw Transceiver sub-menu.
    pub fn raw(self) -> super::nfc_raw::NfcRaw<'a> {
        super::nfc_raw::NfcRaw { t: self.t }
    }

    /// Extra Actions sub-menu.
    pub fn extra(self) -> super::nfc_extra::NfcExtra<'a> {
        super::nfc_extra::NfcExtra { t: self.t }
    }

    /// Enable Reader. Enable/disable NFC reader with auto tag streaming. Wire: `w\n\r`
    pub fn enable_reader(&mut self, enable: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\n\\r");
        encoding::push_int(&mut cmd, enable as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Print Card Info. Display detailed info about detected card. Wire: `w\n\c`
    pub fn print_card_info(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\c");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Status (debug). Display NFC hardware state and debug info. Wire: `w\n\g`
    pub fn get_status(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\g");
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("nfc", false, "data=string", "NFC card status text (card detected / removed)"),
];
