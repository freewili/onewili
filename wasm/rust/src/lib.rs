//! OneWili WASM crate - generated. Do not edit.
//! Enum newtypes keep the firmware's own names, so the lint is off crate-wide.
#![allow(non_camel_case_types)]
pub mod enums;
pub mod transport;
pub mod menus;

pub use enums::*;
pub use transport::OwError;

pub struct OneWili {
    /// No real transport state - `ow_call` is stateless. Kept only so the
    /// generated per-menu accessor pattern has a field to borrow.
    t: (),
}

impl OneWili {
    /// Open the device. WASM guest: no handshake, `ow_call` is stateless.
    pub fn open() -> Result<Self, OwError> {
        Ok(OneWili { t: () })
    }

    /// IO functions.
    pub fn io(&mut self) -> menus::io::Io<'_> {
        menus::io::Io { t: &mut self.t }
    }

    /// GUI Functions.
    pub fn gui(&mut self) -> menus::gui::Gui<'_> {
        menus::gui::Gui { t: &mut self.t }
    }

    /// Hardware Functions.
    pub fn hardware(&mut self) -> menus::hardware::Hardware<'_> {
        menus::hardware::Hardware { t: &mut self.t }
    }

    /// Wireless.
    pub fn wireless(&mut self) -> menus::wireless::Wireless<'_> {
        menus::wireless::Wireless { t: &mut self.t }
    }

    /// Scripting Functions.
    pub fn scripting(&mut self) -> menus::scripting::Scripting<'_> {
        menus::scripting::Scripting { t: &mut self.t }
    }

    /// Apps functions.
    pub fn apps(&mut self) -> menus::apps::Apps<'_> {
        menus::apps::Apps { t: &mut self.t }
    }

    /// Linux Functions.
    pub fn linux(&mut self) -> menus::linux::Linux<'_> {
        menus::linux::Linux { t: &mut self.t }
    }

    /// Logger.
    pub fn logger(&mut self) -> menus::logger::Logger<'_> {
        menus::logger::Logger { t: &mut self.t }
    }
}
