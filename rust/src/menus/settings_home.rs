//! Device Settings menu - generated from fwMenuSettingsHome. Do not edit.

use crate::transport::{OwError, Transport};

pub struct SettingsHome<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> SettingsHome<'a> {
    /// UART Settings sub-menu.
    pub fn uart_settings(self) -> super::uart_settings_2::UartSettings2<'a> {
        super::uart_settings_2::UartSettings2 { t: self.t }
    }

    /// I2C Settings sub-menu.
    pub fn i2c_settings(self) -> super::i2c_settings_2::I2cSettings2<'a> {
        super::i2c_settings_2::I2cSettings2 { t: self.t }
    }

    /// Sensor Settings sub-menu.
    pub fn sensor_settings(self) -> super::sensor_settings::SensorSettings<'a> {
        super::sensor_settings::SensorSettings { t: self.t }
    }

    /// SPI Settings sub-menu.
    pub fn spi_settings(self) -> super::spi_settings_2::SpiSettings2<'a> {
        super::spi_settings_2::SpiSettings2 { t: self.t }
    }

    /// IO Directions sub-menu.
    pub fn io_direction_settings(self) -> super::io_direction_settings_2::IoDirectionSettings2<'a> {
        super::io_direction_settings_2::IoDirectionSettings2 { t: self.t }
    }

    /// FPGA Clock sub-menu.
    pub fn fpga_clock_settings(self) -> super::fpga_clock_settings::FpgaClockSettings<'a> {
        super::fpga_clock_settings::FpgaClockSettings { t: self.t }
    }

    /// radio1 sub-menu.
    pub fn radio_settings(self) -> super::radio_settings::RadioSettings<'a> {
        super::radio_settings::RadioSettings { t: self.t }
    }

    /// radio1 sub-menu.
    pub fn radio_settings_2(self) -> super::radio_settings_2::RadioSettings2<'a> {
        super::radio_settings_2::RadioSettings2 { t: self.t }
    }

    /// RF Analyzer Settings sub-menu.
    pub fn radio_fa_settings(self) -> super::radio_fa_settings::RadioFaSettings<'a> {
        super::radio_fa_settings::RadioFaSettings { t: self.t }
    }

    /// RTC Settings sub-menu.
    pub fn rtc_settings(self) -> super::rtc_settings::RtcSettings<'a> {
        super::rtc_settings::RtcSettings { t: self.t }
    }

    /// Wifi Settings sub-menu.
    pub fn wifi_settings(self) -> super::wifi_settings::WifiSettings<'a> {
        super::wifi_settings::WifiSettings { t: self.t }
    }

    /// Bluetooth Settings sub-menu.
    pub fn ble_settings(self) -> super::ble_settings::BleSettings<'a> {
        super::ble_settings::BleSettings { t: self.t }
    }

    /// Orca Communication sub-menu.
    pub fn orca_settings(self) -> super::orca_settings::OrcaSettings<'a> {
        super::orca_settings::OrcaSettings { t: self.t }
    }

    /// Websocket Server sub-menu.
    pub fn websocket_settings(self) -> super::websocket_settings::WebsocketSettings<'a> {
        super::websocket_settings::WebsocketSettings { t: self.t }
    }

    /// Neptune Settings sub-menu.
    pub fn neptune_settings(self) -> super::neptune_settings::NeptuneSettings<'a> {
        super::neptune_settings::NeptuneSettings { t: self.t }
    }

    /// General Settings sub-menu.
    pub fn general_settings(self) -> super::general_settings::GeneralSettings<'a> {
        super::general_settings::GeneralSettings { t: self.t }
    }

    /// Analog In (TLA2024) Settings sub-menu.
    pub fn analog_in_settings(self) -> super::analog_in_settings::AnalogInSettings<'a> {
        super::analog_in_settings::AnalogInSettings { t: self.t }
    }

    /// Sound Settings sub-menu.
    pub fn sound_settings(self) -> super::sound_settings::SoundSettings<'a> {
        super::sound_settings::SoundSettings { t: self.t }
    }

    /// Power Settings sub-menu.
    pub fn power_settings(self) -> super::power_settings::PowerSettings<'a> {
        super::power_settings::PowerSettings { t: self.t }
    }

    /// LED Show Settings sub-menu.
    pub fn light_show_settings(self) -> super::light_show_settings::LightShowSettings<'a> {
        super::light_show_settings::LightShowSettings { t: self.t }
    }

    /// Interface Settings sub-menu.
    pub fn interface_settings(self) -> super::interface_settings::InterfaceSettings<'a> {
        super::interface_settings::InterfaceSettings { t: self.t }
    }

    /// Software Reset. Performs a software reset of the device.. Wire: `h\s\1`
    pub fn software_reset(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\1");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Reset To Bootloader. Resets the device into the USB bootloader.. Wire: `h\s\2`
    pub fn software_reset_to_bootloader(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\2");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// All Settings To Defaults. Restores all settings to their default values.. Wire: `h\s\3`
    pub fn all_settings_to_defaults(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\3");
        self.t.call(&cmd)?;
        Ok(())
    }
}
