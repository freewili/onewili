//! UART Settings menu - generated from fwMenuUARTSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct UartSettings2<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> UartSettings2<'a> {
    /// Baud Rate. UART baud rate in bits per second. Wire: `h\s\u\f`
    pub fn baud_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\u\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// RTS Hand Shaking. Enable RTS hardware handshaking. Wire: `h\s\u\r`
    pub fn r_ts_hand_shaking(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\u\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CTS Hand Shaking. Enable CTS hardware handshaking. Wire: `h\s\u\c`
    pub fn c_ts_hand_shaking(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\u\\c");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Data Bits. UART data bits. Wire: `h\s\u\b`
    pub fn data_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\u\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Parity. UART parity mode. Wire: `h\s\u\p`
    pub fn parity(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\u\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop Bits. UART stop bits. Wire: `h\s\u\s`
    pub fn stop_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\u\\s");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Module. Which UART module handles the port. Wire: `h\s\u\m`
    pub fn module(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\u\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
