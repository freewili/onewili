//! IO functions menu - generated from fwMenuIO. Do not edit.

use crate::transport::Transport;

pub struct Io<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Io<'a> {
    /// GPIO Functions sub-menu.
    pub fn gpio(self) -> super::gpio::Gpio<'a> {
        super::gpio::Gpio { t: self.t }
    }

    /// UART Functions sub-menu.
    pub fn uart(self) -> super::uart::Uart<'a> {
        super::uart::Uart { t: self.t }
    }

    /// MDIO Functions sub-menu.
    pub fn mdio(self) -> super::mdio::Mdio<'a> {
        super::mdio::Mdio { t: self.t }
    }

    /// Sensor Functions sub-menu.
    pub fn sensors(self) -> super::sensors::Sensors<'a> {
        super::sensors::Sensors { t: self.t }
    }

    /// I2C Functions sub-menu.
    pub fn i2c(self) -> super::i2c::I2c<'a> {
        super::i2c::I2c { t: self.t }
    }

    /// SPI Functions sub-menu.
    pub fn spi(self) -> super::spi::Spi<'a> {
        super::spi::Spi { t: self.t }
    }

    /// CANFD Functions sub-menu.
    pub fn canfd(self) -> super::canfd::Canfd<'a> {
        super::canfd::Canfd { t: self.t }
    }

    /// Analog In Functions sub-menu.
    pub fn analog_in(self) -> super::analog_in::AnalogIn<'a> {
        super::analog_in::AnalogIn { t: self.t }
    }

    /// Analog Out & Trigger Functions sub-menu.
    pub fn analog_out(self) -> super::analog_out::AnalogOut<'a> {
        super::analog_out::AnalogOut { t: self.t }
    }

    /// Logic Player Functions sub-menu.
    pub fn logic_player(self) -> super::logic_player::LogicPlayer<'a> {
        super::logic_player::LogicPlayer { t: self.t }
    }

    /// Logic Analyzer Functions sub-menu.
    pub fn logic_analyzer(self) -> super::logic_analyzer::LogicAnalyzer<'a> {
        super::logic_analyzer::LogicAnalyzer { t: self.t }
    }

    /// WILEye Functions sub-menu.
    pub fn wil_eye(self) -> super::wil_eye::WilEye<'a> {
        super::wil_eye::WilEye { t: self.t }
    }

    /// Audio Functions sub-menu.
    pub fn audio(self) -> super::audio::Audio<'a> {
        super::audio::Audio { t: self.t }
    }

    /// Serial LEDs sub-menu.
    pub fn serial_leds(self) -> super::serial_leds::SerialLeds<'a> {
        super::serial_leds::SerialLeds { t: self.t }
    }

    /// niceusb sub-menu.
    pub fn nice_usb(self) -> super::nice_usb::NiceUsb<'a> {
        super::nice_usb::NiceUsb { t: self.t }
    }

    /// CDC Serial Performance sub-menu.
    pub fn cdc_perf(self) -> super::cdc_perf::CdcPerf<'a> {
        super::cdc_perf::CdcPerf { t: self.t }
    }

    /// Ethernet 10BaseT1S sub-menu.
    pub fn t1s(self) -> super::t1s::T1s<'a> {
        super::t1s::T1s { t: self.t }
    }

    /// Network (TCP/IP) sub-menu.
    pub fn net(self) -> super::net::Net<'a> {
        super::net::Net { t: self.t }
    }
}
