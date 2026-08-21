//! GUI Panels menu - generated from fwMenuGUIPanels. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct GuiPanels<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> GuiPanels<'a> {
    /// Add Panel. Reinitializes the custom panel for controls.. Wire: `g\c\a`
    pub fn add_panel(&mut self, use_tile: bool, tile_id: i32, color: &str, show_menu: bool) -> Result<(), OwError> {
        let mut cmd = String::from("g\\c\\a");
        encoding::push_bool(&mut cmd, use_tile);
        encoding::push_int(&mut cmd, tile_id as i64);
        encoding::push_str(&mut cmd, color);
        encoding::push_bool(&mut cmd, show_menu);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Add Panel Picklist. Shows a panel that allows user to pick from a list.. Wire: `g\c\b`
    pub fn add_panel_picklist(&mut self, use_tile: bool, tile_id: i32, icon_id: i32, log_index: i32, back_color: &str, fore_color: &str, caption: &str) -> Result<(), OwError> {
        let mut cmd = String::from("g\\c\\b");
        encoding::push_bool(&mut cmd, use_tile);
        encoding::push_int(&mut cmd, tile_id as i64);
        encoding::push_int(&mut cmd, icon_id as i64);
        encoding::push_int(&mut cmd, log_index as i64);
        encoding::push_str(&mut cmd, back_color);
        encoding::push_str(&mut cmd, fore_color);
        encoding::push_str(&mut cmd, caption);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Show Panel. Brings the panel with the given index to the front on the DISPLAY.. Wire: `g\c\c`
    pub fn show_panel(&mut self, index: i32) -> Result<(), OwError> {
        let mut cmd = String::from("g\\c\\c");
        encoding::push_int(&mut cmd, index as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
