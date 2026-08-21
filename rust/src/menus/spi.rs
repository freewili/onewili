//! SPI Functions menu - generated from fwMenuSPI. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Spi<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Spi<'a> {
    /// SPI Settings sub-menu.
    pub fn settings(self) -> super::spi_settings::SpiSettings<'a> {
        super::spi_settings::SpiSettings { t: self.t }
    }

    /// Write and Read. Writes data to SPI and returns response data. Wire: `i\e\w`
    pub fn s_pi_write(&mut self, data_bytes: &[u8]) -> Result<Vec<u8>, OwError> {
        let mut cmd = String::from("i\\e\\w");
        encoding::push_bytes(&mut cmd, data_bytes);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let spi_response = encoding::rest_bytes(&mut toks)?;
        Ok(spi_response)
    }

    /// SPI Slave Enable. Toggles SPI slave mode using the configured response data. Wire: `i\e\e`
    pub fn s_pi_slave_enable(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\e\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set SPI Slave Response Data. Sets the bytes the SPI slave clocks out (max 8, FIFO depth). Wire: `i\e\l`
    pub fn s_pi_slave_set_data(&mut self, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\e\\l");
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("spislv", false, "data_bytes=hexbytes", "SPI slave received bytes from the master"),
];
