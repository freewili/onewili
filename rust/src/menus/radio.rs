//! Radio menu - generated from fwMenuRadio. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Radio<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Radio<'a> {
    /// Select Circuit. Claims the sub-GHz front end for this client and holds it until Release.. Wire: `w\r\s`
    pub fn select_circuit(&mut self, band: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\s");
        encoding::push_int(&mut cmd, band as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Release Circuit. Ends this client's circuit hold and hands the antenna back to LoRa.. Wire: `w\r\e`
    pub fn release_circuit(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\r\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// State. Reads the mux and radio state back, in this order:. Wire: `w\r\t`
    pub fn read_state(&mut self) -> Result<(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, u8), OwError> {
        let cmd = String::from("w\\r\\t");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let owner = encoding::tok_int(&mut toks)? as i32;
        let holder = encoding::tok_int(&mut toks)? as i32;
        let band = encoding::tok_int(&mut toks)? as i32;
        let want_v1 = encoding::tok_int(&mut toks)? as i32;
        let want_v2 = encoding::tok_int(&mut toks)? as i32;
        let have_valid = encoding::tok_int(&mut toks)? as i32;
        let have_v1 = encoding::tok_int(&mut toks)? as i32;
        let have_v2 = encoding::tok_int(&mut toks)? as i32;
        let lora_paused = encoding::tok_int(&mut toks)? as i32;
        let freq_hz = encoding::tok_int(&mut toks)? as i32;
        let active = encoding::tok_int(&mut toks)? as i32;
        let status = encoding::tok_int(&mut toks)? as i32;
        let version = encoding::tok_hex(&mut toks)? as u8;
        Ok((owner, holder, band, want_v1, want_v2, have_valid, have_v1, have_v2, lora_paused, freq_hz, active, status, version))
    }

    /// Band. Forces the matched antenna path now: 1 low, 2 mid, 3 high.. Wire: `w\r\b`
    pub fn select_band(&mut self, band: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\b");
        encoding::push_int(&mut cmd, band as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Comm Check. Reads the CC1101 version register and returns it.. Wire: `w\r\c`
    pub fn comm_check(&mut self) -> Result<u8, OwError> {
        let cmd = String::from("w\\r\\c");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let version = encoding::tok_hex(&mut toks)? as u8;
        Ok(version)
    }

    /// Frequency. Tunes the CC1101 and selects the matched antenna path for that band.. Wire: `w\r\f`
    pub fn set_frequency(&mut self, freq_hz: i32) -> Result<i32, OwError> {
        let mut cmd = String::from("w\\r\\f");
        encoding::push_int(&mut cmd, freq_hz as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let band = encoding::tok_int(&mut toks)? as i32;
        Ok(band)
    }

    /// RSSI. Samples received signal strength once, in dBm.. Wire: `w\r\i`
    pub fn read_rssi(&mut self) -> Result<i32, OwError> {
        let cmd = String::from("w\\r\\i");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let rssi = encoding::tok_int(&mut toks)? as i32;
        Ok(rssi)
    }

    /// Carrier. Keys or unkeys an unmodulated carrier at the current frequency.. Wire: `w\r\o`
    pub fn carrier(&mut self, on: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\o");
        encoding::push_int(&mut cmd, on as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// RX Enable. Puts the CC1101 into continuous receive, or back to idle.. Wire: `w\r\r`
    pub fn rx_enable(&mut self, on: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\r");
        encoding::push_int(&mut cmd, on as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Idle. Returns the CC1101 to idle from receive, transmit or carrier.. Wire: `w\r\w`
    pub fn idle(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\r\\w");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Packet Send. Transmits one GFSK packet through the CC1101 packet engine.. Wire: `w\r\x`
    pub fn packet_send(&mut self, freq_hz: i32, data: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\x");
        encoding::push_int(&mut cmd, freq_hz as i64);
        encoding::push_bytes(&mut cmd, data);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Packet RX. Opens or closes the GFSK packet receiver at the given frequency.. Wire: `w\r\y`
    pub fn packet_rx(&mut self, on: i32, freq_hz: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\y");
        encoding::push_int(&mut cmd, on as i64);
        encoding::push_int(&mut cmd, freq_hz as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Packet Read. Reads the last received GFSK packet: RSSI in dBm, a sequence counter that. Wire: `w\r\k`
    pub fn packet_read(&mut self) -> Result<(i32, i32, Vec<u8>), OwError> {
        let cmd = String::from("w\\r\\k");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let rssi = encoding::tok_int(&mut toks)? as i32;
        let seq = encoding::tok_int(&mut toks)? as i32;
        let data = encoding::rest_bytes(&mut toks)?;
        Ok((rssi, seq, data))
    }

    /// Capture Start. Arms a raw pulse-duration capture at the given frequency.. Wire: `w\r\g`
    pub fn capture_start(&mut self, freq_hz: i32, preset: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\g");
        encoding::push_int(&mut cmd, freq_hz as i64);
        encoding::push_int(&mut cmd, preset as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Capture Stop. Ends a capture and returns how many pulse durations it recorded.. Wire: `w\r\j`
    pub fn capture_stop(&mut self) -> Result<i32, OwError> {
        let cmd = String::from("w\\r\\j");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let durations = encoding::tok_int(&mut toks)? as i32;
        Ok(durations)
    }

    /// Replay. Re-transmits the last capture out the transmit path.. Wire: `w\r\p`
    pub fn replay(&mut self) -> Result<(), OwError> {
        let cmd = String::from("w\\r\\p");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Analyzer. Starts or stops the background frequency-analyzer sweep.. Wire: `w\r\a`
    pub fn analyzer(&mut self, on: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\a");
        encoding::push_int(&mut cmd, on as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Spectrum. Reads the analyzer's results: the peak frequency in Hz, its level in dBm,. Wire: `w\r\n`
    pub fn spectrum(&mut self) -> Result<(i32, i32, Vec<u8>), OwError> {
        let cmd = String::from("w\\r\\n");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let peak_freq_hz = encoding::tok_int(&mut toks)? as i32;
        let peak_rssi = encoding::tok_int(&mut toks)? as i32;
        let bins = encoding::rest_bytes(&mut toks)?;
        Ok((peak_freq_hz, peak_rssi, bins))
    }

    /// Squelch. Sets the level a capture must see before it starts recording, and below. Wire: `w\r\u`
    pub fn squelch(&mut self, dbm: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\u");
        encoding::push_int(&mut cmd, dbm as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Replay Invert. Flips the captured low/high phase before re-keying it on Replay.. Wire: `w\r\v`
    pub fn replay_invert(&mut self, on: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\v");
        encoding::push_int(&mut cmd, on as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Transmit Sub. Transmits a Flipper .sub file from the card.. Wire: `w\r\m`
    pub fn transmit_sub_file(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\m");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Monitor. Keeps the receiver open and samples signal strength continuously, so. Wire: `w\r\l`
    pub fn monitor(&mut self, on: i32) -> Result<(), OwError> {
        let mut cmd = String::from("w\\r\\l");
        encoding::push_int(&mut cmd, on as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("radio1", false, "data_bytes=hexbytes", "Radio 1 received data (hex bytes)"),
    ("radio2", false, "data_bytes=hexbytes", "Radio 2 received data (hex bytes)"),
    ("radioasync", false, "data=string", "Async sub-file transmit/capture status (free-form text)"),
];
