//! Wireless menu - generated from fwMenuWireless. Do not edit.

use crate::transport::Transport;

pub struct Wireless<'a> {
    pub(crate) t: &'a mut Transport,
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
}
