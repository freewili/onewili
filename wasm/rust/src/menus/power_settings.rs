//! Power Settings menu - generated from fwMenuPowerSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct PowerSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> PowerSettings<'a> {
    /// Brightness Powered. Display backlight percent while on USB power. Wire: `h\s\m\a`
    pub fn brightness_powered(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(513 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BRIGHTNESS_POWERED */, &a)?;
        Ok(())
    }

    /// Brightness Batt Gt 70. Display backlight percent with battery above 70 percent. Wire: `h\s\m\b`
    pub fn brightness_batt_gt70(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(514 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BRIGHTNESS_BATT_GT70 */, &a)?;
        Ok(())
    }

    /// Brightness Batt Gt 30. Display backlight percent with battery between 30 and 70 percent. Wire: `h\s\m\c`
    pub fn brightness_batt_gt30(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(515 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BRIGHTNESS_BATT_GT30 */, &a)?;
        Ok(())
    }

    /// Brightness Batt Lt 30. Display backlight percent with battery below 30 percent. Wire: `h\s\m\g`
    pub fn brightness_batt_lt30(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(516 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BRIGHTNESS_BATT_LT30 */, &a)?;
        Ok(())
    }

    /// Battery Timeout. Seconds of inactivity before the display shuts off on battery. Wire: `h\s\m\e`
    pub fn battery_timeout(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(517 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BATTERY_TIMEOUT */, &a)?;
        Ok(())
    }

    /// Powered Timeout. Seconds of inactivity before the display shuts off on USB power, 0 keeps it on. Wire: `h\s\m\f`
    pub fn powered_timeout(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(518 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_POWERED_TIMEOUT */, &a)?;
        Ok(())
    }

    /// Wake On Sound. Wake the display when the mic hears a sound. Wire: `h\s\m\s`
    pub fn wake_on_sound(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(519 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_WAKE_ON_SOUND */, &a)?;
        Ok(())
    }

    /// Wake On Move. Wake the display when the accelerometer sees movement. Wire: `h\s\m\m`
    pub fn wake_on_move(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(520 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_WAKE_ON_MOVE */, &a)?;
        Ok(())
    }

    /// Auto Power Zones. Auto-manage power zones; off keeps every live rail on, commands still raise rails on demand. Wire: `h\s\m\o`
    pub fn auto_power_zones(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(521 /* CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_AUTO_POWER_ZONES */, &a)?;
        Ok(())
    }
}
