//! GUI Controls menu - generated from fwMenuGUIControls. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct GuiControls<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> GuiControls<'a> {
    /// Add LED. Add a LED control to the panel.. Wire: `g\b\a`
    pub fn add_led(&mut self, index: i32, x: i32, y: i32, color: i32, size: i32, inital_value: bool) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\a");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, color as i64);
        encoding::push_int(&mut cmd, size as i64);
        encoding::push_bool(&mut cmd, inital_value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add LogList. Adds a Log control or a list control to the panel.. Wire: `g\b\b`
    pub fn add_log_list(&mut self, index: i32, log: i32, x: i32, y: i32, width: i32, height: i32, font_type: i32, font_size: i32, back_color: &str, fore_color: &str, list_mode: bool) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\b");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, log as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, height as i64);
        encoding::push_int(&mut cmd, font_type as i64);
        encoding::push_int(&mut cmd, font_size as i64);
        encoding::push_str(&mut cmd, back_color);
        encoding::push_str(&mut cmd, fore_color);
        encoding::push_bool(&mut cmd, list_mode);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Plot. Adds a plot to the panel.. Wire: `g\b\c`
    pub fn add_plot(&mut self, index: i32, plot_data_index_bit_field: i32, x: i32, y: i32, width: i32, height: i32, min_y: i32, max_y: i32, back_color: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\c");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, plot_data_index_bit_field as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, height as i64);
        encoding::push_int(&mut cmd, min_y as i64);
        encoding::push_int(&mut cmd, max_y as i64);
        encoding::push_str(&mut cmd, back_color);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Number. add a numeric control to a panel. Wire: `g\b\l`
    pub fn add_number(&mut self, index: i32, x: i32, y: i32, width: i32, font_type: i32, font_size: i32, fore_color: &str, back_color: &str, is_float: bool, float_digit_count: i32, is_hex_format: bool, is_unsigned: bool) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\l");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, font_type as i64);
        encoding::push_int(&mut cmd, font_size as i64);
        encoding::push_str(&mut cmd, fore_color);
        encoding::push_str(&mut cmd, back_color);
        encoding::push_bool(&mut cmd, is_float);
        encoding::push_int(&mut cmd, float_digit_count as i64);
        encoding::push_bool(&mut cmd, is_hex_format);
        encoding::push_bool(&mut cmd, is_unsigned);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Text. Add static text to the panel. Wire: `g\b\e`
    pub fn add_text(&mut self, index: i32, x: i32, y: i32, font_type: i32, font_size: i32, fore_color: &str, back_color: &str, text: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\e");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, font_type as i64);
        encoding::push_int(&mut cmd, font_size as i64);
        encoding::push_str(&mut cmd, fore_color);
        encoding::push_str(&mut cmd, back_color);
        encoding::push_str(&mut cmd, text);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Bargraph. Add a bar graph to a panel.. Wire: `g\b\f`
    pub fn add_bargraph(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, min: i32, max: i32, bar_color: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\f");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, height as i64);
        encoding::push_int(&mut cmd, min as i64);
        encoding::push_int(&mut cmd, max as i64);
        encoding::push_str(&mut cmd, bar_color);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Meter. Add a Meter control to a panel. Wire: `g\b\g`
    pub fn add_meter(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, min: i32, max: i32, needle_color: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\g");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, height as i64);
        encoding::push_int(&mut cmd, min as i64);
        encoding::push_int(&mut cmd, max as i64);
        encoding::push_str(&mut cmd, needle_color);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Button. Add a button control to a panel. Wire: `g\b\i`
    pub fn add_button(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, fore_color: &str, back_color: &str, text: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\i");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, height as i64);
        encoding::push_str(&mut cmd, fore_color);
        encoding::push_str(&mut cmd, back_color);
        encoding::push_str(&mut cmd, text);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Picture. Shows a ROM picture on the panel.. Wire: `g\b\j`
    pub fn add_picture(&mut self, index: i32, x: i32, y: i32, picture_id: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\j");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, picture_id as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Picture From File. Loads a picture from the file system. Wire: `g\b\k`
    pub fn add_picture_from_file(&mut self, index: i32, x: i32, y: i32, picture_path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\k");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_str(&mut cmd, picture_path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Waterfall. Adds an FFT waterfall (spectrogram) control to the panel. Rows commit when the control value changes.. Wire: `g\b\m`
    pub fn add_waterfall(&mut self, index: i32, plot_data_index: i32, bin_count: i32, x: i32, y: i32, width: i32, height: i32, back_color: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\m");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, plot_data_index as i64);
        encoding::push_int(&mut cmd, bin_count as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, height as i64);
        encoding::push_str(&mut cmd, back_color);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Wili8. Adds a clipped, integer-scaled Wili8 canvas control. Animation 0 is Wave; 255 stores ScriptPath for future custom execution.. Wire: `g\b\n`
    pub fn add_wili8(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, scale: i32, back_color: &str, animation: i32, script_path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\n");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, height as i64);
        encoding::push_int(&mut cmd, scale as i64);
        encoding::push_str(&mut cmd, back_color);
        encoding::push_int(&mut cmd, animation as i64);
        encoding::push_str(&mut cmd, script_path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add File List. Adds a device-fed SD/flash file browser control. Activating a file (or OK in pick dir mode) raises a filepicked event with the full path.. Wire: `g\b\o`
    pub fn add_file_list(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, mode: i32, back_color: &str, start_path: &str, filter: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\b\\o");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, x as i64);
        encoding::push_int(&mut cmd, y as i64);
        encoding::push_int(&mut cmd, width as i64);
        encoding::push_int(&mut cmd, height as i64);
        encoding::push_int(&mut cmd, mode as i64);
        encoding::push_str(&mut cmd, back_color);
        encoding::push_str(&mut cmd, start_path);
        encoding::push_str(&mut cmd, filter);
        self.t.call(&cmd)?;
        Ok(())
    }
}
