//! Orca Communication menu - generated from fwMenuOrcaSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct OrcaSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> OrcaSettings<'a> {
    /// Orca Com over UART. Set Communication protocol for connected Orca device over UART. Wire: `h\s\g\u`
    pub fn orca_com_over_uart(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(250 /* CMD_HARDWARE_SETTINGS_HOME_ORCA_SETTINGS_ORCA_COM_OVER_UART */, &a)?;
        Ok(())
    }
}
