//! Analog In (TLA2024) Settings menu - generated from fwMenuAnalogInSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct AnalogInSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> AnalogInSettings<'a> {
    /// Ch0 Input. TLA2024 channel 0 input mux. Wire: `h\s\j\0`
    pub fn ch0_input(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\0");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Ch1 Input. TLA2024 channel 1 input mux. Wire: `h\s\j\1`
    pub fn ch1_input(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\1");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Ch2 Input. TLA2024 channel 2 input mux. Wire: `h\s\j\2`
    pub fn ch2_input(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\2");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Ch3 Input. TLA2024 channel 3 input mux. Wire: `h\s\j\3`
    pub fn ch3_input(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\3");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Ch0 Range. TLA2024 channel 0 full-scale range. Wire: `h\s\j\4`
    pub fn ch0_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\4");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Ch1 Range. TLA2024 channel 1 full-scale range. Wire: `h\s\j\5`
    pub fn ch1_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\5");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Ch2 Range. TLA2024 channel 2 full-scale range. Wire: `h\s\j\6`
    pub fn ch2_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\6");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Ch3 Range. TLA2024 channel 3 full-scale range. Wire: `h\s\j\7`
    pub fn ch3_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\7");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Data Rate. TLA2024 conversion data rate. Wire: `h\s\j\8`
    pub fn data_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\j\\8");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
