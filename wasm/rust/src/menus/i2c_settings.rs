//! I2C Settings menu - generated from fwMenuI2CSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct I2cSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> I2cSettings<'a> {
    /// Frequency. I2C bus clock frequency in Hz. Wire: `i\i\s\f`
    pub fn frequency(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(50 /* CMD_IO_I2C_SETTINGS_FREQUENCY */, &a)?;
        Ok(())
    }

    /// PullUps. Enable I2C bus pull-up resistors. Wire: `i\i\s\p`
    pub fn pull_ups(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(51 /* CMD_IO_I2C_SETTINGS_PULL_UPS */, &a)?;
        Ok(())
    }
}
