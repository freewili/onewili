//! UART Settings menu - generated from fwMenuUARTSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct UartSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> UartSettings<'a> {
    /// Baud Rate. UART baud rate in bits per second. Wire: `i\u\s\f`
    pub fn baud_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(21 /* CMD_IO_UART_SETTINGS_BAUD_RATE */, &a)?;
        Ok(())
    }

    /// RTS Hand Shaking. Enable RTS hardware handshaking. Wire: `i\u\s\r`
    pub fn r_ts_hand_shaking(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(22 /* CMD_IO_UART_SETTINGS_R_TS_HAND_SHAKING */, &a)?;
        Ok(())
    }

    /// CTS Hand Shaking. Enable CTS hardware handshaking. Wire: `i\u\s\c`
    pub fn c_ts_hand_shaking(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(23 /* CMD_IO_UART_SETTINGS_C_TS_HAND_SHAKING */, &a)?;
        Ok(())
    }

    /// Data Bits. UART data bits. Wire: `i\u\s\b`
    pub fn data_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(24 /* CMD_IO_UART_SETTINGS_DATA_BITS */, &a)?;
        Ok(())
    }

    /// Parity. UART parity mode. Wire: `i\u\s\p`
    pub fn parity(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(25 /* CMD_IO_UART_SETTINGS_PARITY */, &a)?;
        Ok(())
    }

    /// Stop Bits. UART stop bits. Wire: `i\u\s\s`
    pub fn stop_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(26 /* CMD_IO_UART_SETTINGS_STOP_BITS */, &a)?;
        Ok(())
    }

    /// Module. Which UART module handles the port. Wire: `i\u\s\m`
    pub fn module(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(27 /* CMD_IO_UART_SETTINGS_MODULE */, &a)?;
        Ok(())
    }
}
