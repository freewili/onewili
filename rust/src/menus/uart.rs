//! UART Functions menu - generated from fwMenuUART. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Uart<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Uart<'a> {
    /// UART Settings sub-menu.
    pub fn settings(self) -> super::uart_settings::UartSettings<'a> {
        super::uart_settings::UartSettings { t: self.t }
    }

    /// Write. Writes data to a specific I2C Address. Wire: `i\u\w`
    pub fn u_art_write(&mut self, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\u\\w");
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Enable UART Read Events. Reads the number from the address. Wire: `i\u\r`
    pub fn toggle_stream(&mut self) -> Result<Vec<u8>, OwError> {
        let cmd = String::from("i\\u\\r");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let data_bytes = encoding::rest_bytes(&mut toks)?;
        Ok(data_bytes)
    }

    /// Enable UART API mode. Tests all addresses for I2C Response. Wire: `i\u\t`
    pub fn uart_enable_api_mode(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\u\\t");
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("uart1", false, "data_bytes=hexbytes", "uart receive frame"),
];
