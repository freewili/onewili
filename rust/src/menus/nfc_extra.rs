//! Extra Actions menu - generated from fwMenuNFCExtra. Do not edit.

use crate::transport::{OwError, Transport};

pub struct NfcExtra<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> NfcExtra<'a> {
    /// Halt Card. Send HLTA command to put card in HALT state. Wire: `w\n\x\a`
    pub fn halt_card(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\n\\x\\a");
        self.t.call(&cmd)?;
        Ok(())
    }
}
