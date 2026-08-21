//! Websocket Server menu - generated from fwMenuWebsocketSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct WebsocketSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> WebsocketSettings<'a> {
    /// Start WS Server. Turn the websocket server on or off. Wire: `h\s\k\r`
    pub fn start_ws_server(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(251 /* CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_START_WS_SERVER */, &a)?;
        Ok(())
    }

    /// WS Server Port. Set the TCP port the websocket server listens on. Wire: `h\s\k\p`
    pub fn w_s_server_port(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(252 /* CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_W_S_SERVER_PORT */, &a)?;
        Ok(())
    }

    /// Auth Mode. Choose whether the websocket server allows open access or requires a username and password. Wire: `h\s\k\m`
    pub fn auth_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(253 /* CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_AUTH_MODE */, &a)?;
        Ok(())
    }

    /// Auth Username. Set the username required to connect to the websocket server when basic authentication is enabled. Wire: `h\s\k\u`
    pub fn auth_username(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(254 /* CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_AUTH_USERNAME */, &a)?;
        Ok(())
    }

    /// Auth Password. Set the password required to connect to the websocket server when basic authentication is enabled. Wire: `h\s\k\e`
    pub fn auth_password(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(255 /* CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_AUTH_PASSWORD */, &a)?;
        Ok(())
    }
}
