//! LED Show Settings menu - generated from fwMenuLightShowSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct LightShowSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> LightShowSettings<'a> {
    /// Default Show. The LED show the display boots into. Wire: `h\s\l\c`
    pub fn default_show(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\l\\c");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// LED Strips Enabled. How many external LED strips the built-in show drives. Wire: `h\s\l\s`
    pub fn l_ed_strips_enabled(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\l\\s");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Roku LED Control. Allow a Roku remote to cycle LED show patterns. Wire: `h\s\l\i`
    pub fn roku_led_control(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\l\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Brightness. Onboard LED strip brightness divisor, 1 (brightest) to 16 (dimmest). Wire: `h\s\l\b`
    pub fn brightness(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\l\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
