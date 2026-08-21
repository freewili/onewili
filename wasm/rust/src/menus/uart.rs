//! UART Functions menu - generated from fwMenuUART. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Uart<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Uart<'a> {
    /// UART Settings sub-menu.
    pub fn settings(self) -> super::uart_settings::UartSettings<'a> {
        super::uart_settings::UartSettings { t: self.t }
    }

    /// Write. Writes data to a specific I2C Address. Wire: `i\u\w`
    pub fn u_art_write(&mut self, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(data_bytes);
        let _r = crate::transport::call(18 /* CMD_IO_UART_U_ART_WRITE */, &a)?;
        Ok(())
    }

    /// Enable UART Read Events. Reads the number from the address. Wire: `i\u\r`
    pub fn toggle_stream(&mut self) -> Result<Vec<u8>, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(19 /* CMD_IO_UART_TOGGLE_STREAM */, &a)?;
        Ok(_r.bytes())
    }

    /// Enable UART API mode. Tests all addresses for I2C Response. Wire: `i\u\t`
    pub fn uart_enable_api_mode(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(20 /* CMD_IO_UART_UART_ENABLE_API_MODE */, &a)?;
        Ok(())
    }
}
