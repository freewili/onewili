//! Audio Functions menu - generated from fwMenuAudio. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Audio<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Audio<'a> {
    /// Play Audio File. Plays a .wav file from the sounds directory.. Wire: `i\k\f`
    pub fn play_audio_file(&mut self, file_path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(file_path);
        let _r = crate::transport::call(97 /* CMD_IO_AUDIO_PLAY_AUDIO_FILE */, &a)?;
        Ok(())
    }

    /// Record Audio. Records audio to a file (blank name = auto-named).. Wire: `i\k\r`
    pub fn record_audio_file(&mut self, file_name: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(file_name);
        let _r = crate::transport::call(98 /* CMD_IO_AUDIO_RECORD_AUDIO_FILE */, &a)?;
        Ok(())
    }

    /// Play Audio Asset. Plays a built-in audio asset by index or name.. Wire: `i\k\a`
    pub fn play_audio_asset(&mut self, asset_name: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(asset_name);
        let _r = crate::transport::call(99 /* CMD_IO_AUDIO_PLAY_AUDIO_ASSET */, &a)?;
        Ok(())
    }

    /// Stream Audio. Enables or disables audio streaming to the host.. Wire: `i\k\s`
    pub fn enable_audio_stream(&mut self, enable: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        let _r = crate::transport::call(100 /* CMD_IO_AUDIO_ENABLE_AUDIO_STREAM */, &a)?;
        Ok(())
    }

    /// Numbers to Speech. Speaks the given number aloud.. Wire: `i\k\n`
    pub fn numbers_to_speech(&mut self, number: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.f32(number as f32);
        let _r = crate::transport::call(101 /* CMD_IO_AUDIO_NUMBERS_TO_SPEECH */, &a)?;
        Ok(())
    }

    /// Play Tone. Plays a tone of the given frequency, duration, and amplitude.. Wire: `i\k\t`
    pub fn tone(&mut self, frequency: f64, duration_ms: f64, amplitude: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.f32(frequency as f32);
        a.f32(duration_ms as f32);
        a.f32(amplitude as f32);
        let _r = crate::transport::call(102 /* CMD_IO_AUDIO_TONE */, &a)?;
        Ok(())
    }

    /// Text to Speech. Speaks the given text aloud (text to speech).. Wire: `i\k\v`
    pub fn speak(&mut self, text: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(text);
        let _r = crate::transport::call(103 /* CMD_IO_AUDIO_SPEAK */, &a)?;
        Ok(())
    }
}
