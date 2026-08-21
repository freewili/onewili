//! SPI Functions menu - generated from fwMenuSPI. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Spi<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Spi<'a> {
    /// SPI Settings sub-menu.
    pub fn settings(self) -> super::spi_settings::SpiSettings<'a> {
        super::spi_settings::SpiSettings { t: self.t }
    }

    /// Write and Read. Writes data to SPI and returns response data. Wire: `i\e\w`
    pub fn s_pi_write(&mut self, data_bytes: &[u8]) -> Result<Vec<u8>, OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(data_bytes);
        let mut _r = crate::transport::call(52 /* CMD_IO_SPI_S_PI_WRITE */, &a)?;
        Ok(_r.bytes())
    }

    /// SPI Slave Enable. Toggles SPI slave mode using the configured response data. Wire: `i\e\e`
    pub fn s_pi_slave_enable(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(489 /* CMD_IO_SPI_S_PI_SLAVE_ENABLE */, &a)?;
        Ok(())
    }

    /// Set SPI Slave Response Data. Sets the bytes the SPI slave clocks out (max 8, FIFO depth). Wire: `i\e\l`
    pub fn s_pi_slave_set_data(&mut self, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(data_bytes);
        let _r = crate::transport::call(491 /* CMD_IO_SPI_S_PI_SLAVE_SET_DATA */, &a)?;
        Ok(())
    }
}
