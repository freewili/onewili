//! Sensor Settings menu - generated from fwMenuSensorSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct SensorSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> SensorSettings<'a> {
    /// Accel Range. Accelerometer full-scale range index. Wire: `h\s\v\a`
    pub fn accel_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(156 /* CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_ACCEL_RANGE */, &a)?;
        Ok(())
    }

    /// Gyro Range. Gyroscope full-scale range index. Wire: `h\s\v\g`
    pub fn gyro_range(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(157 /* CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_GYRO_RANGE */, &a)?;
        Ok(())
    }

    /// Move Threshold. The amount accel must change to signal movement. Wire: `h\s\v\m`
    pub fn move_threshold(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(158 /* CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_MOVE_THRESHOLD */, &a)?;
        Ok(())
    }

    /// TCal Scale. Temperature calibration, the m of mX+b. Wire: `h\s\v\s`
    pub fn t_cal_scale(&mut self, value: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.f32(value as f32);
        let _r = crate::transport::call(159 /* CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_T_CAL_SCALE */, &a)?;
        Ok(())
    }

    /// TCal Offset. Temperature calibration, the b of mX+b. Wire: `h\s\v\o`
    pub fn t_cal_offset(&mut self, value: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.f32(value as f32);
        let _r = crate::transport::call(160 /* CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_T_CAL_OFFSET */, &a)?;
        Ok(())
    }

    /// Stream Defaults. Bitmask of sensor streams enabled at boot: 1 accel-legacy, 2 temp, 4 motion, 8 field, 16 env, 32 orientation. Wire: `h\s\v\b`
    pub fn stream_defaults(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(161 /* CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_STREAM_DEFAULTS */, &a)?;
        Ok(())
    }
}
