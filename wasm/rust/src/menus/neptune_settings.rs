//! Neptune Settings menu - generated from fwMenuNeptuneSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct NeptuneSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> NeptuneSettings<'a> {
    /// CAN1 Mode. CAN Type or UART over CAN PHY. Wire: `h\s\p\a`
    pub fn c_an1_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(256 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_MODE */, &a)?;
        Ok(())
    }

    /// CAN1 Rate. Baudrate of CAN or UART over CAN PHY. Wire: `h\s\p\b`
    pub fn c_an1_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(257 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_RATE */, &a)?;
        Ok(())
    }

    /// CAN1 FD D Rate. Baud Rate for CANFD Data section. Wire: `h\s\p\c`
    pub fn c_an1fdd_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(258 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1FDD_RATE */, &a)?;
        Ok(())
    }

    /// CAN1 Listen Only. Enables Listen Only mode. Wire: `h\s\p\y`
    pub fn c_an1_listen_only(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(259 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_LISTEN_ONLY */, &a)?;
        Ok(())
    }

    /// CAN1 Tx Retry. CAN Transmit retry options. Wire: `h\s\p\e`
    pub fn c_an1_tx_retry(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(260 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_TX_RETRY */, &a)?;
        Ok(())
    }

    /// CAN1 Cust Baud. Hex Value String for Register C1NBTCFG. Blank to disable.. Wire: `h\s\p\f`
    pub fn c_an1_cust_baud(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(261 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_CUST_BAUD */, &a)?;
        Ok(())
    }

    /// CAN1 Cust Data Baud. Hex Value String for Register C1DBTCFG. Wire: `h\s\p\g`
    pub fn c_an1_cust_data_baud(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(262 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_CUST_DATA_BAUD */, &a)?;
        Ok(())
    }

    /// CAN1 Termination. Enables termination for network.. Wire: `h\s\p\1`
    pub fn c_an1_termination(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(263 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_TERMINATION */, &a)?;
        Ok(())
    }

    /// CAN1 API Enabled. Set Wili API Base ID. Wire: `h\s\p\i`
    pub fn c_an1api_enabled(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(264 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1API_ENABLED */, &a)?;
        Ok(())
    }

    /// CAN API ID. Enables Terminal over CANFD. Wire: `h\s\p\j`
    pub fn c_anapiid(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(265 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_ANAPIID */, &a)?;
        Ok(())
    }

    /// CAN2 Mode. CAN Type or UART over CAN PHY. Wire: `h\s\p\k`
    pub fn c_an2_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(266 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_MODE */, &a)?;
        Ok(())
    }

    /// CAN2 Rate. Baudrate of CAN or UART over CAN PHY. Wire: `h\s\p\l`
    pub fn c_an2_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(267 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_RATE */, &a)?;
        Ok(())
    }

    /// CAN2 FD D Rate. Baud Rate for CANFD Data section. Wire: `h\s\p\m`
    pub fn c_an2fdd_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(268 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2FDD_RATE */, &a)?;
        Ok(())
    }

    /// CAN2 Listen Only. Enables Listen Only mode. Wire: `h\s\p\n`
    pub fn c_an2_listen_only(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(269 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_LISTEN_ONLY */, &a)?;
        Ok(())
    }

    /// CAN2 Tx Retry. CAN Transmit retry options. Wire: `h\s\p\o`
    pub fn c_an2_tx_retry(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(270 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_TX_RETRY */, &a)?;
        Ok(())
    }

    /// CAN2 Cust Baud. Hex Value String for Register C1NBTCFG. Blank to disable.. Wire: `h\s\p\p`
    pub fn c_an2_cust_baud(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(271 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_CUST_BAUD */, &a)?;
        Ok(())
    }

    /// CAN2 Cust Data Baud. Hex Value String for Register C1DBTCFG. Wire: `h\s\p\r`
    pub fn c_an2_cust_data_baud(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(272 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_CUST_DATA_BAUD */, &a)?;
        Ok(())
    }

    /// CAN2 Termination. Enables termination for network.. Wire: `h\s\p\s`
    pub fn c_an2_termination(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(273 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_TERMINATION */, &a)?;
        Ok(())
    }

    /// CAN2 API Enabled. Enables Wili API over CANFD. Wire: `h\s\p\t`
    pub fn c_an2api_enabled(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(274 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2API_ENABLED */, &a)?;
        Ok(())
    }

    /// LIN Master En. Enables LIN Master Pull Resistor. Wire: `h\s\p\u`
    pub fn l_in_master_en(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(275 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_L_IN_MASTER_EN */, &a)?;
        Ok(())
    }

    /// LIN Baud Rate. Baud Rate for LIN. Wire: `h\s\p\v`
    pub fn l_in_baud_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(276 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_L_IN_BAUD_RATE */, &a)?;
        Ok(())
    }

    /// Analog In En. Enables analog input measurement. Wire: `h\s\p\x`
    pub fn analog_in_en(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(277 /* CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_ANALOG_IN_EN */, &a)?;
        Ok(())
    }
}
