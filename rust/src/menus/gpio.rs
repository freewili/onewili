//! GPIO Functions menu - generated from fwMenuGPIO. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Gpio<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Gpio<'a> {
    /// IO Directions sub-menu.
    pub fn io_direction_settings(self) -> super::io_direction_settings::IoDirectionSettings<'a> {
        super::io_direction_settings::IoDirectionSettings { t: self.t }
    }

    /// High. Sets a GPIO high. Wire: `i\g\s`
    pub fn set_io_high(&mut self, pin: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\g\\s");
        encoding::push_int(&mut cmd, pin as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Low. Sets a GPIO low. Wire: `i\g\l`
    pub fn set_io_low(&mut self, pin: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\g\\l");
        encoding::push_int(&mut cmd, pin as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Toggle. Toggles the specified GPIO. Wire: `i\g\t`
    pub fn set_io_toggle(&mut self, pin: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\g\\t");
        encoding::push_int(&mut cmd, pin as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PWM IO. Enables the PWM feature of GPIO. Wire: `i\g\p`
    pub fn set_pwm(&mut self, gpio_number: i32, freq: f64, duty: f64) -> Result<(), OwError> {
        let mut cmd = String::from("i\\g\\p");
        encoding::push_int(&mut cmd, gpio_number as i64);
        encoding::push_float(&mut cmd, freq);
        encoding::push_float(&mut cmd, duty);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get All IOs (hex). Reads all the IOs in a bitfield. Wire: `i\g\u`
    pub fn read_all(&mut self) -> Result<u32, OwError> {
        let cmd = String::from("i\\g\\u");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let gpiostate = encoding::tok_hex(&mut toks)? as u32;
        Ok(gpiostate)
    }

    /// Stream IO reads. Sends GPIO values as a specific millisecond rate to host. Wire: `i\g\o`
    pub fn stream_io(&mut self, reportratems: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\g\\o");
        encoding::push_int(&mut cmd, reportratems as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Toggle High-Speed Bidirectional IO. Toggle utilizing GPIO27 to set the direction of GPIO26.. Wire: `i\g\e`
    pub fn toggle_hsbdio(&mut self, pin: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\g\\e");
        encoding::push_int(&mut cmd, pin as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set IO Voltage Source. Selects the voltage source connected to the external IO voltage rail.. Wire: `i\g\v`
    pub fn set_io_voltage_source(&mut self, source: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\g\\v");
        encoding::push_int(&mut cmd, source as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("gpioReport", true, "time_stamp_ns=hexU64,gpio_bitfield=hexU32", "Periodic GPIO bitfield report (binary API)"),
];
