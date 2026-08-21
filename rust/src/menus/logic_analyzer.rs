//! Logic Analyzer Functions menu - generated from fwMenuLogicAnalyzer. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct LogicAnalyzer<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> LogicAnalyzer<'a> {
    /// configure. Configures the logic analyzer capture.. Wire: `i\b\c`
    pub fn setup_logic_analyzer(&mut self, sample_rate_ns: i32, sample_count: i32, pin_start: i32, pin_stop: i32, trigger_pin: i32, trigger_type: i32, rearm: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\b\\c");
        encoding::push_int(&mut cmd, sample_rate_ns as i64);
        encoding::push_int(&mut cmd, sample_count as i64);
        encoding::push_int(&mut cmd, pin_start as i64);
        encoding::push_int(&mut cmd, pin_stop as i64);
        encoding::push_int(&mut cmd, trigger_pin as i64);
        encoding::push_int(&mut cmd, trigger_type as i64);
        encoding::push_int(&mut cmd, rearm as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// configure analog. Configures the analog capture inputs.. Wire: `i\b\a`
    pub fn setup_analog(&mut self, analog_mask: i32, analog_rate_ns: i32, analog_res: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\b\\a");
        encoding::push_int(&mut cmd, analog_mask as i64);
        encoding::push_int(&mut cmd, analog_rate_ns as i64);
        encoding::push_int(&mut cmd, analog_res as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// start. Starts logic analyzer capture.. Wire: `i\b\s`
    pub fn start(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\b\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// stop. Stops logic analyzer capture.. Wire: `i\b\e`
    pub fn stop(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\b\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// trigger. Manually triggers the logic analyzer.. Wire: `i\b\t`
    pub fn trigger(&mut self, trigger_type: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\b\\t");
        encoding::push_int(&mut cmd, trigger_type as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("logicAnalyzerReport", true, "trigger_time_stamp_ns=hexU64,sample_rate_ns=decU32,samples=hexbytes", "Logic analyzer capture report (binary API; header then digital + analog samples)"),
];
