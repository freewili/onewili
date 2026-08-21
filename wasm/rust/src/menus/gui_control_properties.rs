//! GUI Control Properties menu - generated from fwMenuGUIControlProperties. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct GuiControlProperties<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> GuiControlProperties<'a> {
    /// Set Control Value Text. sets the text value of a control. Wire: `g\e\a`
    pub fn set_control_value_text(&mut self, index: i32, text: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.str(text);
        let _r = crate::transport::call(125 /* CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_TEXT */, &a)?;
        Ok(())
    }

    /// Set Control Value Int. Set the text value of a control. Wire: `g\e\b`
    pub fn set_control_value_int(&mut self, index: i32, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(value);
        let _r = crate::transport::call(126 /* CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_INT */, &a)?;
        Ok(())
    }

    /// Set Control Value Float. Set the float value of the control.. Wire: `g\e\c`
    pub fn set_control_value_float(&mut self, index: i32, value: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.f32(value as f32);
        let _r = crate::transport::call(127 /* CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_FLOAT */, &a)?;
        Ok(())
    }

    /// Set List Item Text. Sets the text and color of a specific list item. Wire: `g\e\k`
    pub fn set_list_item_text(&mut self, log_index: i32, list_item: i32, color: i32, text: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(log_index);
        a.i32(list_item);
        a.i32(color);
        a.str(text);
        let _r = crate::transport::call(128 /* CMD_GUI_CONTROL_PROPERTIES_SET_LIST_ITEM_TEXT */, &a)?;
        Ok(())
    }

    /// Set Control Value Min Max Int. Sets whether a min and max is applied to a controls value. Wire: `g\e\e`
    pub fn set_control_value_min_max_int(&mut self, index: i32, enable: bool, min: i32, max: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.u8(if enable { 1 } else { 0 });
        a.i32(min);
        a.i32(max);
        let _r = crate::transport::call(129 /* CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_MIN_MAX_INT */, &a)?;
        Ok(())
    }

    /// Set Control Value Min Max Float. Sets whether a min and max is applied to a controls value. Wire: `g\e\l`
    pub fn set_control_value_min_max_float(&mut self, index: i32, enable: bool, min: f64, max: f64) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.u8(if enable { 1 } else { 0 });
        a.f32(min as f32);
        a.f32(max as f32);
        let _r = crate::transport::call(130 /* CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_MIN_MAX_FLOAT */, &a)?;
        Ok(())
    }

    /// Set Plot Data. This adds data to a plot. Wire: `g\e\f`
    pub fn set_plot_data(&mut self, plot_data_index: i32, settings: i32, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(plot_data_index);
        a.i32(settings);
        a.i32(value);
        let _r = crate::transport::call(131 /* CMD_GUI_CONTROL_PROPERTIES_SET_PLOT_DATA */, &a)?;
        Ok(())
    }

    /// Set List Item Selected. This sets which item in a list is selected.. Wire: `g\e\g`
    pub fn set_list_item_selected(&mut self, log_index: i32, list_index: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(log_index);
        a.i32(list_index);
        let _r = crate::transport::call(132 /* CMD_GUI_CONTROL_PROPERTIES_SET_LIST_ITEM_SELECTED */, &a)?;
        Ok(())
    }

    /// Set List Item Top Index. This sets the first viewable item in the list. . Wire: `g\e\i`
    pub fn set_list_item_top_index(&mut self, log_item: i32, list_index: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(log_item);
        a.i32(list_index);
        let _r = crate::transport::call(133 /* CMD_GUI_CONTROL_PROPERTIES_SET_LIST_ITEM_TOP_INDEX */, &a)?;
        Ok(())
    }

    /// Set Control Property. Sets a property based on a property type index. Wire: `g\e\j`
    pub fn set_control_property(&mut self, index: i32, property: i32, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(property);
        a.i32(value);
        let _r = crate::transport::call(134 /* CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_PROPERTY */, &a)?;
        Ok(())
    }
}
