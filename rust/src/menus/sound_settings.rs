//! Sound Settings menu - generated from fwMenuSoundSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct SoundSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> SoundSettings<'a> {
    /// Quiet Threshold. The mic level that counts as an active sound. Wire: `h\s\n\f`
    pub fn quiet_threshold(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\n\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Speaker Volume. The multiplier applied to sound playback. Wire: `h\s\n\v`
    pub fn speaker_volume(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\n\\v");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Recording Volume. The multiplier applied to mic recording. Wire: `h\s\n\c`
    pub fn recording_volume(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\n\\c");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Record Len Sec. The default length of a recording in seconds. Wire: `h\s\n\r`
    pub fn record_len_sec(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\n\\r");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// System Sounds. Sounds for system events (also gates all audio playback). Wire: `h\s\n\p`
    pub fn system_sounds(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\n\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
