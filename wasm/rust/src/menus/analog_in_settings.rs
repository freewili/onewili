//! Analog In (TLA2024) Settings menu - generated from fwMenuAnalogInSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct AnalogInSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> AnalogInSettings<'a> {
    /// Ch0 Input. TLA2024 channel 0 input mux. Wire: `h\s\j\0`
    pub fn ch0_input(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(282 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH0_INPUT */, &a)?;
        Ok(())
    }

    /// Ch1 Input. TLA2024 channel 1 input mux. Wire: `h\s\j\1`
    pub fn ch1_input(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(283 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH1_INPUT */, &a)?;
        Ok(())
    }

    /// Ch2 Input. TLA2024 channel 2 input mux. Wire: `h\s\j\2`
    pub fn ch2_input(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(284 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH2_INPUT */, &a)?;
        Ok(())
    }

    /// Ch3 Input. TLA2024 channel 3 input mux. Wire: `h\s\j\3`
    pub fn ch3_input(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(285 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH3_INPUT */, &a)?;
        Ok(())
    }

    /// Ch0 Range. TLA2024 channel 0 full-scale range. Wire: `h\s\j\4`
    pub fn ch0_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(286 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH0_RANGE */, &a)?;
        Ok(())
    }

    /// Ch1 Range. TLA2024 channel 1 full-scale range. Wire: `h\s\j\5`
    pub fn ch1_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(287 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH1_RANGE */, &a)?;
        Ok(())
    }

    /// Ch2 Range. TLA2024 channel 2 full-scale range. Wire: `h\s\j\6`
    pub fn ch2_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(288 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH2_RANGE */, &a)?;
        Ok(())
    }

    /// Ch3 Range. TLA2024 channel 3 full-scale range. Wire: `h\s\j\7`
    pub fn ch3_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(289 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH3_RANGE */, &a)?;
        Ok(())
    }

    /// Data Rate. TLA2024 conversion data rate. Wire: `h\s\j\8`
    pub fn data_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(290 /* CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_DATA_RATE */, &a)?;
        Ok(())
    }
}
