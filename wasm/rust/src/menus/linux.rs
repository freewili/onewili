//! Linux Functions menu - generated from fwMenuLinux. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Linux<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Linux<'a> {
    /// Enable Linux CPU. Not yet implemented; always reports failure. Wire: `l\a`
    pub fn enable_linux_cpu(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(382 /* CMD_LINUX_ENABLE_LINUX_CPU */, &a)?;
        Ok(())
    }

    /// Open Shell. Wire: `l\b`
    pub fn open_shell(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(398 /* CMD_LINUX_OPEN_SHELL */, &a)?;
        Ok(())
    }
}
