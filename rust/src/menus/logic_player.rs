//! Logic Player Functions menu - generated from fwMenuLogicPlayer. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct LogicPlayer<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> LogicPlayer<'a> {
    /// configure. Configures digital playback. Wire: `i\p\c`
    pub fn setup_player(&mut self, sample_rate_ns: i32, sample_count: i32, pin_start: i32, pin_stop: i32, start_mode: i32, trigger_pin: i32, loop_: bool) -> Result<(), OwError> {
        let mut cmd = String::from("i\\p\\c");
        encoding::push_int(&mut cmd, sample_rate_ns as i64);
        encoding::push_int(&mut cmd, sample_count as i64);
        encoding::push_int(&mut cmd, pin_start as i64);
        encoding::push_int(&mut cmd, pin_stop as i64);
        encoding::push_int(&mut cmd, start_mode as i64);
        encoding::push_int(&mut cmd, trigger_pin as i64);
        encoding::push_bool(&mut cmd, loop_);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// configure analog. Configures DAC playback. Wire: `i\p\a`
    pub fn setup_analog(&mut self, mask: i32, analog_rate_ns: i32, analog_resolution: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\p\\a");
        encoding::push_int(&mut cmd, mask as i64);
        encoding::push_int(&mut cmd, analog_rate_ns as i64);
        encoding::push_int(&mut cmd, analog_resolution as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// load. Loads a raw buffer from the filesystem. Wire: `i\p\l`
    pub fn load_file(&mut self, file_path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\p\\l");
        encoding::push_str(&mut cmd, file_path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// start. Starts playback. Wire: `i\p\s`
    pub fn start(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\p\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// stop. Stops playback. Wire: `i\p\e`
    pub fn stop(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\p\\e");
        self.t.call(&cmd)?;
        Ok(())
    }
}
