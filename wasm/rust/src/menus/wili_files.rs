//! Wili Files menu - generated from fwMenuWiliFiles. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct WiliFiles<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> WiliFiles<'a> {
    /// Load. Loads a .wili project.. Wire: `s\f\l`
    pub fn wili_load(&mut self, filepath: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filepath);
        let _r = crate::transport::call(441 /* CMD_SCRIPTING_WILI_FILES_WILI_LOAD */, &a)?;
        Ok(())
    }

    /// Save Current. Saves the current Wili project to its source path.. Wire: `s\f\s`
    pub fn wili_save(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(444 /* CMD_SCRIPTING_WILI_FILES_WILI_SAVE */, &a)?;
        Ok(())
    }

    /// Reset. Clears the live panels, Wili Blocks, and app signals.. Wire: `s\f\r`
    pub fn wili_reset(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(443 /* CMD_SCRIPTING_WILI_FILES_WILI_RESET */, &a)?;
        Ok(())
    }

    /// Make Default. Sets the Wili project loaded at boot.. Wire: `s\f\m`
    pub fn wili_default(&mut self, filepath: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filepath);
        let _r = crate::transport::call(442 /* CMD_SCRIPTING_WILI_FILES_WILI_DEFAULT */, &a)?;
        Ok(())
    }

    /// Remove Default. Removes the configured boot Wili project.. Wire: `s\f\x`
    pub fn wili_remove_default(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(445 /* CMD_SCRIPTING_WILI_FILES_WILI_REMOVE_DEFAULT */, &a)?;
        Ok(())
    }
}
