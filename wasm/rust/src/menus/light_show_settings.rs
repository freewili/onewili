//! LED Show Settings menu - generated from fwMenuLightShowSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct LightShowSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> LightShowSettings<'a> {
    /// Default Show. The LED show the display boots into. Wire: `h\s\l\c`
    pub fn default_show(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(522 /* CMD_HARDWARE_SETTINGS_HOME_LIGHT_SHOW_SETTINGS_DEFAULT_SHOW */, &a)?;
        Ok(())
    }

    /// LED Strips Enabled. How many external LED strips the built-in show drives. Wire: `h\s\l\s`
    pub fn l_ed_strips_enabled(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(523 /* CMD_HARDWARE_SETTINGS_HOME_LIGHT_SHOW_SETTINGS_L_ED_STRIPS_ENABLED */, &a)?;
        Ok(())
    }

    /// Roku LED Control. Allow a Roku remote to cycle LED show patterns. Wire: `h\s\l\i`
    pub fn roku_led_control(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(524 /* CMD_HARDWARE_SETTINGS_HOME_LIGHT_SHOW_SETTINGS_ROKU_LED_CONTROL */, &a)?;
        Ok(())
    }
}
