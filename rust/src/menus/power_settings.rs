//! Power Settings menu - generated from fwMenuPowerSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct PowerSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> PowerSettings<'a> {
    /// Brightness Powered. Display backlight percent while on USB power. Wire: `h\s\m\a`
    pub fn brightness_powered(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\m\\a");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Brightness Batt Gt 70. Display backlight percent with battery above 70 percent. Wire: `h\s\m\b`
    pub fn brightness_batt_gt70(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\m\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Brightness Batt Gt 30. Display backlight percent with battery between 30 and 70 percent. Wire: `h\s\m\c`
    pub fn brightness_batt_gt30(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\m\\c");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Brightness Batt Lt 30. Display backlight percent with battery below 30 percent. Wire: `h\s\m\g`
    pub fn brightness_batt_lt30(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\m\\g");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Battery Timeout. Seconds of inactivity before the display shuts off on battery. Wire: `h\s\m\e`
    pub fn battery_timeout(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\m\\e");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Powered Timeout. Seconds of inactivity before the display shuts off on USB power, 0 keeps it on. Wire: `h\s\m\f`
    pub fn powered_timeout(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\m\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Wake On Sound. Wake the display when the mic hears a sound. Wire: `h\s\m\s`
    pub fn wake_on_sound(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\m\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Wake On Move. Wake the display when the accelerometer sees movement. Wire: `h\s\m\m`
    pub fn wake_on_move(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\m\\m");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Auto Power Zones. Auto-manage power zones; off keeps every live rail on, commands still raise rails on demand. Wire: `h\s\m\o`
    pub fn auto_power_zones(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\m\\o");
        self.t.call(&cmd)?;
        Ok(())
    }
}
