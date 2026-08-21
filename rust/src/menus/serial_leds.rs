//! Serial LEDs menu - generated from fwMenuSerialLEDs. Do not edit.

use crate::encoding;
use crate::enums::owLEDLightShow;
use crate::enums::owSerialLEDType;
use crate::transport::{OwError, Transport};

pub struct SerialLeds<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> SerialLeds<'a> {
    /// Configure Strip. Configure one of 8 serial LED strips: 0-based strip index, external GPIO (0=disabled; valid: 8-17,25,26,27), LED count (1-1024), LED type (rgb=3-byte WS2812, rgbw=4-byte SK6812), inverted polarity flag. Wire: `i\l\c`
    pub fn configure_strip(&mut self, strip: i32, gpio: i32, length: i32, led_type: owSerialLEDType, inverted: bool) -> Result<(), OwError> {
        let mut cmd = String::from("i\\l\\c");
        encoding::push_int(&mut cmd, strip as i64);
        encoding::push_int(&mut cmd, gpio as i64);
        encoding::push_int(&mut cmd, length as i64);
        encoding::push_int(&mut cmd, led_type.0);
        encoding::push_bool(&mut cmd, inverted);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Show Config. Prints the configuration of all 8 serial LED strips and PSRAM buffer availability. Wire: `i\l\s`
    pub fn show_config(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\l\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set LEDs. Sets a run of LEDs on a strip to an RGB(W) value: strip 0-7, start index, repeat count, then red/green/blue/white 0-255 (white ignored on 3-byte strips). Wire: `i\l\v`
    pub fn set_leds(&mut self, strip: i32, start: i32, count: i32, red: i32, green: i32, blue: i32, white: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\l\\v");
        encoding::push_int(&mut cmd, strip as i64);
        encoding::push_int(&mut cmd, start as i64);
        encoding::push_int(&mut cmd, count as i64);
        encoding::push_int(&mut cmd, red as i64);
        encoding::push_int(&mut cmd, green as i64);
        encoding::push_int(&mut cmd, blue as i64);
        encoding::push_int(&mut cmd, white as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Show. Runs a light show pattern on one strip (0-7) or all strips (-1). Wire: `i\l\w`
    pub fn set_show(&mut self, strip: i32, show: owLEDLightShow) -> Result<(), OwError> {
        let mut cmd = String::from("i\\l\\w");
        encoding::push_int(&mut cmd, strip as i64);
        encoding::push_int(&mut cmd, show.0);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Enable Jambu Orca. Configures strips 1..N for the Jambu Orca 8-channel LED breakout (GPIOs 13,14,11,15,26,25,9,10). Wire: `i\l\j`
    pub fn enable_jambu_orca(&mut self, num_strips: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\l\\j");
        encoding::push_int(&mut cmd, num_strips as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Auto Show. Automatically run the light show selected in the Light Show app on all serial LED strips. Wire: `i\l\a`
    pub fn auto_show(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\l\\a");
        self.t.call(&cmd)?;
        Ok(())
    }
}
