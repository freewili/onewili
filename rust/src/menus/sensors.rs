//! Sensor Functions menu - generated from fwMenuSensors. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Sensors<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Sensors<'a> {
    /// Stream Motion. Streams accelerometer and gyroscope data to the host at the given rate. 0 stops the stream.. Wire: `i\s\m`
    pub fn enable_motion_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\s\\m");
        encoding::push_int(&mut cmd, stream_rate_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream Field. Streams magnetometer data to the host at the given rate. 0 stops the stream.. Wire: `i\s\f`
    pub fn enable_field_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\s\\f");
        encoding::push_int(&mut cmd, stream_rate_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream Env. Streams temperature, humidity and ambient light to the host. This stream is change-driven: the rate is a heartbeat floor, so samples can arrive faster when readings move. 0 stops the stream.. Wire: `i\s\e`
    pub fn enable_env_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\s\\e");
        encoding::push_int(&mut cmd, stream_rate_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream Orientation. Streams fused roll, pitch, yaw and heading to the host at the given rate. 0 stops the stream.. Wire: `i\s\r`
    pub fn enable_orientation_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\s\\r");
        encoding::push_int(&mut cmd, stream_rate_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Sensors. Prints the most recent sample from each of the four sensor groups.. Wire: `i\s\g`
    pub fn get_sensors(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\s\\g");
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("motion", false, "ax_mg=decS32,ay_mg=decS32,az_mg=decS32,gx_ddps=decS32,gy_ddps=decS32,gz_ddps=decS32", "Accelerometer and gyroscope data"),
    ("field", false, "mx_dut=decS32,my_dut=decS32,mz_dut=decS32,magnitude_dut=decS32,heading_cdeg=decS32", "Magnetometer data"),
    ("env", false, "temp_cc=decS32,rh_cpct=decS32,lux_clux=decU32", "Temperature, humidity and ambient light"),
    ("orientation", false, "roll_cdeg=decS32,pitch_cdeg=decS32,yaw_cdeg=decS32,heading_cdeg=decS32,flags=decS32", "Fused roll, pitch, yaw and heading"),
];
