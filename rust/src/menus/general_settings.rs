//! General Settings menu - generated from fwMenuGeneralSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct GeneralSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> GeneralSettings<'a> {
    /// Startup Wasm Script. Path to wasm or RTHON script.. Wire: `h\s\e\a`
    pub fn startup_wasm_script(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\e\\a");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Startup Zoom Script. Path to zoom script.. Wire: `h\s\e\b`
    pub fn startup_zoom_script(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\e\\b");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Default FPGA Script. Path to FPGA bit file. Wire: `h\s\e\c`
    pub fn default_fpga_script(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\e\\c");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Wasm debug level. Debug messaging from WiliWasm. Wire: `h\s\e\f`
    pub fn wasm_debug_level(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\e\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
