//! Audio Functions menu - generated from fwMenuAudio. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Audio<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Audio<'a> {
    /// Play Audio File. Plays a .wav file from the sounds directory.. Wire: `i\k\f`
    pub fn play_audio_file(&mut self, file_path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\k\\f");
        encoding::push_str(&mut cmd, file_path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Record Audio. Records audio to a file (blank name = auto-named).. Wire: `i\k\r`
    pub fn record_audio_file(&mut self, file_name: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\k\\r");
        encoding::push_str(&mut cmd, file_name);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Play Audio Asset. Plays a built-in audio asset by index or name.. Wire: `i\k\a`
    pub fn play_audio_asset(&mut self, asset_name: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\k\\a");
        encoding::push_str(&mut cmd, asset_name);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream Audio. Enables or disables audio streaming to the host.. Wire: `i\k\s`
    pub fn enable_audio_stream(&mut self, enable: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\k\\s");
        encoding::push_int(&mut cmd, enable as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Numbers to Speech. Speaks the given number aloud.. Wire: `i\k\n`
    pub fn numbers_to_speech(&mut self, number: f64) -> Result<(), OwError> {
        let mut cmd = String::from("i\\k\\n");
        encoding::push_float(&mut cmd, number);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Play Tone. Plays a tone of the given frequency, duration, and amplitude.. Wire: `i\k\t`
    pub fn tone(&mut self, frequency: f64, duration_ms: f64, amplitude: f64) -> Result<(), OwError> {
        let mut cmd = String::from("i\\k\\t");
        encoding::push_float(&mut cmd, frequency);
        encoding::push_float(&mut cmd, duration_ms);
        encoding::push_float(&mut cmd, amplitude);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Text to Speech. Speaks the given text aloud (text to speech).. Wire: `i\k\v`
    pub fn speak(&mut self, text: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\k\\v");
        encoding::push_str(&mut cmd, text);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("record", false, "progress=decU32", "Sound recording progress (permille of the clip length)"),
    ("audio", false, "s0=decS32,s1=decS32,s2=decS32,s3=decS32,s4=decS32,s5=decS32,s6=decS32,s7=decS32", "PDM microphone sample batch (8 signed samples per event)"),
];
