//! Scripting Functions menu - generated from fwMenuScripting. Do not edit.

use crate::transport::{OwError, Transport};

pub struct Scripting<'a> {
    pub(crate) t: &'a mut Transport,
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
        let cmd = String::from("s\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Power Cycle Debugger. Powers debugger zone 16 off for 500 ms, then powers it back on.. Wire: `s\c`
    pub fn power_cycle_debugger(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\c");
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("script", false, "data=string", "Script engine output / status line (wasm and rThon runners)"),
];
