//! BT Functions menu - generated from fwMenuBluetoothLE. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct BluetoothLe<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> BluetoothLe<'a> {
    /// Bluetooth Settings sub-menu.
    pub fn ble_settings(self) -> super::ble_settings_2::BleSettings2<'a> {
        super::ble_settings_2::BleSettings2 { t: self.t }
    }

    /// Start BT Advertising. Sets the Host Name for the Bluetooth LE. Wire: `w\b\a`
    pub fn on_start_bt_advertising(&mut self, hostname: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\b\\a");
        encoding::push_str(&mut cmd, hostname);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop BT Advertising. Stops BT Advertising. Wire: `w\b\t`
    pub fn on_stop_bt_advertising(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\b\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Scan for BT Devices. Scans for BT devices for a given duration. Wire: `w\b\s`
    pub fn on_scan_bt_devices(&mut self, durationms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\b\\s");
        encoding::push_int(&mut cmd, durationms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Toggle Enable Terminal API Mode. Enables BLE to FreeWili Terminal API Mode. Wire: `w\b\e`
    pub fn on_enable_terminal(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\b\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Attribute. Sets a user attribute slot a connected phone can read and subscribe to. Wire: `w\b\v`
    pub fn on_set_attribute(&mut self, slot: i32, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\b\\v");
        encoding::push_int(&mut cmd, slot as i64);
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Attribute. Reads a user attribute slot back from the radio. Wire: `w\b\u`
    pub fn on_get_attribute(&mut self, slot: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\b\\u");
        encoding::push_int(&mut cmd, slot as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("btscan", false, "name=string,mac=string,rssi=decS32", "BLE scan result (device name, MAC address, RSSI)"),
    ("attrvalue", false, "slot=decU32,value=string", "User attribute slot value read back from the radio"),
    ("attrwritten", false, "slot=decU32,value=string", "A connected peer wrote a user attribute slot"),
];
