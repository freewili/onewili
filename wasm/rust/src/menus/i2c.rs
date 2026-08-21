//! I2C Functions menu - generated from fwMenuI2C. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct I2c<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> I2c<'a> {
    /// I2C Settings sub-menu.
    pub fn settings(self) -> super::i2c_settings::I2cSettings<'a> {
        super::i2c_settings::I2cSettings { t: self.t }
    }

    /// Write. Writes data to a specific I2C Address. Wire: `i\i\w`
    pub fn i2c_write(&mut self, address: u8, register: u8, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(address);
        a.u8(register);
        a.bytes(data_bytes);
        let _r = crate::transport::call(47 /* CMD_IO_I2C_I2C_WRITE */, &a)?;
        Ok(())
    }

    /// Read. Reads the number from the address. Wire: `i\i\r`
    pub fn i2c_read(&mut self) -> Result<Vec<u8>, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(48 /* CMD_IO_I2C_I2C_READ */, &a)?;
        Ok(_r.bytes())
    }

    /// Poll. Tests all addresses for I2C Response. Wire: `i\i\p`
    pub fn i2c_poll(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(49 /* CMD_IO_I2C_I2C_POLL */, &a)?;
        Ok(())
    }

    /// I2C Slave Enable. Enables I2C slave mode at the given 7-bit address; 0 disables. Wire: `i\i\e`
    pub fn i2c_slave_enable(&mut self, address: u8) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(address);
        let _r = crate::transport::call(490 /* CMD_IO_I2C_I2C_SLAVE_ENABLE */, &a)?;
        Ok(())
    }

    /// Set I2C Slave Data. Writes bytes into the I2C slave register file. Wire: `i\i\l`
    pub fn i2c_slave_set_data(&mut self, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(data_bytes);
        let _r = crate::transport::call(492 /* CMD_IO_I2C_I2C_SLAVE_SET_DATA */, &a)?;
        Ok(())
    }
}
