//! App Signals menu - generated from fwMenuAppSignals. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct AppSignals<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> AppSignals<'a> {
    /// Add. Adds an app signal.. Wire: `s\i\a`
    pub fn app_signal_add(&mut self, name: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(name);
        let _r = crate::transport::call(446 /* CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_ADD */, &a)?;
        Ok(())
    }

    /// Remove. Removes an app signal.. Wire: `s\i\x`
    pub fn app_signal_remove(&mut self, name: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(name);
        let _r = crate::transport::call(452 /* CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_REMOVE */, &a)?;
        Ok(())
    }

    /// Rename. Renames an app signal.. Wire: `s\i\r`
    pub fn app_signal_rename(&mut self, name: &str, new_name: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(name);
        a.str(new_name);
        let _r = crate::transport::call(448 /* CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_RENAME */, &a)?;
        Ok(())
    }

    /// Set Value. Sets an app signal value.. Wire: `s\i\s`
    pub fn app_signal_set(&mut self, name: &str, value: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(name);
        a.f32(value as f32);
        let _r = crate::transport::call(449 /* CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_SET */, &a)?;
        Ok(())
    }

    /// Get Value. Gets an app signal value.. Wire: `s\i\g`
    pub fn app_signal_get(&mut self, name: &str) -> Result<(String, f64), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(name);
        let mut _r = crate::transport::call(447 /* CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_GET */, &a)?;
        Ok((_r.string(), _r.f32() as f64))
    }

    /// Apply Wave. Applies wave mode 0-8 (0 off; sine, triangle, square and saw at 0.5/2 Hz).. Wire: `s\i\w`
    pub fn app_signal_wave(&mut self, name: &str, wave: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(name);
        a.i32(wave);
        let _r = crate::transport::call(451 /* CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_WAVE */, &a)?;
        Ok(())
    }

    /// Stream. Streams every defined app signal; 0 disables streaming.. Wire: `s\i\t`
    pub fn app_signal_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(stream_rate_ms);
        let _r = crate::transport::call(450 /* CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_STREAM */, &a)?;
        Ok(())
    }
}
