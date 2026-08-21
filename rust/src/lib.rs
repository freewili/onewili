//! OneWili API - generated Rust bindings for the FreeWili serial menu system.
//! Do not edit; regenerate from the firmware sources.
#![allow(non_camel_case_types)]

pub mod binary_framing;
pub mod binary_transport;
pub mod encoding;
pub mod enums;
pub mod events;
pub mod files;
pub mod framing;
pub mod menus;
pub mod transport;

pub use events::Event;
pub use transport::OwError;
pub use enums::*;

pub struct OneWili {
    transport: transport::Transport,
    binary: Option<binary_transport::BinaryTransport>,
}

impl OneWili {
    /// Find the FreeWili by USB VID 0x093C and open its serial port.
    pub fn connect() -> Result<Self, OwError> {
        Ok(Self {
            transport: transport::Transport::open(&transport::find_port()?)?,
            binary: None,
        })
    }

    /// Open a specific serial port, e.g. "COM5" or "/dev/ttyACM0".
    pub fn connect_port(name: &str) -> Result<Self, OwError> {
        Ok(Self { transport: transport::Transport::open(name)?, binary: None })
    }

    /// Open both ports: text (menu commands) + binary (FTDI event
    /// stream). If the binary port fails to open, the already-open
    /// text port is dropped with the error.
    pub fn connect_binary() -> Result<Self, OwError> {
        let mut dev = Self::connect()?;
        dev.open_binary()?;
        Ok(dev)
    }

    /// Lazily open the binary (FTDI) event port by discovery. Idempotent.
    pub fn open_binary(&mut self) -> Result<(), OwError> {
        if self.binary.is_none() {
            return self.open_binary_port(&transport::find_binary_port()?);
        }
        Ok(())
    }

    /// Open a specific binary port, e.g. "COM9" - use when more than one
    /// FTDI device is connected and discovery is ambiguous. Idempotent.
    pub fn open_binary_port(&mut self, name: &str) -> Result<(), OwError> {
        if self.binary.is_none() {
            self.binary = Some(binary_transport::BinaryTransport::open(name)?);
        }
        Ok(())
    }

    /// Next decoded event - binary frames first, then text events.
    /// Never blocks; returns Ok(None) when nothing is pending.
    pub fn poll_event(&mut self) -> Result<Option<events::Event>, OwError> {
        if let Some(b) = self.binary.as_mut() {
            while let Some(f) = b.poll_raw()? {
                match events::decode(f.header_type, &f.payload, f.error) {
                    Some(events::Event::Unknown { header_type, len }) => {
                        b.unknown_frames += 1;
                        return Ok(Some(events::Event::Unknown { header_type, len }));
                    }
                    Some(ev) => return Ok(Some(ev)),
                    None => b.size_mismatches += 1,
                }
            }
        }
        if let Some(line) = self.transport.poll_text_event()? {
            let body = line.strip_prefix("[*").unwrap_or(&line);
            let body = body.strip_suffix(']').unwrap_or(body);
            let (id, args) = match body.split_once(' ') {
                Some((i, a)) => (i.to_string(), a.to_string()),
                None => (body.to_string(), String::new()),
            };
            return Ok(Some(events::Event::Text { id, args }));
        }
        Ok(None)
    }

    /// File transfer and directory listing.
    pub fn files(&mut self) -> files::Files<'_> {
        files::Files {
            t: &mut self.transport,
            // Delegates to the generated hardware().file_system().set_sd_card_host() binding.
            sd_host_select_impl: |t, host| {
                menus::hardware::Hardware { t }.file_system().set_sd_card_host(host)
            },
        }
    }

    /// IO functions.
    pub fn io(&mut self) -> menus::io::Io<'_> {
        menus::io::Io { t: &mut self.transport }
    }

    /// GUI Functions.
    pub fn gui(&mut self) -> menus::gui::Gui<'_> {
        menus::gui::Gui { t: &mut self.transport }
    }

    /// Hardware Functions.
    pub fn hardware(&mut self) -> menus::hardware::Hardware<'_> {
        menus::hardware::Hardware { t: &mut self.transport }
    }

    /// Wireless.
    pub fn wireless(&mut self) -> menus::wireless::Wireless<'_> {
        menus::wireless::Wireless { t: &mut self.transport }
    }

    /// Scripting Functions.
    pub fn scripting(&mut self) -> menus::scripting::Scripting<'_> {
        menus::scripting::Scripting { t: &mut self.transport }
    }

    /// Apps functions.
    pub fn apps(&mut self) -> menus::apps::Apps<'_> {
        menus::apps::Apps { t: &mut self.transport }
    }

    /// Linux Functions.
    pub fn linux(&mut self) -> menus::linux::Linux<'_> {
        menus::linux::Linux { t: &mut self.transport }
    }

    /// Logger.
    pub fn logger(&mut self) -> menus::logger::Logger<'_> {
        menus::logger::Logger { t: &mut self.transport }
    }
}
