//! RTC Settings menu - generated from fwMenuRTCSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct RtcSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> RtcSettings<'a> {
    /// Year. Set the year on the real-time clock. Wire: `h\s\c\y`
    pub fn year(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(231 /* CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_YEAR */, &a)?;
        Ok(())
    }

    /// Month. Set the month on the real-time clock. Wire: `h\s\c\n`
    pub fn month(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(232 /* CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_MONTH */, &a)?;
        Ok(())
    }

    /// Day. Set the day of the month on the real-time clock. Wire: `h\s\c\e`
    pub fn day(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(233 /* CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_DAY */, &a)?;
        Ok(())
    }

    /// Day Of Week. Set the day of the week on the real-time clock. Wire: `h\s\c\w`
    pub fn day_of_week(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(234 /* CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_DAY_OF_WEEK */, &a)?;
        Ok(())
    }

    /// Hours. Set the hour on the real-time clock (24-hour format). Wire: `h\s\c\o`
    pub fn hours(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(235 /* CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_HOURS */, &a)?;
        Ok(())
    }

    /// Minutes. Set the minutes on the real-time clock. Wire: `h\s\c\m`
    pub fn minutes(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(236 /* CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_MINUTES */, &a)?;
        Ok(())
    }

    /// Seconds. Set the seconds on the real-time clock. Wire: `h\s\c\s`
    pub fn seconds(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(237 /* CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_SECONDS */, &a)?;
        Ok(())
    }

    /// Trim. Add or subtract n*2 clock cycles every minute. Wire: `h\s\c\t`
    pub fn trim(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(238 /* CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_TRIM */, &a)?;
        Ok(())
    }
}
