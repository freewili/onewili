//! GPIO Functions menu - generated from fwMenuGPIO. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Gpio<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Gpio<'a> {
    /// IO Directions sub-menu.
    pub fn io_direction_settings(self) -> super::io_direction_settings::IoDirectionSettings<'a> {
        super::io_direction_settings::IoDirectionSettings { t: self.t }
    }

    /// High. Sets a GPIO high. Wire: `i\g\s`
    pub fn set_io_high(&mut self, pin: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(pin);
        let _r = crate::transport::call(0 /* CMD_IO_GPIO_SET_IO_HIGH */, &a)?;
        Ok(())
    }

    /// Low. Sets a GPIO low. Wire: `i\g\l`
    pub fn set_io_low(&mut self, pin: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(pin);
        let _r = crate::transport::call(1 /* CMD_IO_GPIO_SET_IO_LOW */, &a)?;
        Ok(())
    }

    /// Toggle. Toggles the specified GPIO. Wire: `i\g\t`
    pub fn set_io_toggle(&mut self, pin: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(pin);
        let _r = crate::transport::call(2 /* CMD_IO_GPIO_SET_IO_TOGGLE */, &a)?;
        Ok(())
    }

    /// PWM IO. Enables the PWM feature of GPIO. Wire: `i\g\p`
    pub fn set_pwm(&mut self, gpio_number: i32, freq: f64, duty: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(gpio_number);
        a.f32(freq as f32);
        a.f32(duty as f32);
        let _r = crate::transport::call(3 /* CMD_IO_GPIO_SET_PWM */, &a)?;
        Ok(())
    }

    /// Get All IOs (hex). Reads all the IOs in a bitfield. Wire: `i\g\u`
    pub fn read_all(&mut self) -> Result<u32, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(4 /* CMD_IO_GPIO_READ_ALL */, &a)?;
        Ok(_r.u32())
    }

    /// Stream IO reads. Sends GPIO values as a specific millisecond rate to host. Wire: `i\g\o`
    pub fn stream_io(&mut self, reportratems: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(reportratems);
        let _r = crate::transport::call(5 /* CMD_IO_GPIO_STREAM_IO */, &a)?;
        Ok(())
    }

    /// Toggle High-Speed Bidirectional IO. Toggle utilizing GPIO27 to set the direction of GPIO26.. Wire: `i\g\e`
    pub fn toggle_hsbdio(&mut self, pin: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(pin);
        let _r = crate::transport::call(6 /* CMD_IO_GPIO_TOGGLE_HSBDIO */, &a)?;
        Ok(())
    }

    /// Set IO Voltage Source. Selects the voltage source connected to the external IO voltage rail.. Wire: `i\g\v`
    pub fn set_io_voltage_source(&mut self, source: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(source);
        let _r = crate::transport::call(394 /* CMD_IO_GPIO_SET_IO_VOLTAGE_SOURCE */, &a)?;
        Ok(())
    }
}
