//! UART Settings menu - generated from fwMenuUARTSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct UartSettings2<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> UartSettings2<'a> {
    /// Baud Rate. UART baud rate in bits per second. Wire: `h\s\u\f`
    pub fn baud_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(147 /* CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_BAUD_RATE */, &a)?;
        Ok(())
    }

    /// RTS Hand Shaking. Enable RTS hardware handshaking. Wire: `h\s\u\r`
    pub fn r_ts_hand_shaking(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(148 /* CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_R_TS_HAND_SHAKING */, &a)?;
        Ok(())
    }

    /// CTS Hand Shaking. Enable CTS hardware handshaking. Wire: `h\s\u\c`
    pub fn c_ts_hand_shaking(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(149 /* CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_C_TS_HAND_SHAKING */, &a)?;
        Ok(())
    }

    /// Data Bits. UART data bits. Wire: `h\s\u\b`
    pub fn data_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(150 /* CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_DATA_BITS */, &a)?;
        Ok(())
    }

    /// Parity. UART parity mode. Wire: `h\s\u\p`
    pub fn parity(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(151 /* CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_PARITY */, &a)?;
        Ok(())
    }

    /// Stop Bits. UART stop bits. Wire: `h\s\u\s`
    pub fn stop_bits(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(152 /* CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_STOP_BITS */, &a)?;
        Ok(())
    }

    /// Module. Which UART module handles the port. Wire: `h\s\u\m`
    pub fn module(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(153 /* CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_MODULE */, &a)?;
        Ok(())
    }
}
