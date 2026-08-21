//! Dialogs menu - generated from fwMenuGUIDialogs. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct GuiDialogs<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> GuiDialogs<'a> {
    /// Message Box. Shows a message box with optional buttons and auto close timer. . Wire: `g\f\a`
    pub fn message_box(&mut self, auto_close_half_sec: i32, show_ok: bool, show_ok_cancel: bool, show_none: bool, picture_index: i32, message: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\a");
        encoding::push_int(&mut cmd, auto_close_half_sec as i64);
        encoding::push_bool(&mut cmd, show_ok);
        encoding::push_bool(&mut cmd, show_ok_cancel);
        encoding::push_bool(&mut cmd, show_none);
        encoding::push_int(&mut cmd, picture_index as i64);
        encoding::push_str(&mut cmd, message);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Dialog Description. Sets the description of the dialog.. Wire: `g\f\b`
    pub fn set_dialog_description(&mut self, description: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\b");
        encoding::push_str(&mut cmd, description);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Progress Bar. shows a dialog with a progress bar. Wire: `g\f\c`
    pub fn progress_bar(&mut self, picture_index: i32, ok_to_close: bool, auto_close_at100: bool, auto_close_half_sec: i32, title: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\c");
        encoding::push_int(&mut cmd, picture_index as i64);
        encoding::push_bool(&mut cmd, ok_to_close);
        encoding::push_bool(&mut cmd, auto_close_at100);
        encoding::push_int(&mut cmd, auto_close_half_sec as i64);
        encoding::push_str(&mut cmd, title);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Number Edit. Shows a dialog box to edit numbers. Wire: `g\f\k`
    pub fn number_edit(&mut self, min: i32, max: i32, initial: i32, use_min_max: bool, is_unsigned: bool, hex_fomat: bool, message: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\k");
        encoding::push_int(&mut cmd, min as i64);
        encoding::push_int(&mut cmd, max as i64);
        encoding::push_int(&mut cmd, initial as i64);
        encoding::push_bool(&mut cmd, use_min_max);
        encoding::push_bool(&mut cmd, is_unsigned);
        encoding::push_bool(&mut cmd, hex_fomat);
        encoding::push_str(&mut cmd, message);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Number Edit Float. Shows a dialog to enter a float number. Wire: `g\f\e`
    pub fn number_edit_float(&mut self, min: f64, max: f64, initial: f64, use_min_max: bool, digit_count: i32, message: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\e");
        encoding::push_float(&mut cmd, min);
        encoding::push_float(&mut cmd, max);
        encoding::push_float(&mut cmd, initial);
        encoding::push_bool(&mut cmd, use_min_max);
        encoding::push_int(&mut cmd, digit_count as i64);
        encoding::push_str(&mut cmd, message);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Text Edit. Shows a dialog to edit a text value.. Wire: `g\f\f`
    pub fn text_edit(&mut self, message: &str, inital_value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\f");
        encoding::push_str(&mut cmd, message);
        encoding::push_str(&mut cmd, inital_value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Pick List. Shows a list of items to pick from. The list of items is loaded into a log.. Wire: `g\f\g`
    pub fn pick_list(&mut self, log_index: i32, message: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\g");
        encoding::push_int(&mut cmd, log_index as i64);
        encoding::push_str(&mut cmd, message);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Show Text Editor. Shows a full screen text editor.. Wire: `g\f\i`
    pub fn show_text_editor(&mut self, editor_type: i32, message: &str, inital_value: &str) -> Result<bool, OwError> {
        let mut cmd = String::from("g\\f\\i");
        encoding::push_int(&mut cmd, editor_type as i64);
        encoding::push_str(&mut cmd, message);
        encoding::push_str(&mut cmd, inital_value);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let basic = encoding::tok_bool(&mut toks)?;
        Ok(basic)
    }

    /// Set Progess Dialog Value. Sets the value of progress on the dialog. Wire: `g\f\j`
    pub fn set_progess_dialog_value(&mut self, value0_to100: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\j");
        encoding::push_int(&mut cmd, value0_to100 as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// File Picker. Shows a full screen file browser dialog over the current view. The chosen path (or cancel) returns as a filepicked event.. Wire: `g\f\l`
    pub fn file_picker(&mut self, mode: i32, start_path: &str, filter: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\f\\l");
        encoding::push_int(&mut cmd, mode as i64);
        encoding::push_str(&mut cmd, start_path);
        encoding::push_str(&mut cmd, filter);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("filepicked", false, "panel=decS32,control=decS32,picked=bool,path=string", "File list / file picker result"),
];
