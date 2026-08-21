//! Radio menu - generated from fwMenuRadio. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Radio<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Radio<'a> {
    /// Select Circuit. Claims the sub-GHz front end for this client and holds it until Release.. Wire: `w\r\s`
    pub fn select_circuit(&mut self, band: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(band);
        let _r = crate::transport::call(430 /* CMD_WIRELESS_RADIO_SELECT_CIRCUIT */, &a)?;
        Ok(())
    }

    /// Release Circuit. Ends this client's circuit hold and hands the antenna back to LoRa.. Wire: `w\r\e`
    pub fn release_circuit(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(418 /* CMD_WIRELESS_RADIO_RELEASE_CIRCUIT */, &a)?;
        Ok(())
    }

    /// State. Reads the mux and radio state back, in this order:. Wire: `w\r\t`
    pub fn read_state(&mut self) -> Result<(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, u8), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(431 /* CMD_WIRELESS_RADIO_READ_STATE */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.u8()))
    }

    /// Band. Forces the matched antenna path now: 1 low, 2 mid, 3 high.. Wire: `w\r\b`
    pub fn select_band(&mut self, band: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(band);
        let _r = crate::transport::call(416 /* CMD_WIRELESS_RADIO_SELECT_BAND */, &a)?;
        Ok(())
    }

    /// Comm Check. Reads the CC1101 version register and returns it.. Wire: `w\r\c`
    pub fn comm_check(&mut self) -> Result<u8, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(417 /* CMD_WIRELESS_RADIO_COMM_CHECK */, &a)?;
        Ok(_r.u8())
    }

    /// Frequency. Tunes the CC1101 and selects the matched antenna path for that band.. Wire: `w\r\f`
    pub fn set_frequency(&mut self, freq_hz: i32) -> Result<i32, OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(freq_hz);
        let mut _r = crate::transport::call(419 /* CMD_WIRELESS_RADIO_SET_FREQUENCY */, &a)?;
        Ok(_r.i32())
    }

    /// RSSI. Samples received signal strength once, in dBm.. Wire: `w\r\i`
    pub fn read_rssi(&mut self) -> Result<i32, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(421 /* CMD_WIRELESS_RADIO_READ_RSSI */, &a)?;
        Ok(_r.i32())
    }

    /// Carrier. Keys or unkeys an unmodulated carrier at the current frequency.. Wire: `w\r\o`
    pub fn carrier(&mut self, on: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(on);
        let _r = crate::transport::call(427 /* CMD_WIRELESS_RADIO_CARRIER */, &a)?;
        Ok(())
    }

    /// RX Enable. Puts the CC1101 into continuous receive, or back to idle.. Wire: `w\r\r`
    pub fn rx_enable(&mut self, on: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(on);
        let _r = crate::transport::call(429 /* CMD_WIRELESS_RADIO_RX_ENABLE */, &a)?;
        Ok(())
    }

    /// Idle. Returns the CC1101 to idle from receive, transmit or carrier.. Wire: `w\r\w`
    pub fn idle(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(434 /* CMD_WIRELESS_RADIO_IDLE */, &a)?;
        Ok(())
    }

    /// Packet Send. Transmits one GFSK packet through the CC1101 packet engine.. Wire: `w\r\x`
    pub fn packet_send(&mut self, freq_hz: i32, data: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(freq_hz);
        a.bytes(data);
        let _r = crate::transport::call(435 /* CMD_WIRELESS_RADIO_PACKET_SEND */, &a)?;
        Ok(())
    }

    /// Packet RX. Opens or closes the GFSK packet receiver at the given frequency.. Wire: `w\r\y`
    pub fn packet_rx(&mut self, on: i32, freq_hz: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(on);
        a.i32(freq_hz);
        let _r = crate::transport::call(436 /* CMD_WIRELESS_RADIO_PACKET_RX */, &a)?;
        Ok(())
    }

    /// Packet Read. Reads the last received GFSK packet: RSSI in dBm, a sequence counter that. Wire: `w\r\k`
    pub fn packet_read(&mut self) -> Result<(i32, i32, Vec<u8>), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(423 /* CMD_WIRELESS_RADIO_PACKET_READ */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.bytes()))
    }

    /// Capture Start. Arms a raw pulse-duration capture at the given frequency.. Wire: `w\r\g`
    pub fn capture_start(&mut self, freq_hz: i32, preset: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(freq_hz);
        a.i32(preset);
        let _r = crate::transport::call(420 /* CMD_WIRELESS_RADIO_CAPTURE_START */, &a)?;
        Ok(())
    }

    /// Capture Stop. Ends a capture and returns how many pulse durations it recorded.. Wire: `w\r\j`
    pub fn capture_stop(&mut self) -> Result<i32, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(422 /* CMD_WIRELESS_RADIO_CAPTURE_STOP */, &a)?;
        Ok(_r.i32())
    }

    /// Replay. Re-transmits the last capture out the transmit path.. Wire: `w\r\p`
    pub fn replay(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(428 /* CMD_WIRELESS_RADIO_REPLAY */, &a)?;
        Ok(())
    }

    /// Analyzer. Starts or stops the background frequency-analyzer sweep.. Wire: `w\r\a`
    pub fn analyzer(&mut self, on: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(on);
        let _r = crate::transport::call(415 /* CMD_WIRELESS_RADIO_ANALYZER */, &a)?;
        Ok(())
    }

    /// Spectrum. Reads the analyzer's results: the peak frequency in Hz, its level in dBm,. Wire: `w\r\n`
    pub fn spectrum(&mut self) -> Result<(i32, i32, Vec<u8>), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(426 /* CMD_WIRELESS_RADIO_SPECTRUM */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.bytes()))
    }

    /// Squelch. Sets the level a capture must see before it starts recording, and below. Wire: `w\r\u`
    pub fn squelch(&mut self, dbm: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(dbm);
        let _r = crate::transport::call(432 /* CMD_WIRELESS_RADIO_SQUELCH */, &a)?;
        Ok(())
    }

    /// Replay Invert. Flips the captured low/high phase before re-keying it on Replay.. Wire: `w\r\v`
    pub fn replay_invert(&mut self, on: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(on);
        let _r = crate::transport::call(433 /* CMD_WIRELESS_RADIO_REPLAY_INVERT */, &a)?;
        Ok(())
    }

    /// Transmit Sub. Transmits a Flipper .sub file from the card.. Wire: `w\r\m`
    pub fn transmit_sub_file(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(425 /* CMD_WIRELESS_RADIO_TRANSMIT_SUB_FILE */, &a)?;
        Ok(())
    }

    /// Monitor. Keeps the receiver open and samples signal strength continuously, so. Wire: `w\r\l`
    pub fn monitor(&mut self, on: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(on);
        let _r = crate::transport::call(424 /* CMD_WIRELESS_RADIO_MONITOR */, &a)?;
        Ok(())
    }
}
