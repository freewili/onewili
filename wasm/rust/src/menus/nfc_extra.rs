//! Extra Actions menu - generated from fwMenuNFCExtra. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct NfcExtra<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> NfcExtra<'a> {
    /// Halt Card. Send HLTA command to put card in HALT state. Wire: `w\n\x\a`
    pub fn halt_card(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(414 /* CMD_WIRELESS_NFC_EXTRA_HALT_CARD */, &a)?;
        Ok(())
    }
}
