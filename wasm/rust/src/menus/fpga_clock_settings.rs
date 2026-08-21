//! FPGA Clock menu - generated from fwMenuFPGAClockSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct FpgaClockSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> FpgaClockSettings<'a> {
    /// Clk Source. Choose the clock source that drives the FPGA (CPU clock, oscillator, USB, or RTC). Wire: `h\s\f\c`
    pub fn clk_source(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(178 /* CMD_HARDWARE_SETTINGS_HOME_FPGA_CLOCK_SETTINGS_CLK_SOURCE */, &a)?;
        Ok(())
    }

    /// Clk Divider (int). Set the integer part of the clock divider used to derive the FPGA clock frequency. Wire: `h\s\f\i`
    pub fn clk_divider_int(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(179 /* CMD_HARDWARE_SETTINGS_HOME_FPGA_CLOCK_SETTINGS_CLK_DIVIDER_INT */, &a)?;
        Ok(())
    }

    /// Clk Divider (Frac). Set the fractional part of the clock divider used to fine-tune the FPGA clock frequency. Wire: `h\s\f\f`
    pub fn clk_divider_frac(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(180 /* CMD_HARDWARE_SETTINGS_HOME_FPGA_CLOCK_SETTINGS_CLK_DIVIDER_FRAC */, &a)?;
        Ok(())
    }

    /// Comms Mode. Choose whether the CPU talks to the FPGA configuration registers over SPI or I2C. Wire: `h\s\f\m`
    pub fn comms_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(181 /* CMD_HARDWARE_SETTINGS_HOME_FPGA_CLOCK_SETTINGS_COMMS_MODE */, &a)?;
        Ok(())
    }
}
