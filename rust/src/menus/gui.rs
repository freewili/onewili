//! GUI Functions menu - generated from fwMenuGUI. Do not edit.

use crate::encoding;
use crate::enums::owButtonPressType;
use crate::enums::owGUIButton;
use crate::enums::owLEDManagerLEDMode;
use crate::enums::owScreenshotFileType;
use crate::transport::{OwError, Transport};

pub struct Gui<'a> {
    pub(crate) t: &'a mut Transport,
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
        let mut cmd = String::from("g\\s");
        encoding::push_int(&mut cmd, ledindex as i64);
        encoding::push_int(&mut cmd, red as i64);
        encoding::push_int(&mut cmd, green as i64);
        encoding::push_int(&mut cmd, blue as i64);
        encoding::push_int(&mut cmd, duration as i64);
        encoding::push_int(&mut cmd, mode.0);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Show FWI Image. Shows an freewili image (fwi) file from the file system.. Wire: `g\l`
    pub fn show_fwi_image(&mut self, filename: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\l");
        encoding::push_str(&mut cmd, filename);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Reset Display. Clears any GUI menu actions done to the display such as show image or show text. Wire: `g\t`
    pub fn clear_display(&mut self) -> Result<(), OwError> {
        let cmd = String::from("g\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Show Text Display. Show text on the free wili display. Wire: `g\p`
    pub fn show_text(&mut self, texttodisplay: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\p");
        encoding::push_str(&mut cmd, texttodisplay);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read All Buttons. Sets the baud rate for I2C in Hz. Wire: `g\u`
    pub fn read_all(&mut self) -> Result<(), OwError> {
        let cmd = String::from("g\\u");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream Buttons. Sends GPIO values as a specific rate to host. Wire: `g\o`
    pub fn stream_io(&mut self, pin: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\o");
        encoding::push_int(&mut cmd, pin as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Show Asset Image. Reads the number from the address. Wire: `g\a`
    pub fn show_image_asset_by_id(&mut self, image_id: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\a");
        encoding::push_int(&mut cmd, image_id as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Screenshot. Saves the display to SD. Args: filename, png/fwi, counter 0/1, timestamp 0/1.. Wire: `g\i`
    pub fn screenshot(&mut self, filename: &str, filetype: owScreenshotFileType, counter: bool, timestamp: bool) -> Result<(), OwError> {
        let mut cmd = String::from("g\\i");
        encoding::push_str(&mut cmd, filename);
        encoding::push_int(&mut cmd, filetype.0);
        encoding::push_bool(&mut cmd, counter);
        encoding::push_bool(&mut cmd, timestamp);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Simulate Keypress. Injects a button action into the DISPLAY GUI.. Wire: `g\k`
    pub fn simulate_keypress(&mut self, button: owGUIButton, presstype: owButtonPressType) -> Result<(), OwError> {
        let mut cmd = String::from("g\\k");
        encoding::push_int(&mut cmd, button.0);
        encoding::push_int(&mut cmd, presstype.0);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Reinit LCD Panel. Tells DISPLAY to re-initialize its ST7796 LCD panel without rebooting. Arg: mode 0-2.. Wire: `g\r`
    pub fn reinit_lcd_panel(&mut self, mode: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\r");
        encoding::push_int(&mut cmd, mode as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("button", false, "gray=decU32,yellow=decU32,green=decU32,blue=decU32,red=decU32", "Button state report (1 = pressed, per button)"),
];
