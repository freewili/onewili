//! niceusb menu - generated from fwMenuNiceUsb. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct NiceUsb<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> NiceUsb<'a> {
    /// Start Gadget. Attaches the USB HID gadget on the second USB port, so the host sees a keyboard, mouse and gamepad. Returns as soon as the display accepts the request; the host takes a moment longer to enumerate. Refused while SubGHz is in use.. Wire: `i\n\s`
    pub fn nice_usb_start(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(493 /* CMD_IO_NICE_USB_NICE_USB_START */, &a)?;
        Ok(())
    }

    /// Stop Gadget. Detaches the USB HID gadget, gives the display screen back and releases the SubGHz lockout. A stop asked for while a script is running takes effect when that script finishes.. Wire: `i\n\t`
    pub fn nice_usb_stop(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(494 /* CMD_IO_NICE_USB_NICE_USB_STOP */, &a)?;
        Ok(())
    }

    /// niceusb Status. Reports the gadget state as the display holds it, whether a script is running, whether SubGHz is locked out, and the last script error.. Wire: `i\n\i`
    pub fn nice_usb_status(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(495 /* CMD_IO_NICE_USB_NICE_USB_STATUS */, &a)?;
        Ok(())
    }

    /// Run Test Script. Types a two-line proof script on the host, three seconds apart. The gadget must already be attached. Types into whatever window has focus on the host.. Wire: `i\n\r`
    pub fn nice_usb_run_test_script(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(496 /* CMD_IO_NICE_USB_NICE_USB_RUN_TEST_SCRIPT */, &a)?;
        Ok(())
    }

    /// niceusb Script. SD path of the wusb script Start Gadget loads, or empty for the gadget's built-in personality.. Wire: `i\n\c`
    pub fn niceusb_script(&mut self, value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(value);
        let _r = crate::transport::call(497 /* CMD_IO_NICE_USB_NICEUSB_SCRIPT */, &a)?;
        Ok(())
    }

    /// niceusb Force VID/PID. Enable to make Start Gadget present the VID/PID below instead of the script's own or the built-in personality's.. Wire: `i\n\o`
    pub fn niceusb_force_vidpid(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(498 /* CMD_IO_NICE_USB_NICEUSB_FORCE_VIDPID */, &a)?;
        Ok(())
    }

    /// niceusb VID. Forced USB vendor ID, used only while niceusb Force VID/PID is on.. Wire: `i\n\v`
    pub fn niceusb_vid(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(499 /* CMD_IO_NICE_USB_NICEUSB_VID */, &a)?;
        Ok(())
    }

    /// niceusb PID. Forced USB product ID, used only while niceusb Force VID/PID is on.. Wire: `i\n\p`
    pub fn niceusb_pid(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(500 /* CMD_IO_NICE_USB_NICEUSB_PID */, &a)?;
        Ok(())
    }
}
