//! GUI Control Properties menu - generated from fwMenuGUIControlProperties. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct GuiControlProperties<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> GuiControlProperties<'a> {
    /// Set Control Value Text. sets the text value of a control. Wire: `g\e\a`
    pub fn set_control_value_text(&mut self, index: i32, text: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\a");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_str(&mut cmd, text);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Control Value Int. Set the text value of a control. Wire: `g\e\b`
    pub fn set_control_value_int(&mut self, index: i32, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\b");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Control Value Float. Set the float value of the control.. Wire: `g\e\c`
    pub fn set_control_value_float(&mut self, index: i32, value: f64) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\c");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_float(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set List Item Text. Sets the text and color of a specific list item. Wire: `g\e\k`
    pub fn set_list_item_text(&mut self, log_index: i32, list_item: i32, color: i32, text: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\k");
        encoding::push_int(&mut cmd, log_index as i64);
        encoding::push_int(&mut cmd, list_item as i64);
        encoding::push_int(&mut cmd, color as i64);
        encoding::push_str(&mut cmd, text);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Control Value Min Max Int. Sets whether a min and max is applied to a controls value. Wire: `g\e\e`
    pub fn set_control_value_min_max_int(&mut self, index: i32, enable: bool, min: i32, max: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\e");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_bool(&mut cmd, enable);
        encoding::push_int(&mut cmd, min as i64);
        encoding::push_int(&mut cmd, max as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Control Value Min Max Float. Sets whether a min and max is applied to a controls value. Wire: `g\e\l`
    pub fn set_control_value_min_max_float(&mut self, index: i32, enable: bool, min: f64, max: f64) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\l");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_bool(&mut cmd, enable);
        encoding::push_float(&mut cmd, min);
        encoding::push_float(&mut cmd, max);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Plot Data. This adds data to a plot. Wire: `g\e\f`
    pub fn set_plot_data(&mut self, plot_data_index: i32, settings: i32, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\f");
        encoding::push_int(&mut cmd, plot_data_index as i64);
        encoding::push_int(&mut cmd, settings as i64);
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set List Item Selected. This sets which item in a list is selected.. Wire: `g\e\g`
    pub fn set_list_item_selected(&mut self, log_index: i32, list_index: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\g");
        encoding::push_int(&mut cmd, log_index as i64);
        encoding::push_int(&mut cmd, list_index as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set List Item Top Index. This sets the first viewable item in the list. . Wire: `g\e\i`
    pub fn set_list_item_top_index(&mut self, log_item: i32, list_index: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\i");
        encoding::push_int(&mut cmd, log_item as i64);
        encoding::push_int(&mut cmd, list_index as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Control Property. Sets a property based on a property type index. Wire: `g\e\j`
    pub fn set_control_property(&mut self, index: i32, property: i32, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\e\\j");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, property as i64);
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
