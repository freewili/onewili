//! File System menu - generated from fwMenuFileSystem. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct FileSystem<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> FileSystem<'a> {
    /// Change Directory. Changes current directory. Wire: `h\x\a`
    pub fn change_directory(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\a");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Create Directory. Creates a new directory. Wire: `h\x\c`
    pub fn create_directory(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\c");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Remove File or Directory. Removes a file or directory. Wire: `h\x\r`
    pub fn remove_file_or_directory(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\r");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get File From PC. Downloads file to Free Wili. Wire: `h\x\f`
    pub fn get_file_from_pc(&mut self, path: &str, size: i32, crc32: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\f");
        encoding::push_str(&mut cmd, path);
        encoding::push_int(&mut cmd, size as i64);
        encoding::push_int(&mut cmd, crc32 as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Send File To PC. Sends file to PC. Wire: `h\x\u`
    pub fn send_file_to_pc(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\u");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Print File. Prints the File Content. Wire: `h\x\p`
    pub fn print_file(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\p");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Create Blank File. Creates a blank file. Wire: `h\x\b`
    pub fn create_blank_file(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\b");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Edit File. Edits a text file. Wire: `h\x\e`
    pub fn edit_file(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\e");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Rename or Move File Or Directory. Renames or Moves a File or Directory. Wire: `h\x\n`
    pub fn rename_or_move_file_directory(&mut self, path: &str, new_path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\n");
        encoding::push_str(&mut cmd, path);
        encoding::push_str(&mut cmd, new_path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// List Directory. lists the contents of a directory. Blank for current directory.. Wire: `h\x\l`
    pub fn list_directory(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\l");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Format File System. reformats the internal flash. Wire: `h\x\t`
    pub fn format_file_system(&mut self, confirm: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\t");
        encoding::push_str(&mut cmd, confirm);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Toggle SDCard Host. Toggles which host controls the SD card.. Wire: `h\x\s`
    pub fn toggle_sd_card_host_select(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\x\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Load Wili Project. Loads a fwcom .wili project (panels, blocks, app signals) and shows the Panels app.. Wire: `h\x\w`
    pub fn load_wili_project(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\w");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SDCard Host Select. Connects the SD card to the main CPU (0) or the USB reader / PC (1).. Wire: `h\x\k`
    pub fn set_sd_card_host(&mut self, host: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\k");
        encoding::push_int(&mut cmd, host as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("fdir", false, "kind=string,name=string,size=decU32", "directory listing entry; kind is dir/fil, or end with size as the entry count"),
    ("filedl", false, "data=string", "File download progress ('complete N bytes')"),
    ("fpgadl", false, "data=string", "FPGA bitstream download progress ('complete N bytes')"),
];
