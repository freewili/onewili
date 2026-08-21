//! Sensor Settings menu - generated from fwMenuSensorSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct SensorSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> SensorSettings<'a> {
    /// Accel Range. Accelerometer full-scale range index. Wire: `h\s\v\a`
    pub fn accel_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\v\\a");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Gyro Range. Gyroscope full-scale range index. Wire: `h\s\v\g`
    pub fn gyro_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\v\\g");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Move Threshold. The amount accel must change to signal movement. Wire: `h\s\v\m`
    pub fn move_threshold(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\v\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// TCal Scale. Temperature calibration, the m of mX+b. Wire: `h\s\v\s`
    pub fn t_cal_scale(&mut self, value: f64) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\v\\s");
        encoding::push_float(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// TCal Offset. Temperature calibration, the b of mX+b. Wire: `h\s\v\o`
    pub fn t_cal_offset(&mut self, value: f64) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\v\\o");
        encoding::push_float(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream Defaults. Bitmask of sensor streams enabled at boot: 1 accel-legacy, 2 temp, 4 motion, 8 field, 16 env, 32 orientation. Wire: `h\s\v\b`
    pub fn stream_defaults(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\v\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
