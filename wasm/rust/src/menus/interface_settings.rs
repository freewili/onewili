//! Interface Settings menu - generated from fwMenuInterfaceSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct InterfaceSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> InterfaceSettings<'a> {
    /// Double Click Ms. Button double-click window in milliseconds (currently inert; the display uses a compile-time window). Wire: `h\s\x\c`
    pub fn double_click_ms(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(525 /* CMD_HARDWARE_SETTINGS_HOME_INTERFACE_SETTINGS_DOUBLE_CLICK_MS */, &a)?;
        Ok(())
    }

    /// Roku Gui Control. Allow a Roku remote to drive the display GUI buttons. Wire: `h\s\x\r`
    pub fn roku_gui_control(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(526 /* CMD_HARDWARE_SETTINGS_HOME_INTERFACE_SETTINGS_ROKU_GUI_CONTROL */, &a)?;
        Ok(())
    }
}
