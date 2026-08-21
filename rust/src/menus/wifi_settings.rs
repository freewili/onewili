//! Wifi Settings menu - generated from fwMenuWifiSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct WifiSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> WifiSettings<'a> {
    /// Enable Station Mode. Connect the device to an existing Wi-Fi network in station mode, or disconnect from it. Wire: `h\s\w\s`
    pub fn enable_station_mode(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\w\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SSID for Station Mode. Set the name (SSID) of the Wi-Fi network to join in station mode. Wire: `h\s\w\e`
    pub fn s_sid_for_station_mode(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\w\\e");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Password for Station Mode. Set the password used to join the Wi-Fi network in station mode. Wire: `h\s\w\p`
    pub fn password_for_station_mode(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\w\\p");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Enable AP Mode. Turn the device's own Wi-Fi access point on or off. Wire: `h\s\w\a`
    pub fn enable_ap_mode(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\w\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// AP Auth. Choose the Wi-Fi security type used by the device's own access point. Wire: `h\s\w\u`
    pub fn a_p_auth(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\w\\u");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// AP hide SSID. Hide the access point's network name (SSID) so it isn't broadcast to nearby devices. Wire: `h\s\w\i`
    pub fn a_p_hide_ssid(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\w\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SSID for AP. Set the network name (SSID) broadcast by the device's own access point. Wire: `h\s\w\g`
    pub fn s_sid_for_ap(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\w\\g");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Password for AP. Set the password required to join the device's own access point. Wire: `h\s\w\x`
    pub fn password_for_ap(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\w\\x");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }
}
