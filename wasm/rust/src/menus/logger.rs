//! Logger menu - generated from fwMenuLogger. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Logger<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Logger<'a> {
    /// Start. Arms the logger with the current settings; Immediate trigger mode starts capturing at once. Emits logger events (armed/triggered/complete/error) as it runs.. Wire: `r\s`
    pub fn start(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(460 /* CMD_LOGGER_START */, &a)?;
        Ok(())
    }

    /// Stop. Stops the logger: an armed capture is discarded, a running capture drains its remaining events to the files and closes them.. Wire: `r\e`
    pub fn stop(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(454 /* CMD_LOGGER_STOP */, &a)?;
        Ok(())
    }

    /// Trigger. Software trigger: fires an armed capture regardless of the configured trigger mode.. Wire: `r\t`
    pub fn trigger(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(461 /* CMD_LOGGER_TRIGGER */, &a)?;
        Ok(())
    }

    /// Status. Prints the logger state, file format, trigger mode, output file names and event counters.. Wire: `r\i`
    pub fn status(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(456 /* CMD_LOGGER_STATUS */, &a)?;
        Ok(())
    }

    /// File Format. Output file format for the next capture: CSV text, RTIX binary, or both. Wire: `r\f`
    pub fn file_format(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(455 /* CMD_LOGGER_FILE_FORMAT */, &a)?;
        Ok(())
    }

    /// Trigger Mode. How an armed capture is triggered: Immediate (on start), Button (a device button press), or Expression (a device expression becoming nonzero). Wire: `r\m`
    pub fn trigger_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(457 /* CMD_LOGGER_TRIGGER_MODE */, &a)?;
        Ok(())
    }

    /// Trigger Button. Device button that fires the trigger in Button mode. Wire: `r\b`
    pub fn trigger_button(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(453 /* CMD_LOGGER_TRIGGER_BUTTON */, &a)?;
        Ok(())
    }

    /// Trigger Expression. Expression evaluated every 50 ms in Expression mode; the trigger fires when it evaluates nonzero. Wire: `r\x`
    pub fn trigger_expression(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(463 /* CMD_LOGGER_TRIGGER_EXPRESSION */, &a)?;
        Ok(())
    }

    /// Pre Trigger Ms. Milliseconds of events kept from before the trigger (0-60000). Wire: `r\p`
    pub fn pre_trigger_ms(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(459 /* CMD_LOGGER_PRE_TRIGGER_MS */, &a)?;
        Ok(())
    }

    /// Post Trigger Ms. Milliseconds captured after the trigger before the files close (0 = until stop, max 600000). Wire: `r\o`
    pub fn post_trigger_ms(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(458 /* CMD_LOGGER_POST_TRIGGER_MS */, &a)?;
        Ok(())
    }

    /// Events. Selects which events this instance captures: "all", "none", a comma-separated event-name list, or +name/-name to add/remove one event from the current selection. Wire: `r\v`
    pub fn events(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(462 /* CMD_LOGGER_EVENTS */, &a)?;
        Ok(())
    }

    /// Active Instance. Selects which of the four logger instances (0-3) the settings rows show and the start, stop and trigger commands act on; every instance keeps its own saved configuration. Wire: `r\n`
    pub fn active_instance(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(464 /* CMD_LOGGER_ACTIVE_INSTANCE */, &a)?;
        Ok(())
    }

    /// Name. Optional name for this instance; captures are written to /logs/<name>/<name>_NNNN.* instead of /logs/logI_NNNN.*. Wire: `r\a`
    pub fn name(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(465 /* CMD_LOGGER_NAME */, &a)?;
        Ok(())
    }
}
