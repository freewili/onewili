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

    /// Stream Write. Sends one peer-stream datagram (1-128 bytes) to another OneWili client through MAIN. Best effort: a datagram the destination cannot take now is dropped and counted, never queued behind.. Wire: `h\a\w`
    pub fn stream_write(&mut self, dst: i32, data: &[u8]) -> Result<bool, OwError> {
        let mut cmd = String::from("h\\a\\w");
        encoding::push_int(&mut cmd, dst as i64);
        encoding::push_bytes(&mut cmd, data);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let delivered = encoding::tok_bool(&mut toks)?;
        Ok(delivered)
    }

    /// Stream Poll. Pops peer-stream datagrams queued for the calling client: frames popped, frames still queued, frames dropped for this client so far, then the datagrams packed as [src][len][bytes] records.. Wire: `h\a\p`
    pub fn stream_poll(&mut self, max: i32) -> Result<(i32, i32, i32, Vec<u8>), OwError> {
        let mut cmd = String::from("h\\a\\p");
        encoding::push_int(&mut cmd, max as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let frames = encoding::tok_int(&mut toks)? as i32;
        let queued = encoding::tok_int(&mut toks)? as i32;
        let dropped = encoding::tok_int(&mut toks)? as i32;
        let data = encoding::rest_bytes(&mut toks)?;
        Ok((frames, queued, dropped, data))
    }

    /// Stream Status. Peer-stream state for the calling client: the datagram MTU, datagrams waiting in its queue, datagrams addressed to it that MAIN dropped, and datagrams it sent that MAIN dropped.. Wire: `h\a\c`
    pub fn stream_status(&mut self) -> Result<(i32, i32, i32, i32), OwError> {
        let cmd = String::from("h\\a\\c");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let mtu = encoding::tok_int(&mut toks)? as i32;
        let queued = encoding::tok_int(&mut toks)? as i32;
        let droppedto = encoding::tok_int(&mut toks)? as i32;
        let droppedfrom = encoding::tok_int(&mut toks)? as i32;
        Ok((mtu, queued, droppedto, droppedfrom))
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("battery", false, "data=string", "Battery charger status text"),
];
