//! Wifi Settings menu - generated from fwMenuWifiSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct WifiSettings2<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> WifiSettings2<'a> {
    /// Enable Station Mode. Connect the device to an existing Wi-Fi network in station mode, or disconnect from it. Wire: `w\w\e\s`
    pub fn enable_station_mode(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\e\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SSID for Station Mode. Set the name (SSID) of the Wi-Fi network to join in station mode. Wire: `w\w\e\e`
    pub fn s_sid_for_station_mode(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\w\\e\\e");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Password for Station Mode. Set the password used to join the Wi-Fi network in station mode. Wire: `w\w\e\p`
    pub fn password_for_station_mode(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\w\\e\\p");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Enable AP Mode. Turn the device's own Wi-Fi access point on or off. Wire: `w\w\e\a`
    pub fn enable_ap_mode(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\e\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// AP Auth. Choose the Wi-Fi security type used by the device's own access point. Wire: `w\w\e\u`
    pub fn a_p_auth(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\w\\e\\u");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// AP hide SSID. Hide the access point's network name (SSID) so it isn't broadcast to nearby devices. Wire: `w\w\e\i`
    pub fn a_p_hide_ssid(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\e\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SSID for AP. Set the network name (SSID) broadcast by the device's own access point. Wire: `w\w\e\g`
    pub fn s_sid_for_ap(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\w\\e\\g");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Password for AP. Set the password required to join the device's own access point. Wire: `w\w\e\x`
    pub fn password_for_ap(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\w\\e\\x");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }
}
