//! Apps functions menu - generated from fwMenuApps. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Apps<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Apps<'a> {
    /// Launch App. Switch the built-in display to the app with the given app ID. Wire: `a\a`
    pub fn launch_app(&mut self, app_id: i32) -> Result<(), OwError> {
        let mut cmd = String::from("a\\a");
        encoding::push_int(&mut cmd, app_id as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Run App. Runs /apps/<filename> on the display processor. The destination is inferred by reading the image, not the name: a UF2 whose blocks target SRAM is staged in RAM and launched; one targeting the PSRAM window (0x11000000) is staged into PSRAM through the loader stub and launched; anything else is written to flash. RAM and PSRAM launches leave flash untouched. A flash load takes 30-60 seconds with the screen blank.. Wire: `a\r`
    pub fn run_app(&mut self, filename: &str) -> Result<(), OwError> {
        let mut cmd = String::from("a\\r");
        encoding::push_str(&mut cmd, filename);
        self.t.call(&cmd)?;
        Ok(())
    }
}
