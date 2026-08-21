//! Interface Settings menu - generated from fwMenuInterfaceSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct InterfaceSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> InterfaceSettings<'a> {
    /// Double Click Ms. Button double-click window in milliseconds (currently inert; the display uses a compile-time window). Wire: `h\s\x\c`
    pub fn double_click_ms(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\x\\c");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Roku Gui Control. Allow a Roku remote to drive the display GUI buttons. Wire: `h\s\x\r`
    pub fn roku_gui_control(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\x\\r");
        self.t.call(&cmd)?;
        Ok(())
    }
}
