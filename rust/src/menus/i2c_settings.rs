//! I2C Settings menu - generated from fwMenuI2CSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct I2cSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> I2cSettings<'a> {
    /// Frequency. I2C bus clock frequency in Hz. Wire: `i\i\s\f`
    pub fn frequency(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\i\\s\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PullUps. Enable I2C bus pull-up resistors. Wire: `i\i\s\p`
    pub fn pull_ups(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\i\\s\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
