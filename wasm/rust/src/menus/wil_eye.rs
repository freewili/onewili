//! WILEye Functions menu - generated from fwMenuWILEye. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct WilEye<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> WilEye<'a> {
    /// Take a Picture. Take a picture from WILEye and save its SD card or FREE-WILi's Files system by file name.. Wire: `i\f\t`
    pub fn take_picture(&mut self, destination: i32, filename: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(destination);
        a.str(filename);
        let _r = crate::transport::call(86 /* CMD_IO_WIL_EYE_TAKE_PICTURE */, &a)?;
        Ok(())
    }

    /// Start Recording Video. Start recording video from WILEye and save it to SD card by file name. Wire: `i\f\v`
    pub fn start_recording_video(&mut self, filename: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filename);
        let _r = crate::transport::call(87 /* CMD_IO_WIL_EYE_START_RECORDING_VIDEO */, &a)?;
        Ok(())
    }

    /// Stop Recording Video. Stop recording video from WILEye. Wire: `i\f\s`
    pub fn stop_recording_video(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(88 /* CMD_IO_WIL_EYE_STOP_RECORDING_VIDEO */, &a)?;
        Ok(())
    }

    /// Stream AI Detection Events. Stream AI Detection Events from WILEye. Wire: `i\f\a`
    pub fn toggle_ai_detection_stream(&mut self, ai_stream_mode: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(ai_stream_mode);
        let _r = crate::transport::call(89 /* CMD_IO_WIL_EYE_TOGGLE_AI_DETECTION_STREAM */, &a)?;
        Ok(())
    }

    /// Set Zoom. Set the zoom level of WILEye. Wire: `i\f\m`
    pub fn set_zoom_level(&mut self, zoom: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(zoom);
        let _r = crate::transport::call(90 /* CMD_IO_WIL_EYE_SET_ZOOM_LEVEL */, &a)?;
        Ok(())
    }

    /// Set Contrast. Set the contrast level of WILEye. Wire: `i\f\c`
    pub fn set_contrast(&mut self, contrast: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(contrast);
        let _r = crate::transport::call(91 /* CMD_IO_WIL_EYE_SET_CONTRAST */, &a)?;
        Ok(())
    }

    /// Set Saturation. Set the saturation level of WILEye. Wire: `i\f\i`
    pub fn set_saturation(&mut self, saturation: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(saturation);
        let _r = crate::transport::call(92 /* CMD_IO_WIL_EYE_SET_SATURATION */, &a)?;
        Ok(())
    }

    /// Set Brightness. Set the brightness level of WILEye. Wire: `i\f\b`
    pub fn set_brightness(&mut self, brightness: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(brightness);
        let _r = crate::transport::call(93 /* CMD_IO_WIL_EYE_SET_BRIGHTNESS */, &a)?;
        Ok(())
    }

    /// Set Hue. Set the hue level of WILEye. Wire: `i\f\u`
    pub fn set_hue(&mut self, hue: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(hue);
        let _r = crate::transport::call(94 /* CMD_IO_WIL_EYE_SET_HUE */, &a)?;
        Ok(())
    }

    /// Set Resolution. Set the resolution state of WILEye. Wire: `i\f\y`
    pub fn set_resolution(&mut self, resolutionstate: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(resolutionstate);
        let _r = crate::transport::call(95 /* CMD_IO_WIL_EYE_SET_RESOLUTION */, &a)?;
        Ok(())
    }

    /// Enable Disable Flash. Set the flash state of WILEye. Wire: `i\f\l`
    pub fn set_flash_state(&mut self, flash: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(if flash { 1 } else { 0 });
        let _r = crate::transport::call(96 /* CMD_IO_WIL_EYE_SET_FLASH_STATE */, &a)?;
        Ok(())
    }
}
