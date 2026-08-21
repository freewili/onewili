//! SPI Settings menu - generated from fwMenuSPISettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct SpiSettings2<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> SpiSettings2<'a> {
    /// Frequency. SPI clock frequency in Hz. Wire: `h\s\s\f`
    pub fn frequency(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(162 /* CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_FREQUENCY */, &a)?;
        Ok(())
    }

    /// Chip Select Pin. GPIO pin used as SPI chip select. Wire: `h\s\s\c`
    pub fn chip_select_pin(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(163 /* CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_CHIP_SELECT_PIN */, &a)?;
        Ok(())
    }

    /// Data Bits. SPI data bits per transfer. Wire: `h\s\s\b`
    pub fn data_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(164 /* CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_DATA_BITS */, &a)?;
        Ok(())
    }

    /// CPOL. SPI clock polarity. Wire: `h\s\s\p`
    pub fn c_pol(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(165 /* CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_C_POL */, &a)?;
        Ok(())
    }

    /// CPHA. SPI clock phase. Wire: `h\s\s\a`
    pub fn c_pha(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(166 /* CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_C_PHA */, &a)?;
        Ok(())
    }
}
