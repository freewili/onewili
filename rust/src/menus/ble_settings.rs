//! Bluetooth Settings menu - generated from fwMenuBLESettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct BleSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> BleSettings<'a> {
    /// Enable BT. Turn Bluetooth LE on or off. Wire: `h\s\b\s`
    pub fn enable_bt(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\b\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// BT <-> Terminal. Shown in Bluetooth LE status, but not currently used: the firmware always follows the Enable BT setting instead. Wire: `h\s\b\t`
    pub fn b_t_terminal(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\b\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// BT Advert Name. Set the name the device advertises over Bluetooth LE. Wire: `h\s\b\a`
    pub fn b_t_advert_name(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\b\\a");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }
}
