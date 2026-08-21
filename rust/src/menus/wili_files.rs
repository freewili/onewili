//! Wili Files menu - generated from fwMenuWiliFiles. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct WiliFiles<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> WiliFiles<'a> {
    /// Load. Loads a .wili project.. Wire: `s\f\l`
    pub fn wili_load(&mut self, filepath: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\f\\l");
        encoding::push_str(&mut cmd, filepath);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Save Current. Saves the current Wili project to its source path.. Wire: `s\f\s`
    pub fn wili_save(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\f\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Reset. Clears the live panels, Wili Blocks, and app signals.. Wire: `s\f\r`
    pub fn wili_reset(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\f\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Make Default. Sets the Wili project loaded at boot.. Wire: `s\f\m`
    pub fn wili_default(&mut self, filepath: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\f\\m");
        encoding::push_str(&mut cmd, filepath);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Remove Default. Removes the configured boot Wili project.. Wire: `s\f\x`
    pub fn wili_remove_default(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\f\\x");
        self.t.call(&cmd)?;
        Ok(())
    }
}
