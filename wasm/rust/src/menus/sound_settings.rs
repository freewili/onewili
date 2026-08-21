//! Sound Settings menu - generated from fwMenuSoundSettings. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct SoundSettings<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> SoundSettings<'a> {
    /// Quiet Threshold. The mic level that counts as an active sound. Wire: `h\s\n\f`
    pub fn quiet_threshold(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(508 /* CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_QUIET_THRESHOLD */, &a)?;
        Ok(())
    }

    /// Speaker Volume. The multiplier applied to sound playback. Wire: `h\s\n\v`
    pub fn speaker_volume(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(509 /* CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_SPEAKER_VOLUME */, &a)?;
        Ok(())
    }

    /// Recording Volume. The multiplier applied to mic recording. Wire: `h\s\n\c`
    pub fn recording_volume(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(510 /* CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_RECORDING_VOLUME */, &a)?;
        Ok(())
    }

    /// Record Len Sec. The default length of a recording in seconds. Wire: `h\s\n\r`
    pub fn record_len_sec(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(511 /* CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_RECORD_LEN_SEC */, &a)?;
        Ok(())
    }

    /// System Sounds. Sounds for system events (also gates all audio playback). Wire: `h\s\n\p`
    pub fn system_sounds(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(512 /* CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_SYSTEM_SOUNDS */, &a)?;
        Ok(())
    }
}
