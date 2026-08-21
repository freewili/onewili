//! GUI Panels menu - generated from fwMenuGUIPanels. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct GuiPanels<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> GuiPanels<'a> {
    /// Add Panel. Reinitializes the custom panel for controls.. Wire: `g\c\a`
    pub fn add_panel(&mut self, use_tile: bool, tile_id: i32, color: &str, show_menu: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(if use_tile { 1 } else { 0 });
        a.i32(tile_id);
        a.str(color);
        a.u8(if show_menu { 1 } else { 0 });
        let _r = crate::transport::call(111 /* CMD_GUI_PANELS_ADD_PANEL */, &a)?;
        Ok(())
    }

    /// Add Panel Picklist. Shows a panel that allows user to pick from a list.. Wire: `g\c\b`
    pub fn add_panel_picklist(&mut self, use_tile: bool, tile_id: i32, icon_id: i32, log_index: i32, back_color: &str, fore_color: &str, caption: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(if use_tile { 1 } else { 0 });
        a.i32(tile_id);
        a.i32(icon_id);
        a.i32(log_index);
        a.str(back_color);
        a.str(fore_color);
        a.str(caption);
        let _r = crate::transport::call(112 /* CMD_GUI_PANELS_ADD_PANEL_PICKLIST */, &a)?;
        Ok(())
    }

    /// Show Panel. Brings the panel with the given index to the front on the DISPLAY.. Wire: `g\c\c`
    pub fn show_panel(&mut self, index: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        let _r = crate::transport::call(113 /* CMD_GUI_PANELS_SHOW_PANEL */, &a)?;
        Ok(())
    }
}
