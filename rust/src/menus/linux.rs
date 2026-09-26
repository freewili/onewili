//! Linux Functions menu - generated from fwMenuLinux. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Linux<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Linux<'a> {
    /// Enable Linux CPU. Not yet implemented; always reports failure. Wire: `l\a`
    pub fn enable_linux_cpu(&mut self) -> Result<(), OwError> {
        let cmd = String::from("l\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Open Shell. Wire: `l\b`
    pub fn open_shell(&mut self) -> Result<(), OwError> {
        let cmd = String::from("l\\b");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Open Shell Session. Open a framed Linux shell without entering menu passthrough. Wire: `l\c`
    pub fn open_shell_session(&mut self, session: u32) -> Result<u32, OwError> {
        let mut cmd = String::from("l\\c");
        encoding::push_hex(&mut cmd, session as u64, 8);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let session = encoding::tok_hex(&mut toks)? as u32;
        Ok(session)
    }

    /// Close Shell Session. Release the framed Linux shell and terminate its session. Wire: `l\e`
    pub fn close_shell_session(&mut self, session: u32) -> Result<(), OwError> {
        let mut cmd = String::from("l\\e");
        encoding::push_hex(&mut cmd, session as u64, 8);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Write Shell Session. Queue up to 192 shell bytes and return the accepted byte count. Wire: `l\w`
    pub fn write_shell_session(&mut self, session: u32, data: &str) -> Result<i32, OwError> {
        let mut cmd = String::from("l\\w");
        encoding::push_hex(&mut cmd, session as u64, 8);
        encoding::push_str(&mut cmd, data);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let accepted = encoding::tok_int(&mut toks)? as i32;
        Ok(accepted)
    }

    /// Read Shell Session. Read bounded shell output as hexadecimal inside a normal menu response. Wire: `l\r`
    pub fn read_shell_session(&mut self, session: u32, maximum: i32) -> Result<(i32, String, bool), OwError> {
        let mut cmd = String::from("l\\r");
        encoding::push_hex(&mut cmd, session as u64, 8);
        encoding::push_int(&mut cmd, maximum as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let count = encoding::tok_int(&mut toks)? as i32;
        let data = encoding::rest_str(&mut toks);
        let running = encoding::tok_bool(&mut toks)?;
        Ok((count, data, running))
    }

    /// CM0 USB Mode. Query or switch CM0 USB between PC gadget and USB-A Port 3 host; requires CM0 image support. Wire: `l\u`
    pub fn cm0_usb_mode(&mut self, mode: &str) -> Result<(String, bool), OwError> {
        let mut cmd = String::from("l\\u");
        encoding::push_str(&mut cmd, mode);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let mode = encoding::rest_str(&mut toks);
        let switchable = encoding::tok_bool(&mut toks)?;
        Ok((mode, switchable))
    }
}
