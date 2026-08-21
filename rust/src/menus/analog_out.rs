//! Analog Out & Trigger Functions menu - generated from fwMenuAnalogOut. Do not edit.

use crate::encoding;
use crate::enums::dacWavePhase;
use crate::enums::dacWaveShapeMenu;
use crate::transport::{OwError, Transport};

pub struct AnalogOut<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> AnalogOut<'a> {
    /// Set Analog Output. sets the voltage of an analog output 0 or 1. ch 2 and 3 are use for window comparator. Wire: `i\a\s`
    pub fn set_analog_output(&mut self, channel: i32, value: f64) -> Result<(), OwError> {
        let mut cmd = String::from("i\\a\\s");
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_float(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Trigger Window. Trigger will be 1 when TrigV is between V- and V+.. Wire: `i\a\t`
    pub fn set_trigger_window(&mut self, value_low: f64, value_high: f64) -> Result<(), OwError> {
        let mut cmd = String::from("i\\a\\t");
        encoding::push_float(&mut cmd, value_low);
        encoding::push_float(&mut cmd, value_high);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Enable Trigger. Enables the Trigger Input to CPU. Wire: `i\a\e`
    pub fn set_enable_trigger(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\a\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Programmable VOut. Sets the programmable VOut: enable then target voltage.. Wire: `i\a\u`
    pub fn set_v_prog_vout(&mut self, enable: i32, set_voltage: f64) -> Result<(), OwError> {
        let mut cmd = String::from("i\\a\\u");
        encoding::push_int(&mut cmd, enable as i64);
        encoding::push_float(&mut cmd, set_voltage);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Glitch Programmable VOut. Briefly glitches the programmable VOut for the given nanoseconds.. Wire: `i\a\g`
    pub fn set_glitch(&mut self, nano_seconds: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\a\\g");
        encoding::push_int(&mut cmd, nano_seconds as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Waveform. Configures and starts the DAC63204 function generator on analog output 0 or 1.. Wire: `i\a\w`
    pub fn set_waveform(&mut self, channel: i32, waveform: dacWaveShapeMenu, frequency_hz: f64, low_voltage: f64, high_voltage: f64, phase: dacWavePhase) -> Result<(), OwError> {
        let mut cmd = String::from("i\\a\\w");
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_int(&mut cmd, waveform.0);
        encoding::push_float(&mut cmd, frequency_hz);
        encoding::push_float(&mut cmd, low_voltage);
        encoding::push_float(&mut cmd, high_voltage);
        encoding::push_int(&mut cmd, phase.0);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Waveform Run/Stop. Starts or stops the configured waveforms on analog outputs 0 and 1 in a single write.. Wire: `i\a\x`
    pub fn set_waveform_run(&mut self, mask: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\a\\x");
        encoding::push_int(&mut cmd, mask as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
