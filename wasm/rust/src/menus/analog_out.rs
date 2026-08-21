//! Analog Out & Trigger Functions menu - generated from fwMenuAnalogOut. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::enums::dacWavePhase;
use crate::enums::dacWaveShapeMenu;
use crate::transport::OwError;

pub struct AnalogOut<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> AnalogOut<'a> {
    /// Set Analog Output. sets the voltage of an analog output 0 or 1. ch 2 and 3 are use for window comparator. Wire: `i\a\s`
    pub fn set_analog_output(&mut self, channel: i32, value: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(channel);
        a.f32(value as f32);
        let _r = crate::transport::call(69 /* CMD_IO_ANALOG_OUT_SET_ANALOG_OUTPUT */, &a)?;
        Ok(())
    }

    /// Set Trigger Window. Trigger will be 1 when TrigV is between V- and V+.. Wire: `i\a\t`
    pub fn set_trigger_window(&mut self, value_low: f64, value_high: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.f32(value_low as f32);
        a.f32(value_high as f32);
        let _r = crate::transport::call(70 /* CMD_IO_ANALOG_OUT_SET_TRIGGER_WINDOW */, &a)?;
        Ok(())
    }

    /// Enable Trigger. Enables the Trigger Input to CPU. Wire: `i\a\e`
    pub fn set_enable_trigger(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(71 /* CMD_IO_ANALOG_OUT_SET_ENABLE_TRIGGER */, &a)?;
        Ok(())
    }

    /// Set Programmable VOut. Sets the programmable VOut: enable then target voltage.. Wire: `i\a\u`
    pub fn set_v_prog_vout(&mut self, enable: i32, set_voltage: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        a.f32(set_voltage as f32);
        let _r = crate::transport::call(72 /* CMD_IO_ANALOG_OUT_SET_V_PROG_VOUT */, &a)?;
        Ok(())
    }

    /// Glitch Programmable VOut. Briefly glitches the programmable VOut for the given nanoseconds.. Wire: `i\a\g`
    pub fn set_glitch(&mut self, nano_seconds: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(nano_seconds);
        let _r = crate::transport::call(73 /* CMD_IO_ANALOG_OUT_SET_GLITCH */, &a)?;
        Ok(())
    }

    /// Set Waveform. Configures and starts the DAC63204 function generator on analog output 0 or 1.. Wire: `i\a\w`
    pub fn set_waveform(&mut self, channel: i32, waveform: dacWaveShapeMenu, frequency_hz: f64, low_voltage: f64, high_voltage: f64, phase: dacWavePhase) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(channel);
        a.i32(waveform.0 as i32);
        a.f32(frequency_hz as f32);
        a.f32(low_voltage as f32);
        a.f32(high_voltage as f32);
        a.i32(phase.0 as i32);
        let _r = crate::transport::call(74 /* CMD_IO_ANALOG_OUT_SET_WAVEFORM */, &a)?;
        Ok(())
    }

    /// Waveform Run/Stop. Starts or stops the configured waveforms on analog outputs 0 and 1 in a single write.. Wire: `i\a\x`
    pub fn set_waveform_run(&mut self, mask: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(mask);
        let _r = crate::transport::call(75 /* CMD_IO_ANALOG_OUT_SET_WAVEFORM_RUN */, &a)?;
        Ok(())
    }
}
