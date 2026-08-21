//! niceusb menu - generated from fwMenuNiceUsb. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct NiceUsb<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> NiceUsb<'a> {
    /// Start Gadget. Attaches the USB HID gadget on the second USB port, so the host sees a keyboard, mouse and gamepad. Returns as soon as the display accepts the request; the host takes a moment longer to enumerate. Refused while SubGHz is in use.. Wire: `i\n\s`
    pub fn nice_usb_start(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\n\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop Gadget. Detaches the USB HID gadget, gives the display screen back and releases the SubGHz lockout. A stop asked for while a script is running takes effect when that script finishes.. Wire: `i\n\t`
    pub fn nice_usb_stop(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\n\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// niceusb Status. Reports the gadget state as the display holds it, whether a script is running, whether SubGHz is locked out, and the last script error.. Wire: `i\n\i`
    pub fn nice_usb_status(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\n\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Run Test Script. Types a two-line proof script on the host, three seconds apart. The gadget must already be attached. Types into whatever window has focus on the host.. Wire: `i\n\r`
    pub fn nice_usb_run_test_script(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\n\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// niceusb Script. SD path of the wusb script Start Gadget loads, or empty for the gadget's built-in personality.. Wire: `i\n\c`
    pub fn niceusb_script(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\n\\c");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// niceusb Force VID/PID. Enable to make Start Gadget present the VID/PID below instead of the script's own or the built-in personality's.. Wire: `i\n\o`
    pub fn niceusb_force_vidpid(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\n\\o");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// niceusb VID. Forced USB vendor ID, used only while niceusb Force VID/PID is on.. Wire: `i\n\v`
    pub fn niceusb_vid(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\n\\v");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// niceusb PID. Forced USB product ID, used only while niceusb Force VID/PID is on.. Wire: `i\n\p`
    pub fn niceusb_pid(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\n\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
