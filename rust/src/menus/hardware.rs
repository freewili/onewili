//! Hardware Functions menu - generated from fwMenuHardware. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Hardware<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Hardware<'a> {
    /// Device Settings sub-menu.
    pub fn settings_home(self) -> super::settings_home::SettingsHome<'a> {
        super::settings_home::SettingsHome { t: self.t }
    }

    /// System Functions sub-menu.
    pub fn system(self) -> super::system::System<'a> {
        super::system::System { t: self.t }
    }

    /// File System sub-menu.
    pub fn file_system(self) -> super::file_system::FileSystem<'a> {
        super::file_system::FileSystem { t: self.t }
    }

    /// Power Management sub-menu.
    pub fn power_management(self) -> super::power_management::PowerManagement<'a> {
        super::power_management::PowerManagement { t: self.t }
    }

    /// Display Functions sub-menu.
    pub fn display_functions(self) -> super::display_functions::DisplayFunctions<'a> {
        super::display_functions::DisplayFunctions { t: self.t }
    }

    /// Get Time. Read the current date and time from the board RTC (weekday 0=Sun..6=Sat). Wire: `h\t`
    pub fn get_time(&mut self) -> Result<(i32, i32, i32, i32, i32, i32, i32), OwError> {
        let cmd = String::from("h\\t");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let year = encoding::tok_int(&mut toks)? as i32;
        let month = encoding::tok_int(&mut toks)? as i32;
        let day = encoding::tok_int(&mut toks)? as i32;
        let weekday = encoding::tok_int(&mut toks)? as i32;
        let hour = encoding::tok_int(&mut toks)? as i32;
        let min = encoding::tok_int(&mut toks)? as i32;
        let sec = encoding::tok_int(&mut toks)? as i32;
        Ok((year, month, day, weekday, hour, min, sec))
    }

    /// Set Time. Set the board RTC date and time; the weekday is computed from the date. Wire: `h\c`
    pub fn set_time(&mut self, year: i32, month: i32, day: i32, hour: i32, min: i32, sec: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\c");
        encoding::push_int(&mut cmd, year as i64);
        encoding::push_int(&mut cmd, month as i64);
        encoding::push_int(&mut cmd, day as i64);
        encoding::push_int(&mut cmd, hour as i64);
        encoding::push_int(&mut cmd, min as i64);
        encoding::push_int(&mut cmd, sec as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
