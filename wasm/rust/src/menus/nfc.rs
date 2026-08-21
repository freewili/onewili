//! NFC Functions menu - generated from fwMenuNFC. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Nfc<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
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
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        let _r = crate::transport::call(311 /* CMD_WIRELESS_NFC_ENABLE_READER */, &a)?;
        Ok(())
    }

    /// Print Card Info. Display detailed info about detected card. Wire: `w\n\c`
    pub fn print_card_info(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(399 /* CMD_WIRELESS_NFC_PRINT_CARD_INFO */, &a)?;
        Ok(())
    }

    /// Get Status (debug). Display NFC hardware state and debug info. Wire: `w\n\g`
    pub fn get_status(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(400 /* CMD_WIRELESS_NFC_GET_STATUS */, &a)?;
        Ok(())
    }
}
