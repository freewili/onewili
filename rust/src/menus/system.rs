//! System Functions menu - generated from fwMenuSystem. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct System<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> System<'a> {
    /// Stream Battery Info. Enables or disables streaming of battery info to the host.. Wire: `h\a\o`
    pub fn enable_battery_stream(&mut self, enable: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\a\\o");
        encoding::push_int(&mut cmd, enable as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read OTP Info. Reads bytes from the fused OTP identity blob (bl_otp_info v3). An unprovisioned device reads all zeros. Read in chunks of 256 bytes or less.. Wire: `h\a\b`
    pub fn read_otp_info(&mut self, offset: i32, length: i32) -> Result<Vec<u8>, OwError> {
        let mut cmd = String::from("h\\a\\b");
        encoding::push_int(&mut cmd, offset as i64);
        encoding::push_int(&mut cmd, length as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let otp_blob = encoding::rest_bytes(&mut toks)?;
        Ok(otp_blob)
    }

    /// Boot UF2. Reboots into the SBL bootloader, which chain-loads the named RAM-app UF2 from the SD card /update directory (card root as fallback). No response is sent on success — the device resets.. Wire: `h\a\u`
    pub fn boot_uf2(&mut self, filename: &str) -> Result<(), OwError> {
        let mut cmd = String::from("h\\a\\u");
        encoding::push_str(&mut cmd, filename);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Device State. Report the device state for host sync: SD card host (none|main|usb), event host-streaming gate (0|1), active-stream mask (hex, bit index = event id), clk_sys in Hz. More space-separated fields may be appended later.. Wire: `h\a\g`
    pub fn device_state(&mut self) -> Result<(String, bool, String, i32), OwError> {
        let cmd = String::from("h\\a\\g");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let sd = encoding::rest_str(&mut toks);
        let hoststream = encoding::tok_bool(&mut toks)?;
        let activemask = encoding::rest_str(&mut toks);
        let clksyshz = encoding::tok_int(&mut toks)? as i32;
        Ok((sd, hoststream, activemask, clksyshz))
    }

    /// Event Host Streaming. Enables or disables streaming of events to the host. When disabled, stream-class events are suppressed at the host output; protocol events still flow. Same gate as control bytes 0x05 (off) and 0x06 (on).. Wire: `h\a\e`
    pub fn event_host_streaming(&mut self, enable: i32) -> Result<bool, OwError> {
        let mut cmd = String::from("h\\a\\e");
        encoding::push_int(&mut cmd, enable as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let enabled = encoding::tok_bool(&mut toks)?;
        Ok(enabled)
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("battery", false, "data=string", "Battery charger status text"),
];
