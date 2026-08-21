//! IR Functions menu - generated from fwMenuIR. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Ir<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Ir<'a> {
    /// Stream IR. Enables or disables streaming of received IR codes to the host.. Wire: `w\i\o`
    pub fn enable_ir_stream(&mut self, enable: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        let _r = crate::transport::call(351 /* CMD_WIRELESS_IR_ENABLE_IR_STREAM */, &a)?;
        Ok(())
    }

    /// Send IR. Transmits a 4-byte IR code.. Wire: `w\i\a`
    pub fn send_ir_data(&mut self, ir_code: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(ir_code);
        let _r = crate::transport::call(352 /* CMD_WIRELESS_IR_SEND_IR_DATA */, &a)?;
        Ok(())
    }

    /// IR Self Test. Transmits one frame per supported protocol and checks that the on-board receiver decodes each one back. Takes a few seconds and emits infrared.. Wire: `w\i\t`
    pub fn ir_self_test(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(478 /* CMD_WIRELESS_IR_IR_SELF_TEST */, &a)?;
        Ok(())
    }

    /// List IR Dir. Lists the directories and .ir files on the SD card, directories first. Empty path lists \ir\.. Wire: `w\i\l`
    pub fn ir_list_dir(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(479 /* CMD_WIRELESS_IR_IR_LIST_DIR */, &a)?;
        Ok(())
    }

    /// List IR Buttons. Lists the buttons in one Flipper .ir file with the index each one is sent by. Malformed entries are counted as skipped, not listed.. Wire: `w\i\b`
    pub fn ir_list_buttons(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(480 /* CMD_WIRELESS_IR_IR_LIST_BUTTONS */, &a)?;
        Ok(())
    }

    /// Send IR Button. Transmits one button from a .ir file, repeated by the IR Repeat setting. Emits infrared.. Wire: `w\i\s`
    pub fn ir_send_button(&mut self, index: i32, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.str(path);
        let _r = crate::transport::call(481 /* CMD_WIRELESS_IR_IR_SEND_BUTTON */, &a)?;
        Ok(())
    }

    /// Save IR Capture. Appends the last received signal to \ir\learned.ir under this name, decoded when the protocol was recognised and as raw timings when it was not.. Wire: `w\i\c`
    pub fn ir_save_capture(&mut self, name: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(name);
        let _r = crate::transport::call(482 /* CMD_WIRELESS_IR_IR_SAVE_CAPTURE */, &a)?;
        Ok(())
    }

    /// IR Status. Reports the IR engine's carrier, repeat count, capture overruns and whether the \ir\ tree exists on the card.. Wire: `w\i\i`
    pub fn ir_status(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(483 /* CMD_WIRELESS_IR_IR_STATUS */, &a)?;
        Ok(())
    }

    /// IR Carrier. Default transmit carrier frequency. Only these four are legal; a .ir raw entry with its own frequency line overrides this for that entry.. Wire: `w\i\f`
    pub fn i_r_carrier(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(484 /* CMD_WIRELESS_IR_I_R_CARRIER */, &a)?;
        Ok(())
    }

    /// IR Repeat. How many times Send IR Button transmits each frame, 1 to 5, with a 40 ms gap between repeats.. Wire: `w\i\r`
    pub fn i_r_repeat(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(485 /* CMD_WIRELESS_IR_I_R_REPEAT */, &a)?;
        Ok(())
    }
}
