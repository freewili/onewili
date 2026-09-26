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

    /// Begin File Read. Open an SD file for bounded framed reads. Paths are UTF-8 encoded as compact hex and must be absolute; the session expires after 30 seconds of inactivity.. Wire: `h\x\0`
    pub fn begin_file_read(&mut self, session: u32, path_hex: &str) -> Result<i32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        a.str(path_hex);
        let mut _r = crate::transport::call(607 /* CMD_HARDWARE_FILE_SYSTEM_BEGIN_FILE_READ */, &a)?;
        Ok(_r.i32())
    }

    /// Begin File Write. Stage an upload beside its destination. Existing files are preserved until size and CRC32 validation succeeds. No raw USB mode is entered.. Wire: `h\x\1`
    pub fn begin_file_write(&mut self, session: u32, path_hex: &str, size: i32, crc32: u32, overwrite: bool) -> Result<i32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        a.str(path_hex);
        a.i32(size);
        a.u32(crc32);
        a.u8(if overwrite { 1 } else { 0 });
        let mut _r = crate::transport::call(608 /* CMD_HARDWARE_FILE_SYSTEM_BEGIN_FILE_WRITE */, &a)?;
        Ok(_r.i32())
    }

    /// Read File Chunk. Read the next 1 to 192 bytes as compact hex. Use the exact sequential offset; a dash means an empty file or EOF.. Wire: `h\x\2`
    pub fn read_file_chunk(&mut self, session: u32, offset: i32, maximum: i32) -> Result<(i32, String), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        a.i32(offset);
        a.i32(maximum);
        let mut _r = crate::transport::call(609 /* CMD_HARDWARE_FILE_SYSTEM_READ_FILE_CHUNK */, &a)?;
        Ok((_r.i32(), _r.string()))
    }

    /// Write File Chunk. Write the next 1 to 192 hex-encoded bytes. Duplicate or out-of-order chunks are rejected; never replay an ambiguous timeout.. Wire: `h\x\3`
    pub fn write_file_chunk(&mut self, session: u32, offset: i32, data: &str) -> Result<i32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        a.i32(offset);
        a.str(data);
        let mut _r = crate::transport::call(610 /* CMD_HARDWARE_FILE_SYSTEM_WRITE_FILE_CHUNK */, &a)?;
        Ok(_r.i32())
    }

    /// Finish File Transfer. Verify the byte count, close the file and return CRC32. A complete verified upload is published; an incomplete or corrupt upload never replaces the destination.. Wire: `h\x\4`
    pub fn finish_file_transfer(&mut self, session: u32) -> Result<(i32, u32), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        let mut _r = crate::transport::call(611 /* CMD_HARDWARE_FILE_SYSTEM_FINISH_FILE_TRANSFER */, &a)?;
        Ok((_r.i32(), _r.u32()))
    }

    /// Cancel File Transfer. Close the matching transfer and remove its incomplete staging file. Other shell and menu sessions remain available.. Wire: `h\x\5`
    pub fn cancel_file_transfer(&mut self, session: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(session);
        let _r = crate::transport::call(612 /* CMD_HARDWARE_FILE_SYSTEM_CANCEL_FILE_TRANSFER */, &a)?;
        Ok(())
    }
}
