//! Linux Functions menu - generated from fwMenuLinux. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Linux<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Linux<'a> {
    /// Enable Linux CPU. Not yet implemented; always reports failure. Wire: `l\a`
    pub fn enable_linux_cpu(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(382 /* CMD_LINUX_ENABLE_LINUX_CPU */, &a)?;
        Ok(())
    }

    /// Open Shell. Wire: `l\b`
    pub fn open_shell(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(398 /* CMD_LINUX_OPEN_SHELL */, &a)?;
        Ok(())
    }

    /// Open Shell Session. Open a framed Linux shell without entering menu passthrough. Wire: `l\c`
    pub fn open_shell_session(&mut self, session: u32) -> Result<u32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        let mut _r = crate::transport::call(603 /* CMD_LINUX_OPEN_SHELL_SESSION */, &a)?;
        Ok(_r.u32())
    }

    /// Close Shell Session. Release the framed Linux shell and terminate its session. Wire: `l\e`
    pub fn close_shell_session(&mut self, session: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        let _r = crate::transport::call(604 /* CMD_LINUX_CLOSE_SHELL_SESSION */, &a)?;
        Ok(())
    }

    /// Write Shell Session. Queue up to 192 shell bytes and return the accepted byte count. Wire: `l\w`
    pub fn write_shell_session(&mut self, session: u32, data: &str) -> Result<i32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        a.str(data);
        let mut _r = crate::transport::call(606 /* CMD_LINUX_WRITE_SHELL_SESSION */, &a)?;
        Ok(_r.i32())
    }

    /// Read Shell Session. Read bounded shell output as hexadecimal inside a normal menu response. Wire: `l\r`
    pub fn read_shell_session(&mut self, session: u32, maximum: i32) -> Result<(i32, String, bool), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        a.i32(maximum);
        let mut _r = crate::transport::call(605 /* CMD_LINUX_READ_SHELL_SESSION */, &a)?;
        Ok((_r.i32(), _r.string(), _r.u8() != 0))
    }

    /// CM0 USB Mode. Query or switch CM0 USB between PC gadget and USB-A Port 3 host; requires CM0 image support. Wire: `l\u`
    pub fn cm0_usb_mode(&mut self, mode: &str) -> Result<(String, bool), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(mode);
        let mut _r = crate::transport::call(613 /* CMD_LINUX_CM0_USB_MODE */, &a)?;
        Ok((_r.string(), _r.u8() != 0))
    }
}
