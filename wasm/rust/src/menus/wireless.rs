//! Wireless menu - generated from fwMenuWireless. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Wireless<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Wireless<'a> {
    /// NFC Functions sub-menu.
    pub fn nfc(self) -> super::nfc::Nfc<'a> {
        super::nfc::Nfc { t: self.t }
    }

    /// ESP32 Flasher Functions sub-menu.
    pub fn esp32_flasher(self) -> super::esp32_flasher::Esp32Flasher<'a> {
        super::esp32_flasher::Esp32Flasher { t: self.t }
    }

    /// Wifi Functions sub-menu.
    pub fn wifi(self) -> super::wifi::Wifi<'a> {
        super::wifi::Wifi { t: self.t }
    }

    /// BT Functions sub-menu.
    pub fn bluetooth_le(self) -> super::bluetooth_le::BluetoothLe<'a> {
        super::bluetooth_le::BluetoothLe { t: self.t }
    }

    /// IR Functions sub-menu.
    pub fn ir(self) -> super::ir::Ir<'a> {
        super::ir::Ir { t: self.t }
    }

    /// LoRa sub-menu.
    pub fn lo_ra(self) -> super::lo_ra::LoRa<'a> {
        super::lo_ra::LoRa { t: self.t }
    }

    /// Radio sub-menu.
    pub fn radio(self) -> super::radio::Radio<'a> {
        super::radio::Radio { t: self.t }
    }

    /// RFID Functions sub-menu.
    pub fn rfid(self) -> super::rfid::Rfid<'a> {
        super::rfid::Rfid { t: self.t }
    }

    /// ESP32 Mode. What the ESP32-C5 runs: Default Firmware (the stock WiFi/BLE app) or OneWili API (a BSP app on the ESP32 drives MAIN's OneWili API). OneWili API does not install an app - flash one first.. Wire: `w\e`
    pub fn e_sp32_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(624 /* CMD_WIRELESS_E_SP32_MODE */, &a)?;
        Ok(())
    }
}
