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

    /// Begin File Read. Open an SD file for bounded framed reads. Paths are UTF-8 encoded as compact hex and must be absolute; the session expires after 30 seconds of inactivity.. Wire: `h\x\0`
    pub fn begin_file_read(&mut self, session: u32, path_hex: &str) -> Result<i32, OwError> {
        let mut cmd = String::from("h\\x\\0");
        encoding::push_hex(&mut cmd, session as u64, 8);
        encoding::push_str(&mut cmd, path_hex);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let size = encoding::tok_int(&mut toks)? as i32;
        Ok(size)
    }

    /// Begin File Write. Stage an upload beside its destination. Existing files are preserved until size and CRC32 validation succeeds. No raw USB mode is entered.. Wire: `h\x\1`
    pub fn begin_file_write(&mut self, session: u32, path_hex: &str, size: i32, crc32: u32, overwrite: bool) -> Result<i32, OwError> {
        let mut cmd = String::from("h\\x\\1");
        encoding::push_hex(&mut cmd, session as u64, 8);
        encoding::push_str(&mut cmd, path_hex);
        encoding::push_int(&mut cmd, size as i64);
        encoding::push_hex(&mut cmd, crc32 as u64, 8);
        encoding::push_bool(&mut cmd, overwrite);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let size = encoding::tok_int(&mut toks)? as i32;
        Ok(size)
    }

    /// Read File Chunk. Read the next 1 to 192 bytes as compact hex. Use the exact sequential offset; a dash means an empty file or EOF.. Wire: `h\x\2`
    pub fn read_file_chunk(&mut self, session: u32, offset: i32, maximum: i32) -> Result<(i32, String), OwError> {
        let mut cmd = String::from("h\\x\\2");
        encoding::push_hex(&mut cmd, session as u64, 8);
        encoding::push_int(&mut cmd, offset as i64);
        encoding::push_int(&mut cmd, maximum as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let count = encoding::tok_int(&mut toks)? as i32;
        let data = encoding::rest_str(&mut toks);
        Ok((count, data))
    }

    /// Write File Chunk. Write the next 1 to 192 hex-encoded bytes. Duplicate or out-of-order chunks are rejected; never replay an ambiguous timeout.. Wire: `h\x\3`
    pub fn write_file_chunk(&mut self, session: u32, offset: i32, data: &str) -> Result<i32, OwError> {
        let mut cmd = String::from("h\\x\\3");
        encoding::push_hex(&mut cmd, session as u64, 8);
        encoding::push_int(&mut cmd, offset as i64);
        encoding::push_str(&mut cmd, data);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let position = encoding::tok_int(&mut toks)? as i32;
        Ok(position)
    }

    /// Finish File Transfer. Verify the byte count, close the file and return CRC32. A complete verified upload is published; an incomplete or corrupt upload never replaces the destination.. Wire: `h\x\4`
    pub fn finish_file_transfer(&mut self, session: u32) -> Result<(i32, u32), OwError> {
        let mut cmd = String::from("h\\x\\4");
        encoding::push_hex(&mut cmd, session as u64, 8);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let size = encoding::tok_int(&mut toks)? as i32;
        let crc32 = encoding::tok_hex(&mut toks)? as u32;
        Ok((size, crc32))
    }

    /// Cancel File Transfer. Close the matching transfer and remove its incomplete staging file. Other shell and menu sessions remain available.. Wire: `h\x\5`
    pub fn cancel_file_transfer(&mut self, session: u32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\x\\5");
        encoding::push_hex(&mut cmd, session as u64, 8);
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
