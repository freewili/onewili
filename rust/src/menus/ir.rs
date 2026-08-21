//! IR Functions menu - generated from fwMenuIR. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Ir<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Ir<'a> {
    /// Stream IR. Enables or disables streaming of received IR codes to the host.. Wire: `w\i\o`
    pub fn enable_ir_stream(&mut self, enable: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\i\\o");
        encoding::push_int(&mut cmd, enable as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Send IR. Transmits a 4-byte IR code.. Wire: `w\i\a`
    pub fn send_ir_data(&mut self, ir_code: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\i\\a");
        encoding::push_int(&mut cmd, ir_code as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// IR Self Test. Transmits one frame per supported protocol and checks that the on-board receiver decodes each one back. Takes a few seconds and emits infrared.. Wire: `w\i\t`
    pub fn ir_self_test(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\i\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// List IR Dir. Lists the directories and .ir files on the SD card, directories first. Empty path lists \ir\.. Wire: `w\i\l`
    pub fn ir_list_dir(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\i\\l");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// List IR Buttons. Lists the buttons in one Flipper .ir file with the index each one is sent by. Malformed entries are counted as skipped, not listed.. Wire: `w\i\b`
    pub fn ir_list_buttons(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\i\\b");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Send IR Button. Transmits one button from a .ir file, repeated by the IR Repeat setting. Emits infrared.. Wire: `w\i\s`
    pub fn ir_send_button(&mut self, index: i32, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\i\\s");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Save IR Capture. Appends the last received signal to \ir\learned.ir under this name, decoded when the protocol was recognised and as raw timings when it was not.. Wire: `w\i\c`
    pub fn ir_save_capture(&mut self, name: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\i\\c");
        encoding::push_str(&mut cmd, name);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// IR Status. Reports the IR engine's carrier, repeat count, capture overruns and whether the \ir\ tree exists on the card.. Wire: `w\i\i`
    pub fn ir_status(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\i\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// IR Carrier. Default transmit carrier frequency. Only these four are legal; a .ir raw entry with its own frequency line overrides this for that entry.. Wire: `w\i\f`
    pub fn i_r_carrier(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\i\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// IR Repeat. How many times Send IR Button transmits each frame, 1 to 5, with a 40 ms gap between repeats.. Wire: `w\i\r`
    pub fn i_r_repeat(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\i\\r");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("irrx", false, "code=hexU32", "Received IR code"),
];
