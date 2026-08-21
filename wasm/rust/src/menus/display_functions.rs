//! Display Functions menu - generated from fwMenuDisplayFunctions. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct DisplayFunctions<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> DisplayFunctions<'a> {
    /// List Display Apps. Lists the firmware images available in the SD card /apps/ directory.. Wire: `h\v\l`
    pub fn list_display_apps(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(470 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_LIST_DISPLAY_APPS */, &a)?;
        Ok(())
    }

    /// Restore Display Firmware. Reflashes /firmware/FW2Display.uf2 to restore the standard display GUI.. Wire: `h\v\r`
    pub fn restore_display_firmware(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(471 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_RESTORE_DISPLAY_FIRMWARE */, &a)?;
        Ok(())
    }

    /// Display Bootloader Version. Enters the display bootloader, reads its version, and releases the link without transferring anything.. Wire: `h\v\v`
    pub fn display_bl_version(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(472 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_DISPLAY_BL_VERSION */, &a)?;
        Ok(())
    }

    /// Reset Display CPU. Pulses the display processor reset so it cold-boots its flash image.. Wire: `h\v\x`
    pub fn reset_display_cpu(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(473 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_RESET_DISPLAY_CPU */, &a)?;
        Ok(())
    }

    /// Power Cycle Display. Cuts the display processor's power rail and restores it, giving a true power-on reset. Heavier than Reset Display CPU, which only pulses RUN. Bootloader entry uses RUN/BOOT on its own; use this when a warm reset is not enough.. Wire: `h\v\c`
    pub fn power_cycle_display_cpu(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(468 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_POWER_CYCLE_DISPLAY_CPU */, &a)?;
        Ok(())
    }

    /// Set RAM App Argument. Arms up to 128 bytes for the NEXT Run RAM App, placed at a fixed address near the top of the display's RAM window. Blank clears it. An armed argument makes the launch noticeably slower: the fused bootloader cannot seek, so the loader must pad the wire up to that address.. Wire: `h\v\g`
    pub fn set_ram_app_arg(&mut self, text: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(text);
        let _r = crate::transport::call(469 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_SET_RAM_APP_ARG */, &a)?;
        Ok(())
    }

    /// Run App On Display. Asks the display processor to load and run /apps/<filename> itself: it reads the UF2 over the SD link, shows a progress bar on its own screen, and jumps to the image. Works for UF2s targeting the PSRAM window (0x11000000, up to ~4 MB) or the RAM window (0x20000000, up to 448 KB) -- the display copies the image to its run address at the moment of launch. Flash is untouched; Reset Display CPU restores the stock firmware. Progress and errors appear on the display, not here.. Wire: `h\v\a`
    pub fn run_app_on_display(&mut self, filename: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filename);
        let _r = crate::transport::call(503 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_RUN_APP_ON_DISPLAY */, &a)?;
        Ok(())
    }

    /// Run PSRAM App. Runs /apps/<filename> on the display processor from PSRAM (0x11000000 window, up to 8 MB). Two-hop launch: a small SRAM stub is staged through the fused bootloader, then the stub receives the image into PSRAM and jumps to it. Flash is untouched; Reset Display CPU restores the stock firmware. The image must be a UF2 whose blocks target the PSRAM window.. Wire: `h\v\p`
    pub fn run_psram_app(&mut self, filename: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filename);
        let _r = crate::transport::call(474 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_RUN_PSRAM_APP */, &a)?;
        Ok(())
    }

    /// Load PSRAM Data. Stages /apps/<filename> verbatim into the display's PSRAM at <offset> bytes from 0x11000000, and leaves the loader stub running instead of launching anything. For bulk assets that would otherwise have to travel inside the app's own UF2. The file is taken as raw bytes: no UF2 decode. Repeat for as many blobs as needed, then Run PSRAM App -- the stub stays resident between calls, so only the first pays the two-hop entry, and the launch overwrites only what the app image itself covers. Staged data does NOT survive a display reset.. Wire: `h\v\s`
    pub fn load_psram_data(&mut self, filename: &str, offset: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filename);
        a.u32(offset);
        let _r = crate::transport::call(486 /* CMD_HARDWARE_DISPLAY_FUNCTIONS_LOAD_PSRAM_DATA */, &a)?;
        Ok(())
    }
}
