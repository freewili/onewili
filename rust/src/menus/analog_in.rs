//! Analog In Functions menu - generated from fwMenuAnalogIn. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct AnalogIn<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> AnalogIn<'a> {
    /// Stream Analog In. Streams analog input values to the host at the given rate.. Wire: `i\j\s`
    pub fn enable_analog_in_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\j\\s");
        encoding::push_int(&mut cmd, stream_rate_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read TLA2024. Reads the latest TLA2024 voltages for all 4 channels.. Wire: `i\j\r`
    pub fn read_analog_in2024(&mut self) -> Result<(f64, f64, f64, f64), OwError> {
        let cmd = String::from("i\\j\\r");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let v0 = encoding::tok_float(&mut toks)?;
        let v1 = encoding::tok_float(&mut toks)?;
        let v2 = encoding::tok_float(&mut toks)?;
        let v3 = encoding::tok_float(&mut toks)?;
        Ok((v0, v1, v2, v3))
    }

    /// Config TLA2024 Channel. Configures a TLA2024 channel: mux 0-7 = A0-A1,A0-A3,A1-A3,A2-A3,A0-GND,A1-GND,A2-GND,A3-GND; range 0-5 = 6.144V,4.096V,2.048V,1.024V,0.512V,0.256V.. Wire: `i\j\c`
    pub fn config_analog_in2024(&mut self, channel: i32, mux: i32, range: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\j\\c");
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_int(&mut cmd, mux as i64);
        encoding::push_int(&mut cmd, range as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// TLA2024 Data Rate. Sets the TLA2024 data rate: 0-6 = 128,250,490,920,1600,2400,3300 SPS.. Wire: `i\j\f`
    pub fn set_data_rate2024(&mut self, rate: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\j\\f");
        encoding::push_int(&mut cmd, rate as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream TLA2024. Streams TLA2024 voltages to the host at the given rate (0 stops).. Wire: `i\j\t`
    pub fn enable_analog_in2024_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\j\\t");
        encoding::push_int(&mut cmd, stream_rate_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("ainIn", false, "v0=float,v1=float,v2=float,v3=float", "Internal ADC voltages (connector channels 0-3)"),
    ("adcIn", false, "v0=float,v1=float,v2=float,v3=float", "TLA2024 voltages (connector channels 0-3)"),
];
