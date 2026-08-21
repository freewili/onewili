//! Dialogs menu - generated from fwMenuGUIDialogs. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct GuiDialogs<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> GuiDialogs<'a> {
    /// Message Box. Shows a message box with optional buttons and auto close timer. . Wire: `g\f\a`
    pub fn message_box(&mut self, auto_close_half_sec: i32, show_ok: bool, show_ok_cancel: bool, show_none: bool, picture_index: i32, message: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(auto_close_half_sec);
        a.u8(if show_ok { 1 } else { 0 });
        a.u8(if show_ok_cancel { 1 } else { 0 });
        a.u8(if show_none { 1 } else { 0 });
        a.i32(picture_index);
        a.str(message);
        let _r = crate::transport::call(135 /* CMD_GUI_DIALOGS_MESSAGE_BOX */, &a)?;
        Ok(())
    }

    /// Set Dialog Description. Sets the description of the dialog.. Wire: `g\f\b`
    pub fn set_dialog_description(&mut self, description: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(description);
        let _r = crate::transport::call(136 /* CMD_GUI_DIALOGS_SET_DIALOG_DESCRIPTION */, &a)?;
        Ok(())
    }

    /// Progress Bar. shows a dialog with a progress bar. Wire: `g\f\c`
    pub fn progress_bar(&mut self, picture_index: i32, ok_to_close: bool, auto_close_at100: bool, auto_close_half_sec: i32, title: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(picture_index);
        a.u8(if ok_to_close { 1 } else { 0 });
        a.u8(if auto_close_at100 { 1 } else { 0 });
        a.i32(auto_close_half_sec);
        a.str(title);
        let _r = crate::transport::call(137 /* CMD_GUI_DIALOGS_PROGRESS_BAR */, &a)?;
        Ok(())
    }

    /// Number Edit. Shows a dialog box to edit numbers. Wire: `g\f\k`
    pub fn number_edit(&mut self, min: i32, max: i32, initial: i32, use_min_max: bool, is_unsigned: bool, hex_fomat: bool, message: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(min);
        a.i32(max);
        a.i32(initial);
        a.u8(if use_min_max { 1 } else { 0 });
        a.u8(if is_unsigned { 1 } else { 0 });
        a.u8(if hex_fomat { 1 } else { 0 });
        a.str(message);
        let _r = crate::transport::call(138 /* CMD_GUI_DIALOGS_NUMBER_EDIT */, &a)?;
        Ok(())
    }

    /// Number Edit Float. Shows a dialog to enter a float number. Wire: `g\f\e`
    pub fn number_edit_float(&mut self, min: f64, max: f64, initial: f64, use_min_max: bool, digit_count: i32, message: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.f32(min as f32);
        a.f32(max as f32);
        a.f32(initial as f32);
        a.u8(if use_min_max { 1 } else { 0 });
        a.i32(digit_count);
        a.str(message);
        let _r = crate::transport::call(139 /* CMD_GUI_DIALOGS_NUMBER_EDIT_FLOAT */, &a)?;
        Ok(())
    }

    /// Text Edit. Shows a dialog to edit a text value.. Wire: `g\f\f`
    pub fn text_edit(&mut self, message: &str, inital_value: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(message);
        a.str(inital_value);
        let _r = crate::transport::call(140 /* CMD_GUI_DIALOGS_TEXT_EDIT */, &a)?;
        Ok(())
    }

    /// Pick List. Shows a list of items to pick from. The list of items is loaded into a log.. Wire: `g\f\g`
    pub fn pick_list(&mut self, log_index: i32, message: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(log_index);
        a.str(message);
        let _r = crate::transport::call(141 /* CMD_GUI_DIALOGS_PICK_LIST */, &a)?;
        Ok(())
    }

    /// Show Text Editor. Shows a full screen text editor.. Wire: `g\f\i`
    pub fn show_text_editor(&mut self, editor_type: i32, message: &str, inital_value: &str) -> Result<bool, OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(editor_type);
        a.str(message);
        a.str(inital_value);
        let mut _r = crate::transport::call(142 /* CMD_GUI_DIALOGS_SHOW_TEXT_EDITOR */, &a)?;
        Ok(_r.u8() != 0)
    }

    /// Set Progess Dialog Value. Sets the value of progress on the dialog. Wire: `g\f\j`
    pub fn set_progess_dialog_value(&mut self, value0_to100: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value0_to100);
        let _r = crate::transport::call(143 /* CMD_GUI_DIALOGS_SET_PROGESS_DIALOG_VALUE */, &a)?;
        Ok(())
    }

    /// File Picker. Shows a full screen file browser dialog over the current view. The chosen path (or cancel) returns as a filepicked event.. Wire: `g\f\l`
    pub fn file_picker(&mut self, mode: i32, start_path: &str, filter: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(mode);
        a.str(start_path);
        a.str(filter);
        let _r = crate::transport::call(385 /* CMD_GUI_DIALOGS_FILE_PICKER */, &a)?;
        Ok(())
    }
}
