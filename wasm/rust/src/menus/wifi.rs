//! Wifi Functions menu - generated from fwMenuWifi. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Wifi<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Wifi<'a> {
    /// Wifi Settings sub-menu.
    pub fn settings(self) -> super::wifi_settings_2::WifiSettings2<'a> {
        super::wifi_settings_2::WifiSettings2 { t: self.t }
    }

    /// Enable Wifi Events. Toggle Wifi Event Streaming. Wire: `w\w\r`
    pub fn toggle_events(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(328 /* CMD_WIRELESS_WIFI_TOGGLE_EVENTS */, &a)?;
        Ok(())
    }

    /// Start Access Point. Starts up Access Point with provided SSID and Password. Wire: `w\w\a`
    pub fn on_start_access_point(&mut self, ssid: &str, password: &str, authmode: i32, hidessid: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(ssid);
        a.str(password);
        a.i32(authmode);
        a.u8(if hidessid { 1 } else { 0 });
        let _r = crate::transport::call(329 /* CMD_WIRELESS_WIFI_ON_START_ACCESS_POINT */, &a)?;
        Ok(())
    }

    /// Stop Access Point. Turns off Access Point. Wire: `w\w\t`
    pub fn on_discconect_from_station(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(330 /* CMD_WIRELESS_WIFI_ON_DISCCONECT_FROM_STATION */, &a)?;
        Ok(())
    }

    /// Get Stations connected to AP. Turns off Access Point. Wire: `w\w\g`
    pub fn get_connected_devices(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(331 /* CMD_WIRELESS_WIFI_GET_CONNECTED_DEVICES */, &a)?;
        Ok(())
    }

    /// Connect to a Wifi Access Point. Connect to a WAP with provided SSID and Password. Wire: `w\w\c`
    pub fn on_connect_to_station(&mut self, ssid: &str, password: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(ssid);
        a.str(password);
        let _r = crate::transport::call(332 /* CMD_WIRELESS_WIFI_ON_CONNECT_TO_STATION */, &a)?;
        Ok(())
    }

    /// Disconnect From Wifi Access Point. Disconnect from Wifi Stations. Wire: `w\w\f`
    pub fn on_discconect_from_station_2(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(333 /* CMD_WIRELESS_WIFI_ON_DISCCONECT_FROM_STATION_2 */, &a)?;
        Ok(())
    }

    /// Scan for Access Points. Scans for available WIFI networks. Wire: `w\w\s`
    pub fn on_scan_for_access_points(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(334 /* CMD_WIRELESS_WIFI_ON_SCAN_FOR_ACCESS_POINTS */, &a)?;
        Ok(())
    }

    /// Print out Wifi Info. Scans for available Wifi networks. Wire: `w\w\p`
    pub fn on_get_wif_info(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(335 /* CMD_WIRELESS_WIFI_ON_GET_WIF_INFO */, &a)?;
        Ok(())
    }

    /// Download To SDCard. HTTP GET a URL and write it to a file on the SD card. Wire: `w\w\l`
    pub fn on_http_get_to_sd(&mut self, url: &str, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(url);
        a.str(path);
        let _r = crate::transport::call(506 /* CMD_WIRELESS_WIFI_ON_HTTP_GET_TO_SD */, &a)?;
        Ok(())
    }

    /// Cancel Download. Stops a download started with Download To SDCard. Wire: `w\w\x`
    pub fn on_http_get_abort(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(507 /* CMD_WIRELESS_WIFI_ON_HTTP_GET_ABORT */, &a)?;
        Ok(())
    }
}
