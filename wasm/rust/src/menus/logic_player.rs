//! Logic Player Functions menu - generated from fwMenuLogicPlayer. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct LogicPlayer<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> LogicPlayer<'a> {
    /// configure. Configures digital playback. Wire: `i\p\c`
    pub fn setup_player(&mut self, sample_rate_ns: i32, sample_count: i32, pin_start: i32, pin_stop: i32, start_mode: i32, trigger_pin: i32, loop_: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(sample_rate_ns);
        a.i32(sample_count);
        a.i32(pin_start);
        a.i32(pin_stop);
        a.i32(start_mode);
        a.i32(trigger_pin);
        a.u8(if loop_ { 1 } else { 0 });
        let _r = crate::transport::call(76 /* CMD_IO_LOGIC_PLAYER_SETUP_PLAYER */, &a)?;
        Ok(())
    }

    /// configure analog. Configures DAC playback. Wire: `i\p\a`
    pub fn setup_analog(&mut self, mask: i32, analog_rate_ns: i32, analog_resolution: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(mask);
        a.i32(analog_rate_ns);
        a.i32(analog_resolution);
        let _r = crate::transport::call(77 /* CMD_IO_LOGIC_PLAYER_SETUP_ANALOG */, &a)?;
        Ok(())
    }

    /// load. Loads a raw buffer from the filesystem. Wire: `i\p\l`
    pub fn load_file(&mut self, file_path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(file_path);
        let _r = crate::transport::call(78 /* CMD_IO_LOGIC_PLAYER_LOAD_FILE */, &a)?;
        Ok(())
    }

    /// start. Starts playback. Wire: `i\p\s`
    pub fn start(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(79 /* CMD_IO_LOGIC_PLAYER_START */, &a)?;
        Ok(())
    }

    /// stop. Stops playback. Wire: `i\p\e`
    pub fn stop(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(80 /* CMD_IO_LOGIC_PLAYER_STOP */, &a)?;
        Ok(())
    }
}
