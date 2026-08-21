//! Bluetooth Settings menu - generated from fwMenuBLESettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct BleSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> BleSettings<'a> {
    /// Enable BT. Turn Bluetooth LE on or off. Wire: `h\s\b\s`
    pub fn enable_bt(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(247 /* CMD_HARDWARE_SETTINGS_HOME_BLE_SETTINGS_ENABLE_BT */, &a)?;
        Ok(())
    }

    /// BT <-> Terminal. Shown in Bluetooth LE status, but not currently used: the firmware always follows the Enable BT setting instead. Wire: `h\s\b\t`
    pub fn b_t_terminal(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(248 /* CMD_HARDWARE_SETTINGS_HOME_BLE_SETTINGS_B_T_TERMINAL */, &a)?;
        Ok(())
    }

    /// BT Advert Name. Set the name the device advertises over Bluetooth LE. Wire: `h\s\b\a`
    pub fn b_t_advert_name(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(249 /* CMD_HARDWARE_SETTINGS_HOME_BLE_SETTINGS_B_T_ADVERT_NAME */, &a)?;
        Ok(())
    }
}
