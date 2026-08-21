//! File System menu - generated from fwMenuFileSystem. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct FileSystem<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> FileSystem<'a> {
    /// Change Directory. Changes current directory. Wire: `h\x\a`
    pub fn change_directory(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(292 /* CMD_HARDWARE_FILE_SYSTEM_CHANGE_DIRECTORY */, &a)?;
        Ok(())
    }

    /// Create Directory. Creates a new directory. Wire: `h\x\c`
    pub fn create_directory(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(293 /* CMD_HARDWARE_FILE_SYSTEM_CREATE_DIRECTORY */, &a)?;
        Ok(())
    }

    /// Remove File or Directory. Removes a file or directory. Wire: `h\x\r`
    pub fn remove_file_or_directory(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(294 /* CMD_HARDWARE_FILE_SYSTEM_REMOVE_FILE_OR_DIRECTORY */, &a)?;
        Ok(())
    }

    /// Get File From PC. Downloads file to Free Wili. Wire: `h\x\f`
    pub fn get_file_from_pc(&mut self, path: &str, size: i32, crc32: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        a.i32(size);
        a.i32(crc32);
        let _r = crate::transport::call(295 /* CMD_HARDWARE_FILE_SYSTEM_GET_FILE_FROM_PC */, &a)?;
        Ok(())
    }

    /// Send File To PC. Sends file to PC. Wire: `h\x\u`
    pub fn send_file_to_pc(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(296 /* CMD_HARDWARE_FILE_SYSTEM_SEND_FILE_TO_PC */, &a)?;
        Ok(())
    }

    /// Print File. Prints the File Content. Wire: `h\x\p`
    pub fn print_file(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(297 /* CMD_HARDWARE_FILE_SYSTEM_PRINT_FILE */, &a)?;
        Ok(())
    }

    /// Create Blank File. Creates a blank file. Wire: `h\x\b`
    pub fn create_blank_file(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(298 /* CMD_HARDWARE_FILE_SYSTEM_CREATE_BLANK_FILE */, &a)?;
        Ok(())
    }

    /// Edit File. Edits a text file. Wire: `h\x\e`
    pub fn edit_file(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(299 /* CMD_HARDWARE_FILE_SYSTEM_EDIT_FILE */, &a)?;
        Ok(())
    }

    /// Rename or Move File Or Directory. Renames or Moves a File or Directory. Wire: `h\x\n`
    pub fn rename_or_move_file_directory(&mut self, path: &str, new_path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        a.str(new_path);
        let _r = crate::transport::call(300 /* CMD_HARDWARE_FILE_SYSTEM_RENAME_OR_MOVE_FILE_DIRECTORY */, &a)?;
        Ok(())
    }

    /// List Directory. lists the contents of a directory. Blank for current directory.. Wire: `h\x\l`
    pub fn list_directory(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(301 /* CMD_HARDWARE_FILE_SYSTEM_LIST_DIRECTORY */, &a)?;
        Ok(())
    }

    /// Format File System. reformats the internal flash. Wire: `h\x\t`
    pub fn format_file_system(&mut self, confirm: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(confirm);
        let _r = crate::transport::call(302 /* CMD_HARDWARE_FILE_SYSTEM_FORMAT_FILE_SYSTEM */, &a)?;
        Ok(())
    }

    /// Toggle SDCard Host. Toggles which host controls the SD card.. Wire: `h\x\s`
    pub fn toggle_sd_card_host_select(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(303 /* CMD_HARDWARE_FILE_SYSTEM_TOGGLE_SD_CARD_HOST_SELECT */, &a)?;
        Ok(())
    }

    /// Load Wili Project. Loads a fwcom .wili project (panels, blocks, app signals) and shows the Panels app.. Wire: `h\x\w`
    pub fn load_wili_project(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(386 /* CMD_HARDWARE_FILE_SYSTEM_LOAD_WILI_PROJECT */, &a)?;
        Ok(())
    }

    /// SDCard Host Select. Connects the SD card to the main CPU (0) or the USB reader / PC (1).. Wire: `h\x\k`
    pub fn set_sd_card_host(&mut self, host: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(host);
        let _r = crate::transport::call(304 /* CMD_HARDWARE_FILE_SYSTEM_SET_SD_CARD_HOST */, &a)?;
        Ok(())
    }
}
