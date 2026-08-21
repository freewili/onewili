//! General Settings menu - generated from fwMenuGeneralSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct GeneralSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> GeneralSettings<'a> {
    /// Startup Wasm Script. Path to wasm or RTHON script.. Wire: `h\s\e\a`
    pub fn startup_wasm_script(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(278 /* CMD_HARDWARE_SETTINGS_HOME_GENERAL_SETTINGS_STARTUP_WASM_SCRIPT */, &a)?;
        Ok(())
    }

    /// Startup Zoom Script. Path to zoom script.. Wire: `h\s\e\b`
    pub fn startup_zoom_script(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(279 /* CMD_HARDWARE_SETTINGS_HOME_GENERAL_SETTINGS_STARTUP_ZOOM_SCRIPT */, &a)?;
        Ok(())
    }

    /// Default FPGA Script. Path to FPGA bit file. Wire: `h\s\e\c`
    pub fn default_fpga_script(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(280 /* CMD_HARDWARE_SETTINGS_HOME_GENERAL_SETTINGS_DEFAULT_FPGA_SCRIPT */, &a)?;
        Ok(())
    }

    /// Wasm debug level. Debug messaging from WiliWasm. Wire: `h\s\e\f`
    pub fn wasm_debug_level(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(281 /* CMD_HARDWARE_SETTINGS_HOME_GENERAL_SETTINGS_WASM_DEBUG_LEVEL */, &a)?;
        Ok(())
    }
}
