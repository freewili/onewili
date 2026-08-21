//! Bluetooth Settings menu - generated from fwMenuBLESettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct BleSettings2<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> BleSettings2<'a> {
    /// Enable BT. Turn Bluetooth LE on or off. Wire: `w\b\b\s`
    pub fn enable_bt(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\b\\b\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// BT <-> Terminal. Shown in Bluetooth LE status, but not currently used: the firmware always follows the Enable BT setting instead. Wire: `w\b\b\t`
    pub fn b_t_terminal(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\b\\b\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// BT Advert Name. Set the name the device advertises over Bluetooth LE. Wire: `w\b\b\a`
    pub fn b_t_advert_name(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\b\\b\\a");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }
}
