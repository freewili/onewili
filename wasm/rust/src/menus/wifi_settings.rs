//! Wifi Settings menu - generated from fwMenuWifiSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct WifiSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> WifiSettings<'a> {
    /// Enable Station Mode. Connect the device to an existing Wi-Fi network in station mode, or disconnect from it. Wire: `h\s\w\s`
    pub fn enable_station_mode(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(239 /* CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_ENABLE_STATION_MODE */, &a)?;
        Ok(())
    }

    /// SSID for Station Mode. Set the name (SSID) of the Wi-Fi network to join in station mode. Wire: `h\s\w\e`
    pub fn s_sid_for_station_mode(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(240 /* CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_S_SID_FOR_STATION_MODE */, &a)?;
        Ok(())
    }

    /// Password for Station Mode. Set the password used to join the Wi-Fi network in station mode. Wire: `h\s\w\p`
    pub fn password_for_station_mode(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(241 /* CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_PASSWORD_FOR_STATION_MODE */, &a)?;
        Ok(())
    }

    /// Enable AP Mode. Turn the device's own Wi-Fi access point on or off. Wire: `h\s\w\a`
    pub fn enable_ap_mode(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(242 /* CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_ENABLE_AP_MODE */, &a)?;
        Ok(())
    }

    /// AP Auth. Choose the Wi-Fi security type used by the device's own access point. Wire: `h\s\w\u`
    pub fn a_p_auth(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(243 /* CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_A_P_AUTH */, &a)?;
        Ok(())
    }

    /// AP hide SSID. Hide the access point's network name (SSID) so it isn't broadcast to nearby devices. Wire: `h\s\w\i`
    pub fn a_p_hide_ssid(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(244 /* CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_A_P_HIDE_SSID */, &a)?;
        Ok(())
    }

    /// SSID for AP. Set the network name (SSID) broadcast by the device's own access point. Wire: `h\s\w\g`
    pub fn s_sid_for_ap(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(245 /* CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_S_SID_FOR_AP */, &a)?;
        Ok(())
    }

    /// Password for AP. Set the password required to join the device's own access point. Wire: `h\s\w\x`
    pub fn password_for_ap(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(246 /* CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_PASSWORD_FOR_AP */, &a)?;
        Ok(())
    }
}
