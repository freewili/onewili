//! IO Directions menu - generated from fwMenuIODirectionSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct IoDirectionSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> IoDirectionSettings<'a> {
    /// SPI1 Rx (12). IO direction for SPI1 Rx pin 12 (out/in). Wire: `i\g\a\a`
    pub fn s_pi1_rx12(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(7 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_S_PI1_RX12 */, &a)?;
        Ok(())
    }

    /// GPIO 26 (26). IO direction for GPIO 26 (out/in). Wire: `i\g\a\b`
    pub fn g_pio2626(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(8 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_G_PIO2626 */, &a)?;
        Ok(())
    }

    /// SPI1 CS (13). IO direction for SPI1 CS pin 13 (out/in). Wire: `i\g\a\c`
    pub fn s_pi1cs13(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(9 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_S_PI1CS13 */, &a)?;
        Ok(())
    }

    /// GPIO (27). IO direction for GPIO 27 (out/in). Wire: `i\g\a\l`
    pub fn g_pio27(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(10 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_G_PIO27 */, &a)?;
        Ok(())
    }

    /// UART1 Rx (9). IO direction for UART1 Rx pin 9 (out/in). Wire: `i\g\a\e`
    pub fn u_art1_rx9(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(11 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_U_ART1_RX9 */, &a)?;
        Ok(())
    }

    /// UART1 CTS (10). IO direction for UART1 CTS pin 10 (out/in). Wire: `i\g\a\f`
    pub fn u_art1cts10(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(12 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_U_ART1CTS10 */, &a)?;
        Ok(())
    }

    /// UART1 Tx (8). IO direction for UART1 Tx pin 8 (out/in). Wire: `i\g\a\g`
    pub fn u_art1_tx8(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(13 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_U_ART1_TX8 */, &a)?;
        Ok(())
    }

    /// UART1 RTS (11). IO direction for UART1 RTS pin 11 (out/in). Wire: `i\g\a\m`
    pub fn u_art1rts11(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(14 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_U_ART1RTS11 */, &a)?;
        Ok(())
    }

    /// SPI1 Tx (15). IO direction for SPI1 Tx pin 15 (out/in). Wire: `i\g\a\i`
    pub fn s_pi1_tx15(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(15 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_S_PI1_TX15 */, &a)?;
        Ok(())
    }

    /// SPI1 SCLK (14). IO direction for SPI1 SCLK pin 14 (out/in). Wire: `i\g\a\j`
    pub fn s_pi1sclk14(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(16 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_S_PI1SCLK14 */, &a)?;
        Ok(())
    }

    /// GPIO25 (25). IO direction for GPIO 25 (out/in). Wire: `i\g\a\k`
    pub fn g_pio2525(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(17 /* CMD_IO_GPIO_IO_DIRECTION_SETTINGS_G_PIO2525 */, &a)?;
        Ok(())
    }
}
