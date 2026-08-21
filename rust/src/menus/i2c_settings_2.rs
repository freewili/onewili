//! I2C Settings menu - generated from fwMenuI2CSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct I2cSettings2<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> I2cSettings2<'a> {
    /// Frequency. I2C bus clock frequency in Hz. Wire: `h\s\i\f`
    pub fn frequency(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\i\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PullUps. Enable I2C bus pull-up resistors. Wire: `h\s\i\p`
    pub fn pull_ups(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\i\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
