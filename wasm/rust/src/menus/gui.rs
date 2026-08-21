//! GUI Functions menu - generated from fwMenuGUI. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::enums::owButtonPressType;
use crate::enums::owGUIButton;
use crate::enums::owLEDManagerLEDMode;
use crate::enums::owScreenshotFileType;
use crate::transport::OwError;

pub struct Gui<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Gui<'a> {
    /// GUI Panels sub-menu.
    pub fn panels(self) -> super::gui_panels::GuiPanels<'a> {
        super::gui_panels::GuiPanels { t: self.t }
    }

    /// GUI Controls sub-menu.
    pub fn controls(self) -> super::gui_controls::GuiControls<'a> {
        super::gui_controls::GuiControls { t: self.t }
    }

    /// GUI Control Properties sub-menu.
    pub fn control_properties(self) -> super::gui_control_properties::GuiControlProperties<'a> {
        super::gui_control_properties::GuiControlProperties { t: self.t }
    }

    /// Dialogs sub-menu.
    pub fn dialogs(self) -> super::gui_dialogs::GuiDialogs<'a> {
        super::gui_dialogs::GuiDialogs { t: self.t }
    }

    /// Set Board LED. Sets a led to a specific color. Wire: `g\s`
    pub fn set_led_color(&mut self, ledindex: i32, red: i32, green: i32, blue: i32, duration: i32, mode: owLEDManagerLEDMode) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(ledindex);
        a.i32(red);
        a.i32(green);
        a.i32(blue);
        a.i32(duration);
        a.i32(mode.0 as i32);
        let _r = crate::transport::call(104 /* CMD_GUI_SET_LED_COLOR */, &a)?;
        Ok(())
    }

    /// Show FWI Image. Shows an freewili image (fwi) file from the file system.. Wire: `g\l`
    pub fn show_fwi_image(&mut self, filename: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filename);
        let _r = crate::transport::call(105 /* CMD_GUI_SHOW_FWI_IMAGE */, &a)?;
        Ok(())
    }

    /// Reset Display. Clears any GUI menu actions done to the display such as show image or show text. Wire: `g\t`
    pub fn clear_display(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(106 /* CMD_GUI_CLEAR_DISPLAY */, &a)?;
        Ok(())
    }

    /// Show Text Display. Show text on the free wili display. Wire: `g\p`
    pub fn show_text(&mut self, texttodisplay: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(texttodisplay);
        let _r = crate::transport::call(107 /* CMD_GUI_SHOW_TEXT */, &a)?;
        Ok(())
    }

    /// Read All Buttons. Sets the baud rate for I2C in Hz. Wire: `g\u`
    pub fn read_all(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(108 /* CMD_GUI_READ_ALL */, &a)?;
        Ok(())
    }

    /// Stream Buttons. Sends GPIO values as a specific rate to host. Wire: `g\o`
    pub fn stream_io(&mut self, pin: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(pin);
        let _r = crate::transport::call(109 /* CMD_GUI_STREAM_IO */, &a)?;
        Ok(())
    }

    /// Show Asset Image. Reads the number from the address. Wire: `g\a`
    pub fn show_image_asset_by_id(&mut self, image_id: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(image_id);
        let _r = crate::transport::call(110 /* CMD_GUI_SHOW_IMAGE_ASSET_BY_ID */, &a)?;
        Ok(())
    }

    /// Screenshot. Saves the display to SD. Args: filename, png/fwi, counter 0/1, timestamp 0/1.. Wire: `g\i`
    pub fn screenshot(&mut self, filename: &str, filetype: owScreenshotFileType, counter: bool, timestamp: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filename);
        a.i32(filetype.0 as i32);
        a.u8(if counter { 1 } else { 0 });
        a.u8(if timestamp { 1 } else { 0 });
        let _r = crate::transport::call(439 /* CMD_GUI_SCREENSHOT */, &a)?;
        Ok(())
    }

    /// Simulate Keypress. Injects a button action into the DISPLAY GUI.. Wire: `g\k`
    pub fn simulate_keypress(&mut self, button: owGUIButton, presstype: owButtonPressType) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(button.0 as i32);
        a.i32(presstype.0 as i32);
        let _r = crate::transport::call(440 /* CMD_GUI_SIMULATE_KEYPRESS */, &a)?;
        Ok(())
    }

    /// Reinit LCD Panel. Tells DISPLAY to re-initialize its ST7796 LCD panel without rebooting. Arg: mode 0-2.. Wire: `g\r`
    pub fn reinit_lcd_panel(&mut self, mode: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(mode);
        let _r = crate::transport::call(539 /* CMD_GUI_REINIT_LCD_PANEL */, &a)?;
        Ok(())
    }
}
