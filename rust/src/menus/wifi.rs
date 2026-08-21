//! Wifi Functions menu - generated from fwMenuWifi. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Wifi<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Wifi<'a> {
    /// Wifi Settings sub-menu.
    pub fn settings(self) -> super::wifi_settings_2::WifiSettings2<'a> {
        super::wifi_settings_2::WifiSettings2 { t: self.t }
    }

    /// Enable Wifi Events. Toggle Wifi Event Streaming. Wire: `w\w\r`
    pub fn toggle_events(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Start Access Point. Starts up Access Point with provided SSID and Password. Wire: `w\w\a`
    pub fn on_start_access_point(&mut self, ssid: &str, password: &str, authmode: i32, hidessid: bool) -> Result<(), OwError> {
        let mut cmd = String::from("w\\w\\a");
        encoding::push_str(&mut cmd, ssid);
        encoding::push_str(&mut cmd, password);
        encoding::push_int(&mut cmd, authmode as i64);
        encoding::push_bool(&mut cmd, hidessid);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop Access Point. Turns off Access Point. Wire: `w\w\t`
    pub fn on_discconect_from_station(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Stations connected to AP. Turns off Access Point. Wire: `w\w\g`
    pub fn get_connected_devices(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\g");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Connect to a Wifi Access Point. Connect to a WAP with provided SSID and Password. Wire: `w\w\c`
    pub fn on_connect_to_station(&mut self, ssid: &str, password: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\w\\c");
        encoding::push_str(&mut cmd, ssid);
        encoding::push_str(&mut cmd, password);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Disconnect From Wifi Access Point. Disconnect from Wifi Stations. Wire: `w\w\f`
    pub fn on_discconect_from_station_2(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\f");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Scan for Access Points. Scans for available WIFI networks. Wire: `w\w\s`
    pub fn on_scan_for_access_points(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Print out Wifi Info. Scans for available Wifi networks. Wire: `w\w\p`
    pub fn on_get_wif_info(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\p");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Download To SDCard. HTTP GET a URL and write it to a file on the SD card. Wire: `w\w\l`
    pub fn on_http_get_to_sd(&mut self, url: &str, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\w\\l");
        encoding::push_str(&mut cmd, url);
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Cancel Download. Stops a download started with Download To SDCard. Wire: `w\w\x`
    pub fn on_http_get_abort(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\w\\x");
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("wifistaInfo", false, "ip=string,gateway=string,mask=string", "Station IP configuration (got IP)"),
    ("wifiapInfo", false, "ip=string,gateway=string,mask=string", "Access point IP configuration"),
    ("wifiscan", false, "bssid=string,rssi=decS32,channel=decU32,band=decU32,authmode=decU32,ssid=string", "Wifi scan record (SSID last so consumers can bounded-split)"),
    ("wifiapdevcon", false, "ip=string,mac=string", "Device connected to the access point"),
    ("wifiapdevdc", false, "mac=string", "Device disconnected from the access point"),
    ("wsclientcon", false, "ip=string", "Websocket client connected"),
    ("wsclientdc", false, "ip=string", "Websocket client disconnected"),
    ("wifistations", false, "ip=string,mac=string", "Connected access-point station record (one event per device)"),
    ("httpget", false, "written=decU32,total=decU32", "Download progress; total is 0 when the server sent no length"),
    ("httpgetdone", false, "result=decU32,status=decU32,written=decU32", "Download finished; result 0 is success, otherwise a bnose_http_result_t"),
];
