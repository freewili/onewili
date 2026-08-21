//! IO Directions menu - generated from fwMenuIODirectionSettings. Do not edit.

use crate::transport::{OwError, Transport};

pub struct IoDirectionSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> IoDirectionSettings<'a> {
    /// SPI1 Rx (12). IO direction for SPI1 Rx pin 12 (out/in). Wire: `i\g\a\a`
    pub fn s_pi1_rx12(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// GPIO 26 (26). IO direction for GPIO 26 (out/in). Wire: `i\g\a\b`
    pub fn g_pio2626(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\b");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SPI1 CS (13). IO direction for SPI1 CS pin 13 (out/in). Wire: `i\g\a\c`
    pub fn s_pi1cs13(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\c");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// GPIO (27). IO direction for GPIO 27 (out/in). Wire: `i\g\a\l`
    pub fn g_pio27(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\l");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// UART1 Rx (9). IO direction for UART1 Rx pin 9 (out/in). Wire: `i\g\a\e`
    pub fn u_art1_rx9(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// UART1 CTS (10). IO direction for UART1 CTS pin 10 (out/in). Wire: `i\g\a\f`
    pub fn u_art1cts10(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\f");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// UART1 Tx (8). IO direction for UART1 Tx pin 8 (out/in). Wire: `i\g\a\g`
    pub fn u_art1_tx8(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\g");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// UART1 RTS (11). IO direction for UART1 RTS pin 11 (out/in). Wire: `i\g\a\m`
    pub fn u_art1rts11(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\m");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SPI1 Tx (15). IO direction for SPI1 Tx pin 15 (out/in). Wire: `i\g\a\i`
    pub fn s_pi1_tx15(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SPI1 SCLK (14). IO direction for SPI1 SCLK pin 14 (out/in). Wire: `i\g\a\j`
    pub fn s_pi1sclk14(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\j");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// GPIO25 (25). IO direction for GPIO 25 (out/in). Wire: `i\g\a\k`
    pub fn g_pio2525(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\g\\a\\k");
        self.t.call(&cmd)?;
        Ok(())
    }
}
