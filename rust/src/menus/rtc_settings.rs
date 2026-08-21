//! RTC Settings menu - generated from fwMenuRTCSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct RtcSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> RtcSettings<'a> {
    /// Year. Set the year on the real-time clock. Wire: `h\s\c\y`
    pub fn year(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\c\\y");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Month. Set the month on the real-time clock. Wire: `h\s\c\n`
    pub fn month(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\c\\n");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Day. Set the day of the month on the real-time clock. Wire: `h\s\c\e`
    pub fn day(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\c\\e");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Day Of Week. Set the day of the week on the real-time clock. Wire: `h\s\c\w`
    pub fn day_of_week(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\c\\w");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Hours. Set the hour on the real-time clock (24-hour format). Wire: `h\s\c\o`
    pub fn hours(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\c\\o");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Minutes. Set the minutes on the real-time clock. Wire: `h\s\c\m`
    pub fn minutes(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\c\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Seconds. Set the seconds on the real-time clock. Wire: `h\s\c\s`
    pub fn seconds(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\c\\s");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Trim. Add or subtract n*2 clock cycles every minute. Wire: `h\s\c\t`
    pub fn trim(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\c\\t");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
