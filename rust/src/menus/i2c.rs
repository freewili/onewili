//! I2C Functions menu - generated from fwMenuI2C. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct I2c<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> I2c<'a> {
    /// I2C Settings sub-menu.
    pub fn settings(self) -> super::i2c_settings::I2cSettings<'a> {
        super::i2c_settings::I2cSettings { t: self.t }
    }

    /// Write. Writes data to a specific I2C Address. Wire: `i\i\w`
    pub fn i2c_write(&mut self, address: u8, register: u8, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\i\\w");
        encoding::push_hex(&mut cmd, address as u64, 2);
        encoding::push_hex(&mut cmd, register as u64, 2);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read. Reads the number from the address. Wire: `i\i\r`
    pub fn i2c_read(&mut self) -> Result<Vec<u8>, OwError> {
        let cmd = String::from("i\\i\\r");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let i2crepsone = encoding::rest_bytes(&mut toks)?;
        Ok(i2crepsone)
    }

    /// Poll. Tests all addresses for I2C Response. Wire: `i\i\p`
    pub fn i2c_poll(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\i\\p");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// I2C Slave Enable. Enables I2C slave mode at the given 7-bit address; 0 disables. Wire: `i\i\e`
    pub fn i2c_slave_enable(&mut self, address: u8) -> Result<(), OwError> {
        let mut cmd = String::from("i\\i\\e");
        encoding::push_hex(&mut cmd, address as u64, 2);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set I2C Slave Data. Writes bytes into the I2C slave register file. Wire: `i\i\l`
    pub fn i2c_slave_set_data(&mut self, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\i\\l");
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("i2cmon", false, "data_bytes=hexbytes", "I2C monitor captured bytes"),
    ("i2cslv", false, "data_bytes=hexbytes", "I2C slave received master write (register, data bytes)"),
];
