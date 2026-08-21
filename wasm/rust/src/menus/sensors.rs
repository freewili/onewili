//! Sensor Functions menu - generated from fwMenuSensors. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Sensors<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Sensors<'a> {
    /// Stream Motion. Streams accelerometer and gyroscope data to the host at the given rate. 0 stops the stream.. Wire: `i\s\m`
    pub fn enable_motion_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(stream_rate_ms);
        let _r = crate::transport::call(42 /* CMD_IO_SENSORS_ENABLE_MOTION_STREAM */, &a)?;
        Ok(())
    }

    /// Stream Field. Streams magnetometer data to the host at the given rate. 0 stops the stream.. Wire: `i\s\f`
    pub fn enable_field_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(stream_rate_ms);
        let _r = crate::transport::call(43 /* CMD_IO_SENSORS_ENABLE_FIELD_STREAM */, &a)?;
        Ok(())
    }

    /// Stream Env. Streams temperature, humidity and ambient light to the host. This stream is change-driven: the rate is a heartbeat floor, so samples can arrive faster when readings move. 0 stops the stream.. Wire: `i\s\e`
    pub fn enable_env_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(stream_rate_ms);
        let _r = crate::transport::call(44 /* CMD_IO_SENSORS_ENABLE_ENV_STREAM */, &a)?;
        Ok(())
    }

    /// Stream Orientation. Streams fused roll, pitch, yaw and heading to the host at the given rate. 0 stops the stream.. Wire: `i\s\r`
    pub fn enable_orientation_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(stream_rate_ms);
        let _r = crate::transport::call(45 /* CMD_IO_SENSORS_ENABLE_ORIENTATION_STREAM */, &a)?;
        Ok(())
    }

    /// Get Sensors. Prints the most recent sample from each of the four sensor groups.. Wire: `i\s\g`
    pub fn get_sensors(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(46 /* CMD_IO_SENSORS_GET_SENSORS */, &a)?;
        Ok(())
    }
}
