//! ESP32 Flasher Functions menu - generated from fwMenuESP32Flasher. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Esp32Flasher<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Esp32Flasher<'a> {
    /// Connect To Bootloader. Opens a ROM-loader session: resets the ESP32 into its bootloader and loads the flasher stub. Wire: `w\a\b`
    pub fn enter_bootloader(&mut self, upgrade_transmission_rate: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(upgrade_transmission_rate);
        let _r = crate::transport::call(312 /* CMD_WIRELESS_ESP32_FLASHER_ENTER_BOOTLOADER */, &a)?;
        Ok(())
    }

    /// Reset. Closes any loader session and resets the ESP32 into its application. Wire: `w\a\r`
    pub fn enter_application(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(313 /* CMD_WIRELESS_ESP32_FLASHER_ENTER_APPLICATION */, &a)?;
        Ok(())
    }

    /// Read Chip ID And Security Info. Reads the ESP32's chip ID, ECO version and security flags. Wire: `w\a\i`
    pub fn get_i_dand_security(&mut self) -> Result<(i32, i32, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(314 /* CMD_WIRELESS_ESP32_FLASHER_GET_I_DAND_SECURITY */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0, _r.u8() != 0))
    }

    /// Read Flash Size. Detects the ESP32's flash size in bytes. Wire: `w\a\k`
    pub fn read_flash_size(&mut self) -> Result<i32, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(315 /* CMD_WIRELESS_ESP32_FLASHER_READ_FLASH_SIZE */, &a)?;
        Ok(_r.i32())
    }

    /// Read MAC. Reads the ESP32's factory MAC address. Wire: `w\a\m`
    pub fn read_esp32mac(&mut self) -> Result<String, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(316 /* CMD_WIRELESS_ESP32_FLASHER_READ_ESP32MAC */, &a)?;
        Ok(_r.string())
    }

    /// Erase All Flash. Erases the ESP32's entire flash. Needs an open loader session. Wire: `w\a\e`
    pub fn erase_all_flash(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(317 /* CMD_WIRELESS_ESP32_FLASHER_ERASE_ALL_FLASH */, &a)?;
        Ok(())
    }

    /// Start Writing Flash Operations. Prepares ESP32 to write flash at offset and expected size. Block size can be up to 128 bytes; each Write Flash sends one block. Wire: `w\a\f`
    pub fn start_flash_operations(&mut self, offset: u32, size: i32, block_size: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(offset);
        a.i32(size);
        a.i32(block_size);
        let _r = crate::transport::call(318 /* CMD_WIRELESS_ESP32_FLASHER_START_FLASH_OPERATIONS */, &a)?;
        Ok(())
    }

    /// Finish Flash Writing Operations. Ends ESP32 flashing; reboot=1 also closes the session and starts the new image. Wire: `w\a\p`
    pub fn stop_flash_operation(&mut self, reboot: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(if reboot { 1 } else { 0 });
        let _r = crate::transport::call(319 /* CMD_WIRELESS_ESP32_FLASHER_STOP_FLASH_OPERATION */, &a)?;
        Ok(())
    }

    /// Write Flash. Writes one block (up to the block size given to f) into flash. Wire: `w\a\o`
    pub fn flash_write(&mut self, flash_data: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(flash_data);
        let _r = crate::transport::call(320 /* CMD_WIRELESS_ESP32_FLASHER_FLASH_WRITE */, &a)?;
        Ok(())
    }

    /// Read Flash. Reads up to 128 bytes of ESP32 flash at the given address. Wire: `w\a\j`
    pub fn flash_read(&mut self, offset: u32, size: i32) -> Result<Vec<u8>, OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(offset);
        a.i32(size);
        let mut _r = crate::transport::call(321 /* CMD_WIRELESS_ESP32_FLASHER_FLASH_READ */, &a)?;
        Ok(_r.bytes())
    }

    /// Start Memory Write Operations. Prepares a RAM load on the ESP32. Block size can be up to 128 bytes. Wire: `w\a\y`
    pub fn start_write_memory_operations(&mut self, offset: u32, size: i32, block_size: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(offset);
        a.i32(size);
        a.i32(block_size);
        let _r = crate::transport::call(322 /* CMD_WIRELESS_ESP32_FLASHER_START_WRITE_MEMORY_OPERATIONS */, &a)?;
        Ok(())
    }

    /// Write Memory. Writes one block (up to the block size given to y) into ESP32 RAM. Wire: `w\a\0`
    pub fn memory_write(&mut self, data: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(data);
        let _r = crate::transport::call(323 /* CMD_WIRELESS_ESP32_FLASHER_MEMORY_WRITE */, &a)?;
        Ok(())
    }

    /// Stop Memory Write Operations. Ends a RAM load; a non-zero entry point starts the loaded code and closes the session. Wire: `w\a\t`
    pub fn stop_memory_operation(&mut self, entry_address: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(entry_address);
        let _r = crate::transport::call(324 /* CMD_WIRELESS_ESP32_FLASHER_STOP_MEMORY_OPERATION */, &a)?;
        Ok(())
    }

    /// Write Register. Writes a 4 byte value onto a register in the esp32. Wire: `w\a\g`
    pub fn register_write(&mut self, offset: u32, value: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(offset);
        a.u32(value);
        let _r = crate::transport::call(325 /* CMD_WIRELESS_ESP32_FLASHER_REGISTER_WRITE */, &a)?;
        Ok(())
    }

    /// Read Register. Reads a 4 byte value from a register in the esp32. Wire: `w\a\c`
    pub fn register_read(&mut self, offset: u32) -> Result<u32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(offset);
        let mut _r = crate::transport::call(326 /* CMD_WIRELESS_ESP32_FLASHER_REGISTER_READ */, &a)?;
        Ok(_r.u32())
    }

    /// Flash Default App. Not available on FW2: there is no built-in image. Use Flash From Folder. Wire: `w\a\n`
    pub fn flash_default(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(327 /* CMD_WIRELESS_ESP32_FLASHER_FLASH_DEFAULT */, &a)?;
        Ok(())
    }

    /// Flash From Folder. Flashes the ESP32 from an idf.py build folder on the SD card. Wire: `w\a\w`
    pub fn flash_from_folder(&mut self, folder: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(folder);
        let _r = crate::transport::call(487 /* CMD_WIRELESS_ESP32_FLASHER_FLASH_FROM_FOLDER */, &a)?;
        Ok(())
    }

    /// Flash Status. Reports ESP32 flashing state and progress percentage. Wire: `w\a\s`
    pub fn flash_status(&mut self) -> Result<(bool, i32, i32, i32), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(488 /* CMD_WIRELESS_ESP32_FLASHER_FLASH_STATUS */, &a)?;
        Ok((_r.u8() != 0, _r.i32(), _r.i32(), _r.i32()))
    }
}
