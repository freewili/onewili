//! GUI Controls menu - generated from fwMenuGUIControls. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct GuiControls<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> GuiControls<'a> {
    /// Add LED. Add a LED control to the panel.. Wire: `g\b\a`
    pub fn add_led(&mut self, index: i32, x: i32, y: i32, color: i32, size: i32, inital_value: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(color);
        a.i32(size);
        a.u8(if inital_value { 1 } else { 0 });
        let _r = crate::transport::call(114 /* CMD_GUI_CONTROLS_ADD_LED */, &a)?;
        Ok(())
    }

    /// Add LogList. Adds a Log control or a list control to the panel.. Wire: `g\b\b`
    pub fn add_log_list(&mut self, index: i32, log: i32, x: i32, y: i32, width: i32, height: i32, font_type: i32, font_size: i32, back_color: &str, fore_color: &str, list_mode: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(log);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(height);
        a.i32(font_type);
        a.i32(font_size);
        a.str(back_color);
        a.str(fore_color);
        a.u8(if list_mode { 1 } else { 0 });
        let _r = crate::transport::call(115 /* CMD_GUI_CONTROLS_ADD_LOG_LIST */, &a)?;
        Ok(())
    }

    /// Add Plot. Adds a plot to the panel.. Wire: `g\b\c`
    pub fn add_plot(&mut self, index: i32, plot_data_index_bit_field: i32, x: i32, y: i32, width: i32, height: i32, min_y: i32, max_y: i32, back_color: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(plot_data_index_bit_field);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(height);
        a.i32(min_y);
        a.i32(max_y);
        a.str(back_color);
        let _r = crate::transport::call(116 /* CMD_GUI_CONTROLS_ADD_PLOT */, &a)?;
        Ok(())
    }

    /// Add Number. add a numeric control to a panel. Wire: `g\b\l`
    pub fn add_number(&mut self, index: i32, x: i32, y: i32, width: i32, font_type: i32, font_size: i32, fore_color: &str, back_color: &str, is_float: bool, float_digit_count: i32, is_hex_format: bool, is_unsigned: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(font_type);
        a.i32(font_size);
        a.str(fore_color);
        a.str(back_color);
        a.u8(if is_float { 1 } else { 0 });
        a.i32(float_digit_count);
        a.u8(if is_hex_format { 1 } else { 0 });
        a.u8(if is_unsigned { 1 } else { 0 });
        let _r = crate::transport::call(117 /* CMD_GUI_CONTROLS_ADD_NUMBER */, &a)?;
        Ok(())
    }

    /// Add Text. Add static text to the panel. Wire: `g\b\e`
    pub fn add_text(&mut self, index: i32, x: i32, y: i32, font_type: i32, font_size: i32, fore_color: &str, back_color: &str, text: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(font_type);
        a.i32(font_size);
        a.str(fore_color);
        a.str(back_color);
        a.str(text);
        let _r = crate::transport::call(118 /* CMD_GUI_CONTROLS_ADD_TEXT */, &a)?;
        Ok(())
    }

    /// Add Bargraph. Add a bar graph to a panel.. Wire: `g\b\f`
    pub fn add_bargraph(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, min: i32, max: i32, bar_color: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(height);
        a.i32(min);
        a.i32(max);
        a.str(bar_color);
        let _r = crate::transport::call(119 /* CMD_GUI_CONTROLS_ADD_BARGRAPH */, &a)?;
        Ok(())
    }

    /// Add Meter. Add a Meter control to a panel. Wire: `g\b\g`
    pub fn add_meter(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, min: i32, max: i32, needle_color: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(height);
        a.i32(min);
        a.i32(max);
        a.str(needle_color);
        let _r = crate::transport::call(120 /* CMD_GUI_CONTROLS_ADD_METER */, &a)?;
        Ok(())
    }

    /// Add Button. Add a button control to a panel. Wire: `g\b\i`
    pub fn add_button(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, fore_color: &str, back_color: &str, text: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(height);
        a.str(fore_color);
        a.str(back_color);
        a.str(text);
        let _r = crate::transport::call(121 /* CMD_GUI_CONTROLS_ADD_BUTTON */, &a)?;
        Ok(())
    }

    /// Add Picture. Shows a ROM picture on the panel.. Wire: `g\b\j`
    pub fn add_picture(&mut self, index: i32, x: i32, y: i32, picture_id: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(picture_id);
        let _r = crate::transport::call(122 /* CMD_GUI_CONTROLS_ADD_PICTURE */, &a)?;
        Ok(())
    }

    /// Add Picture From File. Loads a picture from the file system. Wire: `g\b\k`
    pub fn add_picture_from_file(&mut self, index: i32, x: i32, y: i32, picture_path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.str(picture_path);
        let _r = crate::transport::call(123 /* CMD_GUI_CONTROLS_ADD_PICTURE_FROM_FILE */, &a)?;
        Ok(())
    }

    /// Add Waterfall. Adds an FFT waterfall (spectrogram) control to the panel. Rows commit when the control value changes.. Wire: `g\b\m`
    pub fn add_waterfall(&mut self, index: i32, plot_data_index: i32, bin_count: i32, x: i32, y: i32, width: i32, height: i32, back_color: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(plot_data_index);
        a.i32(bin_count);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(height);
        a.str(back_color);
        let _r = crate::transport::call(124 /* CMD_GUI_CONTROLS_ADD_WATERFALL */, &a)?;
        Ok(())
    }

    /// Add Wili8. Adds a clipped, integer-scaled Wili8 canvas control. Animation 0 is Wave; 255 stores ScriptPath for future custom execution.. Wire: `g\b\n`
    pub fn add_wili8(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, scale: i32, back_color: &str, animation: i32, script_path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(height);
        a.i32(scale);
        a.str(back_color);
        a.i32(animation);
        a.str(script_path);
        let _r = crate::transport::call(383 /* CMD_GUI_CONTROLS_ADD_WILI8 */, &a)?;
        Ok(())
    }

    /// Add File List. Adds a device-fed SD/flash file browser control. Activating a file (or OK in pick dir mode) raises a filepicked event with the full path.. Wire: `g\b\o`
    pub fn add_file_list(&mut self, index: i32, x: i32, y: i32, width: i32, height: i32, mode: i32, back_color: &str, start_path: &str, filter: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(x);
        a.i32(y);
        a.i32(width);
        a.i32(height);
        a.i32(mode);
        a.str(back_color);
        a.str(start_path);
        a.str(filter);
        let _r = crate::transport::call(384 /* CMD_GUI_CONTROLS_ADD_FILE_LIST */, &a)?;
        Ok(())
    }
}
