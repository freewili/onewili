//! RF Analyzer Settings menu - generated from fwMenuRadioFASettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct RadioFaSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> RadioFaSettings<'a> {
    /// Default View. Default view for the RF Analyzer. Wire: `h\s\a\a`
    pub fn default_view(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(230 /* CMD_HARDWARE_SETTINGS_HOME_RADIO_FA_SETTINGS_DEFAULT_VIEW */, &a)?;
        Ok(())
    }
}
