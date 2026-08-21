//! Linux Functions menu - generated from fwMenuLinux. Do not edit.

use crate::transport::{OwError, Transport};

pub struct Linux<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Linux<'a> {
    /// Enable Linux CPU. Not yet implemented; always reports failure. Wire: `l\a`
    pub fn enable_linux_cpu(&mut self) -> Result<(), OwError> {
        let cmd = String::from("l\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Open Shell. Wire: `l\b`
    pub fn open_shell(&mut self) -> Result<(), OwError> {
        let cmd = String::from("l\\b");
        self.t.call(&cmd)?;
        Ok(())
    }
}
