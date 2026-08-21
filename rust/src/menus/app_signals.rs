//! App Signals menu - generated from fwMenuAppSignals. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct AppSignals<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> AppSignals<'a> {
    /// Add. Adds an app signal.. Wire: `s\i\a`
    pub fn app_signal_add(&mut self, name: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\i\\a");
        encoding::push_str(&mut cmd, name);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Remove. Removes an app signal.. Wire: `s\i\x`
    pub fn app_signal_remove(&mut self, name: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\i\\x");
        encoding::push_str(&mut cmd, name);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Rename. Renames an app signal.. Wire: `s\i\r`
    pub fn app_signal_rename(&mut self, name: &str, new_name: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\i\\r");
        encoding::push_str(&mut cmd, name);
        encoding::push_str(&mut cmd, new_name);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Value. Sets an app signal value.. Wire: `s\i\s`
    pub fn app_signal_set(&mut self, name: &str, value: f64) -> Result<(), OwError> {
        let mut cmd = String::from("s\\i\\s");
        encoding::push_str(&mut cmd, name);
        encoding::push_float(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Value. Gets an app signal value.. Wire: `s\i\g`
    pub fn app_signal_get(&mut self, name: &str) -> Result<(String, f64), OwError> {
        let mut cmd = String::from("s\\i\\g");
        encoding::push_str(&mut cmd, name);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let name = encoding::rest_str(&mut toks);
        let value = encoding::tok_float(&mut toks)?;
        Ok((name, value))
    }

    /// Apply Wave. Applies wave mode 0-8 (0 off; sine, triangle, square and saw at 0.5/2 Hz).. Wire: `s\i\w`
    pub fn app_signal_wave(&mut self, name: &str, wave: i32) -> Result<(), OwError> {
        let mut cmd = String::from("s\\i\\w");
        encoding::push_str(&mut cmd, name);
        encoding::push_int(&mut cmd, wave as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream. Streams every defined app signal; 0 disables streaming.. Wire: `s\i\t`
    pub fn app_signal_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("s\\i\\t");
        encoding::push_int(&mut cmd, stream_rate_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("appSignal", false, "name=string,value=float", "Streamed app-signal value."),
];
