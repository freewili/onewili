//! ESP32 Flasher Functions menu - generated from fwMenuESP32Flasher. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Esp32Flasher<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Esp32Flasher<'a> {
    /// Connect To Bootloader. Opens a ROM-loader session: resets the ESP32 into its bootloader and loads the flasher stub. Wire: `w\a\b`
    pub fn enter_bootloader(&mut self, upgrade_transmission_rate: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\b");
        encoding::push_int(&mut cmd, upgrade_transmission_rate as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Reset. Closes any loader session and resets the ESP32 into its application. Wire: `w\a\r`
    pub fn enter_application(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\a\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read Chip ID And Security Info. Reads the ESP32's chip ID, ECO version and security flags. Wire: `w\a\i`
    pub fn get_i_dand_security(&mut self) -> Result<(i32, i32, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool), OwError> {
        let cmd = String::from("w\\a\\i");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let esp_chip_id = encoding::tok_int(&mut toks)? as i32;
        let version = encoding::tok_int(&mut toks)? as i32;
        let sb_en = encoding::tok_bool(&mut toks)?;
        let sbar_en = encoding::tok_bool(&mut toks)?;
        let sdm_en = encoding::tok_bool(&mut toks)?;
        let sbrk_1 = encoding::tok_bool(&mut toks)?;
        let sbrk_2 = encoding::tok_bool(&mut toks)?;
        let sbrk_3 = encoding::tok_bool(&mut toks)?;
        let jtag_sw_dis = encoding::tok_bool(&mut toks)?;
        let jtag_hw_dis = encoding::tok_bool(&mut toks)?;
        let usb_dis = encoding::tok_bool(&mut toks)?;
        let flash_enc_en = encoding::tok_bool(&mut toks)?;
        let dcache_dis = encoding::tok_bool(&mut toks)?;
        let icache_dis = encoding::tok_bool(&mut toks)?;
        Ok((esp_chip_id, version, sb_en, sbar_en, sdm_en, sbrk_1, sbrk_2, sbrk_3, jtag_sw_dis, jtag_hw_dis, usb_dis, flash_enc_en, dcache_dis, icache_dis))
    }

    /// Read Flash Size. Detects the ESP32's flash size in bytes. Wire: `w\a\k`
    pub fn read_flash_size(&mut self) -> Result<i32, OwError> {
        let cmd = String::from("w\\a\\k");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let flash_size_bytes = encoding::tok_int(&mut toks)? as i32;
        Ok(flash_size_bytes)
    }

    /// Read MAC. Reads the ESP32's factory MAC address. Wire: `w\a\m`
    pub fn read_esp32mac(&mut self) -> Result<String, OwError> {
        let cmd = String::from("w\\a\\m");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let esp32_mac = encoding::rest_str(&mut toks);
        Ok(esp32_mac)
    }

    /// Erase All Flash. Erases the ESP32's entire flash. Needs an open loader session. Wire: `w\a\e`
    pub fn erase_all_flash(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\a\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Start Writing Flash Operations. Prepares ESP32 to write flash at offset and expected size. Block size can be up to 128 bytes; each Write Flash sends one block. Wire: `w\a\f`
    pub fn start_flash_operations(&mut self, offset: u32, size: i32, block_size: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\f");
        encoding::push_hex(&mut cmd, offset as u64, 8);
        encoding::push_int(&mut cmd, size as i64);
        encoding::push_int(&mut cmd, block_size as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Finish Flash Writing Operations. Ends ESP32 flashing; reboot=1 also closes the session and starts the new image. Wire: `w\a\p`
    pub fn stop_flash_operation(&mut self, reboot: bool) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\p");
        encoding::push_bool(&mut cmd, reboot);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Write Flash. Writes one block (up to the block size given to f) into flash. Wire: `w\a\o`
    pub fn flash_write(&mut self, flash_data: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\o");
        encoding::push_bytes(&mut cmd, flash_data);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read Flash. Reads up to 128 bytes of ESP32 flash at the given address. Wire: `w\a\j`
    pub fn flash_read(&mut self, offset: u32, size: i32) -> Result<Vec<u8>, OwError> {
        let mut cmd = String::from("w\\a\\j");
        encoding::push_hex(&mut cmd, offset as u64, 8);
        encoding::push_int(&mut cmd, size as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let data = encoding::rest_bytes(&mut toks)?;
        Ok(data)
    }

    /// Start Memory Write Operations. Prepares a RAM load on the ESP32. Block size can be up to 128 bytes. Wire: `w\a\y`
    pub fn start_write_memory_operations(&mut self, offset: u32, size: i32, block_size: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\y");
        encoding::push_hex(&mut cmd, offset as u64, 8);
        encoding::push_int(&mut cmd, size as i64);
        encoding::push_int(&mut cmd, block_size as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Write Memory. Writes one block (up to the block size given to y) into ESP32 RAM. Wire: `w\a\0`
    pub fn memory_write(&mut self, data: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\0");
        encoding::push_bytes(&mut cmd, data);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop Memory Write Operations. Ends a RAM load; a non-zero entry point starts the loaded code and closes the session. Wire: `w\a\t`
    pub fn stop_memory_operation(&mut self, entry_address: u32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\t");
        encoding::push_hex(&mut cmd, entry_address as u64, 8);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Write Register. Writes a 4 byte value onto a register in the esp32. Wire: `w\a\g`
    pub fn register_write(&mut self, offset: u32, value: u32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\g");
        encoding::push_hex(&mut cmd, offset as u64, 8);
        encoding::push_hex(&mut cmd, value as u64, 8);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read Register. Reads a 4 byte value from a register in the esp32. Wire: `w\a\c`
    pub fn register_read(&mut self, offset: u32) -> Result<u32, OwError> {
        let mut cmd = String::from("w\\a\\c");
        encoding::push_hex(&mut cmd, offset as u64, 8);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let memory_block = encoding::tok_hex(&mut toks)? as u32;
        Ok(memory_block)
    }

    /// Flash Default App. Not available on FW2: there is no built-in image. Use Flash From Folder. Wire: `w\a\n`
    pub fn flash_default(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\a\\n");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Flash From Folder. Flashes the ESP32 from an idf.py build folder on the SD card. Wire: `w\a\w`
    pub fn flash_from_folder(&mut self, folder: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\a\\w");
        encoding::push_str(&mut cmd, folder);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Flash Status. Reports ESP32 flashing state and progress percentage. Wire: `w\a\s`
    pub fn flash_status(&mut self) -> Result<(bool, i32, i32, i32), OwError> {
        let cmd = String::from("w\\a\\s");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let flashing = encoding::tok_bool(&mut toks)?;
        let progress = encoding::tok_int(&mut toks)? as i32;
        let partition_index = encoding::tok_int(&mut toks)? as i32;
        let partition_count = encoding::tok_int(&mut toks)? as i32;
        Ok((flashing, progress, partition_index, partition_count))
    }
}
