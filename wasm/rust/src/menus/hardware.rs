//! Hardware Functions menu - generated from fwMenuHardware. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Hardware<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
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
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(476 /* CMD_HARDWARE_GET_TIME */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32()))
    }

    /// Set Time. Set the board RTC date and time; the weekday is computed from the date. Wire: `h\c`
    pub fn set_time(&mut self, year: i32, month: i32, day: i32, hour: i32, min: i32, sec: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(year);
        a.i32(month);
        a.i32(day);
        a.i32(hour);
        a.i32(min);
        a.i32(sec);
        let _r = crate::transport::call(477 /* CMD_HARDWARE_SET_TIME */, &a)?;
        Ok(())
    }
}
