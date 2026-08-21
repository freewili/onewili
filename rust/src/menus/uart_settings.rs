//! UART Settings menu - generated from fwMenuUARTSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct UartSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> UartSettings<'a> {
    /// Baud Rate. UART baud rate in bits per second. Wire: `i\u\s\f`
    pub fn baud_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\u\\s\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// RTS Hand Shaking. Enable RTS hardware handshaking. Wire: `i\u\s\r`
    pub fn r_ts_hand_shaking(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\u\\s\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CTS Hand Shaking. Enable CTS hardware handshaking. Wire: `i\u\s\c`
    pub fn c_ts_hand_shaking(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\u\\s\\c");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Data Bits. UART data bits. Wire: `i\u\s\b`
    pub fn data_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\u\\s\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Parity. UART parity mode. Wire: `i\u\s\p`
    pub fn parity(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\u\\s\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop Bits. UART stop bits. Wire: `i\u\s\s`
    pub fn stop_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\u\\s\\s");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Module. Which UART module handles the port. Wire: `i\u\s\m`
    pub fn module(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\u\\s\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
