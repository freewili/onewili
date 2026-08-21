//! IO Directions menu - generated from fwMenuIODirectionSettings. Do not edit.

use crate::transport::{OwError, Transport};

pub struct IoDirectionSettings2<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> IoDirectionSettings2<'a> {
    /// SPI1 Rx (12). IO direction for SPI1 Rx pin 12 (out/in). Wire: `h\s\o\a`
    pub fn s_pi1_rx12(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// GPIO 26 (26). IO direction for GPIO 26 (out/in). Wire: `h\s\o\b`
    pub fn g_pio2626(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\b");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SPI1 CS (13). IO direction for SPI1 CS pin 13 (out/in). Wire: `h\s\o\c`
    pub fn s_pi1cs13(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\c");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// GPIO (27). IO direction for GPIO 27 (out/in). Wire: `h\s\o\l`
    pub fn g_pio27(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\l");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// UART1 Rx (9). IO direction for UART1 Rx pin 9 (out/in). Wire: `h\s\o\e`
    pub fn u_art1_rx9(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// UART1 CTS (10). IO direction for UART1 CTS pin 10 (out/in). Wire: `h\s\o\f`
    pub fn u_art1cts10(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\f");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// UART1 Tx (8). IO direction for UART1 Tx pin 8 (out/in). Wire: `h\s\o\g`
    pub fn u_art1_tx8(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\g");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// UART1 RTS (11). IO direction for UART1 RTS pin 11 (out/in). Wire: `h\s\o\m`
    pub fn u_art1rts11(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\m");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SPI1 Tx (15). IO direction for SPI1 Tx pin 15 (out/in). Wire: `h\s\o\i`
    pub fn s_pi1_tx15(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SPI1 SCLK (14). IO direction for SPI1 SCLK pin 14 (out/in). Wire: `h\s\o\j`
    pub fn s_pi1sclk14(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\j");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// GPIO25 (25). IO direction for GPIO 25 (out/in). Wire: `h\s\o\k`
    pub fn g_pio2525(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\o\\k");
        self.t.call(&cmd)?;
        Ok(())
    }
}
