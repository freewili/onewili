//! FPGA Clock menu - generated from fwMenuFPGAClockSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct FpgaClockSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> FpgaClockSettings<'a> {
    /// Clk Source. Choose the clock source that drives the FPGA (CPU clock, oscillator, USB, or RTC). Wire: `h\s\f\c`
    pub fn clk_source(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\f\\c");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clk Divider (int). Set the integer part of the clock divider used to derive the FPGA clock frequency. Wire: `h\s\f\i`
    pub fn clk_divider_int(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\f\\i");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clk Divider (Frac). Set the fractional part of the clock divider used to fine-tune the FPGA clock frequency. Wire: `h\s\f\f`
    pub fn clk_divider_frac(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\f\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Comms Mode. Choose whether the CPU talks to the FPGA configuration registers over SPI or I2C. Wire: `h\s\f\m`
    pub fn comms_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\f\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
