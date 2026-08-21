//! SPI Settings menu - generated from fwMenuSPISettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct SpiSettings2<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> SpiSettings2<'a> {
    /// Frequency. SPI clock frequency in Hz. Wire: `h\s\s\f`
    pub fn frequency(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\s\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Chip Select Pin. GPIO pin used as SPI chip select. Wire: `h\s\s\c`
    pub fn chip_select_pin(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\s\\c");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Data Bits. SPI data bits per transfer. Wire: `h\s\s\b`
    pub fn data_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\s\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CPOL. SPI clock polarity. Wire: `h\s\s\p`
    pub fn c_pol(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\s\\p");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CPHA. SPI clock phase. Wire: `h\s\s\a`
    pub fn c_pha(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\s\\a");
        self.t.call(&cmd)?;
        Ok(())
    }
}
