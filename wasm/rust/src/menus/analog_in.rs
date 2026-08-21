//! Analog In Functions menu - generated from fwMenuAnalogIn. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct AnalogIn<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> AnalogIn<'a> {
    /// Stream Analog In. Streams analog input values to the host at the given rate.. Wire: `i\j\s`
    pub fn enable_analog_in_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(stream_rate_ms);
        let _r = crate::transport::call(64 /* CMD_IO_ANALOG_IN_ENABLE_ANALOG_IN_STREAM */, &a)?;
        Ok(())
    }

    /// Read TLA2024. Reads the latest TLA2024 voltages for all 4 channels.. Wire: `i\j\r`
    pub fn read_analog_in2024(&mut self) -> Result<(f64, f64, f64, f64), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(65 /* CMD_IO_ANALOG_IN_READ_ANALOG_IN2024 */, &a)?;
        Ok((_r.f32() as f64, _r.f32() as f64, _r.f32() as f64, _r.f32() as f64))
    }

    /// Config TLA2024 Channel. Configures a TLA2024 channel: mux 0-7 = A0-A1,A0-A3,A1-A3,A2-A3,A0-GND,A1-GND,A2-GND,A3-GND; range 0-5 = 6.144V,4.096V,2.048V,1.024V,0.512V,0.256V.. Wire: `i\j\c`
    pub fn config_analog_in2024(&mut self, channel: i32, mux: i32, range: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(channel);
        a.i32(mux);
        a.i32(range);
        let _r = crate::transport::call(66 /* CMD_IO_ANALOG_IN_CONFIG_ANALOG_IN2024 */, &a)?;
        Ok(())
    }

    /// TLA2024 Data Rate. Sets the TLA2024 data rate: 0-6 = 128,250,490,920,1600,2400,3300 SPS.. Wire: `i\j\f`
    pub fn set_data_rate2024(&mut self, rate: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(rate);
        let _r = crate::transport::call(67 /* CMD_IO_ANALOG_IN_SET_DATA_RATE2024 */, &a)?;
        Ok(())
    }

    /// Stream TLA2024. Streams TLA2024 voltages to the host at the given rate (0 stops).. Wire: `i\j\t`
    pub fn enable_analog_in2024_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(stream_rate_ms);
        let _r = crate::transport::call(68 /* CMD_IO_ANALOG_IN_ENABLE_ANALOG_IN2024_STREAM */, &a)?;
        Ok(())
    }
}
