//! Logger menu - generated from fwMenuLogger. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Logger<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Logger<'a> {
    /// Start. Arms the logger with the current settings; Immediate trigger mode starts capturing at once. Emits logger events (armed/triggered/complete/error) as it runs.. Wire: `r\s`
    pub fn start(&mut self) -> Result<(), OwError> {
        let cmd = String::from("r\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop. Stops the logger: an armed capture is discarded, a running capture drains its remaining events to the files and closes them.. Wire: `r\e`
    pub fn stop(&mut self) -> Result<(), OwError> {
        let cmd = String::from("r\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Trigger. Software trigger: fires an armed capture regardless of the configured trigger mode.. Wire: `r\t`
    pub fn trigger(&mut self) -> Result<(), OwError> {
        let cmd = String::from("r\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Status. Prints the logger state, file format, trigger mode, output file names and event counters.. Wire: `r\i`
    pub fn status(&mut self) -> Result<(), OwError> {
        let cmd = String::from("r\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// File Format. Output file format for the next capture: CSV text, RTIX binary, or both. Wire: `r\f`
    pub fn file_format(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("r\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Trigger Mode. How an armed capture is triggered: Immediate (on start), Button (a device button press), or Expression (a device expression becoming nonzero). Wire: `r\m`
    pub fn trigger_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("r\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Trigger Button. Device button that fires the trigger in Button mode. Wire: `r\b`
    pub fn trigger_button(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("r\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Trigger Expression. Expression evaluated every 50 ms in Expression mode; the trigger fires when it evaluates nonzero. Wire: `r\x`
    pub fn trigger_expression(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("r\\x");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Pre Trigger Ms. Milliseconds of events kept from before the trigger (0-60000). Wire: `r\p`
    pub fn pre_trigger_ms(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("r\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Post Trigger Ms. Milliseconds captured after the trigger before the files close (0 = until stop, max 600000). Wire: `r\o`
    pub fn post_trigger_ms(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("r\\o");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Events. Selects which events this instance captures: "all", "none", a comma-separated event-name list, or +name/-name to add/remove one event from the current selection. Wire: `r\v`
    pub fn events(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("r\\v");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Active Instance. Selects which of the four logger instances (0-3) the settings rows show and the start, stop and trigger commands act on; every instance keeps its own saved configuration. Wire: `r\n`
    pub fn active_instance(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("r\\n");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Name. Optional name for this instance; captures are written to /logs/<name>/<name>_NNNN.* instead of /logs/logI_NNNN.*. Wire: `r\a`
    pub fn name(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("r\\a");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("logger", false, "info=string", "Logger state change, prefixed with the instance number 0-3: <inst> armed, <inst> triggered, <inst> complete <csv> <rtix> <n> records, or <inst> error <reason>"),
];
