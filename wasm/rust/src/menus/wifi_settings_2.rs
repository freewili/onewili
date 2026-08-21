//! Wifi Settings menu - generated from fwMenuWifiSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct WifiSettings2<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> WifiSettings2<'a> {
    /// Enable Station Mode. Connect the device to an existing Wi-Fi network in station mode, or disconnect from it. Wire: `w\w\e\s`
    pub fn enable_station_mode(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(336 /* CMD_WIRELESS_WIFI_SETTINGS_ENABLE_STATION_MODE */, &a)?;
        Ok(())
    }

    /// SSID for Station Mode. Set the name (SSID) of the Wi-Fi network to join in station mode. Wire: `w\w\e\e`
    pub fn s_sid_for_station_mode(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(337 /* CMD_WIRELESS_WIFI_SETTINGS_S_SID_FOR_STATION_MODE */, &a)?;
        Ok(())
    }

    /// Password for Station Mode. Set the password used to join the Wi-Fi network in station mode. Wire: `w\w\e\p`
    pub fn password_for_station_mode(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(338 /* CMD_WIRELESS_WIFI_SETTINGS_PASSWORD_FOR_STATION_MODE */, &a)?;
        Ok(())
    }

    /// Enable AP Mode. Turn the device's own Wi-Fi access point on or off. Wire: `w\w\e\a`
    pub fn enable_ap_mode(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(339 /* CMD_WIRELESS_WIFI_SETTINGS_ENABLE_AP_MODE */, &a)?;
        Ok(())
    }

    /// AP Auth. Choose the Wi-Fi security type used by the device's own access point. Wire: `w\w\e\u`
    pub fn a_p_auth(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(340 /* CMD_WIRELESS_WIFI_SETTINGS_A_P_AUTH */, &a)?;
        Ok(())
    }

    /// AP hide SSID. Hide the access point's network name (SSID) so it isn't broadcast to nearby devices. Wire: `w\w\e\i`
    pub fn a_p_hide_ssid(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(341 /* CMD_WIRELESS_WIFI_SETTINGS_A_P_HIDE_SSID */, &a)?;
        Ok(())
    }

    /// SSID for AP. Set the network name (SSID) broadcast by the device's own access point. Wire: `w\w\e\g`
    pub fn s_sid_for_ap(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(342 /* CMD_WIRELESS_WIFI_SETTINGS_S_SID_FOR_AP */, &a)?;
        Ok(())
    }

    /// Password for AP. Set the password required to join the device's own access point. Wire: `w\w\e\x`
    pub fn password_for_ap(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(343 /* CMD_WIRELESS_WIFI_SETTINGS_PASSWORD_FOR_AP */, &a)?;
        Ok(())
    }
}
