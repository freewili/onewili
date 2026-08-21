//! Websocket Server menu - generated from fwMenuWebsocketSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct WebsocketSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> WebsocketSettings<'a> {
    /// Start WS Server. Turn the websocket server on or off. Wire: `h\s\k\r`
    pub fn start_ws_server(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\k\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WS Server Port. Set the TCP port the websocket server listens on. Wire: `h\s\k\p`
    pub fn w_s_server_port(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\k\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Auth Mode. Choose whether the websocket server allows open access or requires a username and password. Wire: `h\s\k\m`
    pub fn auth_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\k\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Auth Username. Set the username required to connect to the websocket server when basic authentication is enabled. Wire: `h\s\k\u`
    pub fn auth_username(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\k\\u");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Auth Password. Set the password required to connect to the websocket server when basic authentication is enabled. Wire: `h\s\k\e`
    pub fn auth_password(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\k\\e");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }
}
