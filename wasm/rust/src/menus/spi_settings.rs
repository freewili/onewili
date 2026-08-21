//! SPI Settings menu - generated from fwMenuSPISettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct SpiSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> SpiSettings<'a> {
    /// Frequency. SPI clock frequency in Hz. Wire: `i\e\s\f`
    pub fn frequency(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(53 /* CMD_IO_SPI_SETTINGS_FREQUENCY */, &a)?;
        Ok(())
    }

    /// Chip Select Pin. GPIO pin used as SPI chip select. Wire: `i\e\s\c`
    pub fn chip_select_pin(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(54 /* CMD_IO_SPI_SETTINGS_CHIP_SELECT_PIN */, &a)?;
        Ok(())
    }

    /// Data Bits. SPI data bits per transfer. Wire: `i\e\s\b`
    pub fn data_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(55 /* CMD_IO_SPI_SETTINGS_DATA_BITS */, &a)?;
        Ok(())
    }

    /// CPOL. SPI clock polarity. Wire: `i\e\s\p`
    pub fn c_pol(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(56 /* CMD_IO_SPI_SETTINGS_C_POL */, &a)?;
        Ok(())
    }

    /// CPHA. SPI clock phase. Wire: `i\e\s\a`
    pub fn c_pha(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(57 /* CMD_IO_SPI_SETTINGS_C_PHA */, &a)?;
        Ok(())
    }
}
