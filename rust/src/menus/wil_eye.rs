//! WILEye Functions menu - generated from fwMenuWILEye. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct WilEye<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> WilEye<'a> {
    /// Take a Picture. Take a picture from WILEye and save its SD card or FREE-WILi's Files system by file name.. Wire: `i\f\t`
    pub fn take_picture(&mut self, destination: i32, filename: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\t");
        encoding::push_int(&mut cmd, destination as i64);
        encoding::push_str(&mut cmd, filename);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Start Recording Video. Start recording video from WILEye and save it to SD card by file name. Wire: `i\f\v`
    pub fn start_recording_video(&mut self, filename: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\v");
        encoding::push_str(&mut cmd, filename);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop Recording Video. Stop recording video from WILEye. Wire: `i\f\s`
    pub fn stop_recording_video(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\f\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream AI Detection Events. Stream AI Detection Events from WILEye. Wire: `i\f\a`
    pub fn toggle_ai_detection_stream(&mut self, ai_stream_mode: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\a");
        encoding::push_int(&mut cmd, ai_stream_mode as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Zoom. Set the zoom level of WILEye. Wire: `i\f\m`
    pub fn set_zoom_level(&mut self, zoom: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\m");
        encoding::push_int(&mut cmd, zoom as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Contrast. Set the contrast level of WILEye. Wire: `i\f\c`
    pub fn set_contrast(&mut self, contrast: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\c");
        encoding::push_int(&mut cmd, contrast as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Saturation. Set the saturation level of WILEye. Wire: `i\f\i`
    pub fn set_saturation(&mut self, saturation: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\i");
        encoding::push_int(&mut cmd, saturation as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Brightness. Set the brightness level of WILEye. Wire: `i\f\b`
    pub fn set_brightness(&mut self, brightness: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\b");
        encoding::push_int(&mut cmd, brightness as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Hue. Set the hue level of WILEye. Wire: `i\f\u`
    pub fn set_hue(&mut self, hue: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\u");
        encoding::push_int(&mut cmd, hue as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Resolution. Set the resolution state of WILEye. Wire: `i\f\y`
    pub fn set_resolution(&mut self, resolutionstate: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\y");
        encoding::push_int(&mut cmd, resolutionstate as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Enable Disable Flash. Set the flash state of WILEye. Wire: `i\f\l`
    pub fn set_flash_state(&mut self, flash: bool) -> Result<(), OwError> {
        let mut cmd = String::from("i\\f\\l");
        encoding::push_bool(&mut cmd, flash);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("WILEye", false, "data_bytes=hexbytes", "DEPRECATED: never emitted; kept for wire compatibility (see WILEyeAI)"),
    ("WILEyeImgStart", false, "data=string", "WILEye image transfer started ('Image Stream Start: N bytes')"),
    ("WILEyeImgChunk", false, "data=string", "WILEye image chunk received ('N bytes')"),
    ("WILEyeImgEnd", false, "data=string", "WILEye image transfer complete ('saved as: <file>')"),
    ("WILEyeImgAbort", false, "data=string", "WILEye image transfer aborted (timeout)"),
    ("WILEyeSDcard", false, "data=string", "WILEye SD card switched to USB mode"),
    ("WILEyeAI", false, "data=string", "WILEye AI detection event (mode + bounding box text)"),
    ("WILEyeUnknown", false, "data=string", "WILEye unknown message received"),
];
