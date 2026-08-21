//! Serial LEDs menu - generated from fwMenuSerialLEDs. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::enums::owLEDLightShow;
use crate::enums::owSerialLEDType;
use crate::transport::OwError;

pub struct SerialLeds<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> SerialLeds<'a> {
    /// Configure Strip. Configure one of 8 serial LED strips: 0-based strip index, external GPIO (0=disabled; valid: 8-17,25,26,27), LED count (1-1024), LED type (rgb=3-byte WS2812, rgbw=4-byte SK6812), inverted polarity flag. Wire: `i\l\c`
    pub fn configure_strip(&mut self, strip: i32, gpio: i32, length: i32, led_type: owSerialLEDType, inverted: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(strip);
        a.i32(gpio);
        a.i32(length);
        a.i32(led_type.0 as i32);
        a.u8(if inverted { 1 } else { 0 });
        let _r = crate::transport::call(387 /* CMD_IO_SERIAL_LEDS_CONFIGURE_STRIP */, &a)?;
        Ok(())
    }

    /// Show Config. Prints the configuration of all 8 serial LED strips and PSRAM buffer availability. Wire: `i\l\s`
    pub fn show_config(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(388 /* CMD_IO_SERIAL_LEDS_SHOW_CONFIG */, &a)?;
        Ok(())
    }

    /// Set LEDs. Sets a run of LEDs on a strip to an RGB(W) value: strip 0-7, start index, repeat count, then red/green/blue/white 0-255 (white ignored on 3-byte strips). Wire: `i\l\v`
    pub fn set_leds(&mut self, strip: i32, start: i32, count: i32, red: i32, green: i32, blue: i32, white: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(strip);
        a.i32(start);
        a.i32(count);
        a.i32(red);
        a.i32(green);
        a.i32(blue);
        a.i32(white);
        let _r = crate::transport::call(389 /* CMD_IO_SERIAL_LEDS_SET_LEDS */, &a)?;
        Ok(())
    }

    /// Set Show. Runs a light show pattern on one strip (0-7) or all strips (-1). Wire: `i\l\w`
    pub fn set_show(&mut self, strip: i32, show: owLEDLightShow) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(strip);
        a.i32(show.0 as i32);
        let _r = crate::transport::call(390 /* CMD_IO_SERIAL_LEDS_SET_SHOW */, &a)?;
        Ok(())
    }

    /// Enable Jambu Orca. Configures strips 1..N for the Jambu Orca 8-channel LED breakout (GPIOs 13,14,11,15,26,25,9,10). Wire: `i\l\j`
    pub fn enable_jambu_orca(&mut self, num_strips: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(num_strips);
        let _r = crate::transport::call(391 /* CMD_IO_SERIAL_LEDS_ENABLE_JAMBU_ORCA */, &a)?;
        Ok(())
    }

    /// Auto Show. Automatically run the light show selected in the Light Show app on all serial LED strips. Wire: `i\l\a`
    pub fn auto_show(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(392 /* CMD_IO_SERIAL_LEDS_AUTO_SHOW */, &a)?;
        Ok(())
    }
}
