//! Scripting Functions menu - generated from fwMenuScripting. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Scripting<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Scripting<'a> {
    /// App Signals sub-menu.
    pub fn app_signals(self) -> super::app_signals::AppSignals<'a> {
        super::app_signals::AppSignals { t: self.t }
    }

    /// Wili Files sub-menu.
    pub fn wili_files(self) -> super::wili_files::WiliFiles<'a> {
        super::wili_files::WiliFiles { t: self.t }
    }

    /// ZoomIO Functions sub-menu.
    pub fn zoom_io(self) -> super::zoom_io::ZoomIo<'a> {
        super::zoom_io::ZoomIo { t: self.t }
    }

    /// WASM Debug sub-menu.
    pub fn wasm_debug(self) -> super::wasm_debug::WasmDebug<'a> {
        super::wasm_debug::WasmDebug { t: self.t }
    }

    /// rThon Debug sub-menu.
    pub fn rthon_debug(self) -> super::rthon_debug::RthonDebug<'a> {
        super::rthon_debug::RthonDebug { t: self.t }
    }

    /// Launch Script. Not yet implemented; always reports failure. Wire: `s\a`
    pub fn launch_script(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(358 /* CMD_SCRIPTING_LAUNCH_SCRIPT */, &a)?;
        Ok(())
    }

    /// Power Cycle Debugger. Powers debugger zone 16 off for 500 ms, then powers it back on.. Wire: `s\c`
    pub fn power_cycle_debugger(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(475 /* CMD_SCRIPTING_POWER_CYCLE_DEBUGGER */, &a)?;
        Ok(())
    }
}
