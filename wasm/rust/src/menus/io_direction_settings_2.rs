//! IO Directions menu - generated from fwMenuIODirectionSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct IoDirectionSettings2<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> IoDirectionSettings2<'a> {
    /// SPI1 Rx (12). IO direction for SPI1 Rx pin 12 (out/in). Wire: `h\s\o\a`
    pub fn s_pi1_rx12(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(167 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_S_PI1_RX12 */, &a)?;
        Ok(())
    }

    /// GPIO 26 (26). IO direction for GPIO 26 (out/in). Wire: `h\s\o\b`
    pub fn g_pio2626(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(168 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_G_PIO2626 */, &a)?;
        Ok(())
    }

    /// SPI1 CS (13). IO direction for SPI1 CS pin 13 (out/in). Wire: `h\s\o\c`
    pub fn s_pi1cs13(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(169 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_S_PI1CS13 */, &a)?;
        Ok(())
    }

    /// GPIO (27). IO direction for GPIO 27 (out/in). Wire: `h\s\o\l`
    pub fn g_pio27(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(170 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_G_PIO27 */, &a)?;
        Ok(())
    }

    /// UART1 Rx (9). IO direction for UART1 Rx pin 9 (out/in). Wire: `h\s\o\e`
    pub fn u_art1_rx9(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(171 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_U_ART1_RX9 */, &a)?;
        Ok(())
    }

    /// UART1 CTS (10). IO direction for UART1 CTS pin 10 (out/in). Wire: `h\s\o\f`
    pub fn u_art1cts10(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(172 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_U_ART1CTS10 */, &a)?;
        Ok(())
    }

    /// UART1 Tx (8). IO direction for UART1 Tx pin 8 (out/in). Wire: `h\s\o\g`
    pub fn u_art1_tx8(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(173 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_U_ART1_TX8 */, &a)?;
        Ok(())
    }

    /// UART1 RTS (11). IO direction for UART1 RTS pin 11 (out/in). Wire: `h\s\o\m`
    pub fn u_art1rts11(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(174 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_U_ART1RTS11 */, &a)?;
        Ok(())
    }

    /// SPI1 Tx (15). IO direction for SPI1 Tx pin 15 (out/in). Wire: `h\s\o\i`
    pub fn s_pi1_tx15(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(175 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_S_PI1_TX15 */, &a)?;
        Ok(())
    }

    /// SPI1 SCLK (14). IO direction for SPI1 SCLK pin 14 (out/in). Wire: `h\s\o\j`
    pub fn s_pi1sclk14(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(176 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_S_PI1SCLK14 */, &a)?;
        Ok(())
    }

    /// GPIO25 (25). IO direction for GPIO 25 (out/in). Wire: `h\s\o\k`
    pub fn g_pio2525(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(177 /* CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_G_PIO2525 */, &a)?;
        Ok(())
    }
}
