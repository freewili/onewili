//! Neptune Settings menu - generated from fwMenuNeptuneSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct NeptuneSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> NeptuneSettings<'a> {
    /// CAN1 Mode. CAN Type or UART over CAN PHY. Wire: `h\s\p\a`
    pub fn c_an1_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\a");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN1 Rate. Baudrate of CAN or UART over CAN PHY. Wire: `h\s\p\b`
    pub fn c_an1_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN1 FD D Rate. Baud Rate for CANFD Data section. Wire: `h\s\p\c`
    pub fn c_an1fdd_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\c");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN1 Listen Only. Enables Listen Only mode. Wire: `h\s\p\y`
    pub fn c_an1_listen_only(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\p\\y");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN1 Tx Retry. CAN Transmit retry options. Wire: `h\s\p\e`
    pub fn c_an1_tx_retry(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\e");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN1 Cust Baud. Hex Value String for Register C1NBTCFG. Blank to disable.. Wire: `h\s\p\f`
    pub fn c_an1_cust_baud(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\f");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN1 Cust Data Baud. Hex Value String for Register C1DBTCFG. Wire: `h\s\p\g`
    pub fn c_an1_cust_data_baud(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\g");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN1 Termination. Enables termination for network.. Wire: `h\s\p\1`
    pub fn c_an1_termination(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\p\\1");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN1 API Enabled. Set Wili API Base ID. Wire: `h\s\p\i`
    pub fn c_an1api_enabled(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\p\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN API ID. Enables Terminal over CANFD. Wire: `h\s\p\j`
    pub fn c_anapiid(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\j");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 Mode. CAN Type or UART over CAN PHY. Wire: `h\s\p\k`
    pub fn c_an2_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\k");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 Rate. Baudrate of CAN or UART over CAN PHY. Wire: `h\s\p\l`
    pub fn c_an2_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\l");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 FD D Rate. Baud Rate for CANFD Data section. Wire: `h\s\p\m`
    pub fn c_an2fdd_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 Listen Only. Enables Listen Only mode. Wire: `h\s\p\n`
    pub fn c_an2_listen_only(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\p\\n");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 Tx Retry. CAN Transmit retry options. Wire: `h\s\p\o`
    pub fn c_an2_tx_retry(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\o");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 Cust Baud. Hex Value String for Register C1NBTCFG. Blank to disable.. Wire: `h\s\p\p`
    pub fn c_an2_cust_baud(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\p");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 Cust Data Baud. Hex Value String for Register C1DBTCFG. Wire: `h\s\p\r`
    pub fn c_an2_cust_data_baud(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\r");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 Termination. Enables termination for network.. Wire: `h\s\p\s`
    pub fn c_an2_termination(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\p\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CAN2 API Enabled. Enables Wili API over CANFD. Wire: `h\s\p\t`
    pub fn c_an2api_enabled(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\p\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// LIN Master En. Enables LIN Master Pull Resistor. Wire: `h\s\p\u`
    pub fn l_in_master_en(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\p\\u");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// LIN Baud Rate. Baud Rate for LIN. Wire: `h\s\p\v`
    pub fn l_in_baud_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\p\\v");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Analog In En. Enables analog input measurement. Wire: `h\s\p\x`
    pub fn analog_in_en(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\p\\x");
        self.t.call(&cmd)?;
        Ok(())
    }
}
