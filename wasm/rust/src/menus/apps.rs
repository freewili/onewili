//! Apps functions menu - generated from fwMenuApps. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Apps<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Apps<'a> {
    /// Launch App. Switch the built-in display to the app with the given app ID. Wire: `a\a`
    pub fn launch_app(&mut self, app_id: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(app_id);
        let _r = crate::transport::call(381 /* CMD_APPS_LAUNCH_APP */, &a)?;
        Ok(())
    }

    /// Run App. Runs /apps/<filename> on the display processor. The destination is inferred by reading the image, not the name: a UF2 whose blocks target SRAM is staged in RAM and launched; one targeting the PSRAM window (0x11000000) is staged into PSRAM through the loader stub and launched; anything else is written to flash. RAM and PSRAM launches leave flash untouched. A flash load takes 30-60 seconds with the screen blank.. Wire: `a\r`
    pub fn run_app(&mut self, filename: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filename);
        let _r = crate::transport::call(466 /* CMD_APPS_RUN_APP */, &a)?;
        Ok(())
    }
}
