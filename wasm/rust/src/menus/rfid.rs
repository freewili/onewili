//! RFID Functions menu - generated from fwMenuRFID. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Rfid<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Rfid<'a> {
    /// Enable Reader. Start or stop the 125 kHz carrier and tag reader. Wire: `w\p\r`
    pub fn enable_reader(&mut self, enable: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        let _r = crate::transport::call(534 /* CMD_WIRELESS_RFID_ENABLE_READER */, &a)?;
        Ok(())
    }

    /// Get Status. Reader state, carrier frequency and live envelope. Wire: `w\p\g`
    pub fn get_status(&mut self) -> Result<(i32, u8, i32, i32, i32, i32, i32, i32), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(529 /* CMD_WIRELESS_RFID_GET_STATUS */, &a)?;
        Ok((_r.i32(), _r.u8(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32()))
    }

    /// Read Tag. Block until one tag is decoded or the timeout expires. Wire: `w\p\t`
    pub fn read_tag(&mut self, timeout_ms: i32) -> Result<(i32, i32, Vec<u8>), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(timeout_ms);
        let mut _r = crate::transport::call(536 /* CMD_WIRELESS_RFID_READ_TAG */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.bytes()))
    }

    /// Stream Tags. Push each decoded tag to the host as an event. Wire: `w\p\s`
    pub fn stream_tags(&mut self, enable: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        let _r = crate::transport::call(535 /* CMD_WIRELESS_RFID_STREAM_TAGS */, &a)?;
        Ok(())
    }

    /// Clear Stats. Zero the frame and tag counters. Wire: `w\p\c`
    pub fn clear_stats(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(528 /* CMD_WIRELESS_RFID_CLEAR_STATS */, &a)?;
        Ok(())
    }

    /// Tune Constant. Set a demodulator constant live, without reflashing. Wire: `w\p\u`
    pub fn tune(&mut self, param: i32, value: i32) -> Result<(i32, i32), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(param);
        a.i32(value);
        let mut _r = crate::transport::call(537 /* CMD_WIRELESS_RFID_TUNE */, &a)?;
        Ok((_r.i32(), _r.i32()))
    }

    /// Raw Bits. Raw bits of the last assembled frame. Wire: `w\p\b`
    pub fn raw_bits(&mut self) -> Result<(i32, i32, Vec<u8>), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(527 /* CMD_WIRELESS_RFID_RAW_BITS */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.bytes()))
    }

    /// Write Tag. Write one 32-bit block to a T5577/T5557 tag. Wire: `w\p\w`
    pub fn write_tag(&mut self, block: i32, value: u32) -> Result<(i32, i32), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(block);
        a.u32(value);
        let mut _r = crate::transport::call(538 /* CMD_WIRELESS_RFID_WRITE_TAG */, &a)?;
        Ok((_r.i32(), _r.i32()))
    }

    /// Carrier Info. Measured carrier and PSK front-end telemetry. Wire: `w\p\i`
    pub fn carrier_info(&mut self) -> Result<(i32, bool, i32, i32, i32, bool, i32, i32, i32, i32), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(530 /* CMD_WIRELESS_RFID_CARRIER_INFO */, &a)?;
        Ok((_r.i32(), _r.u8() != 0, _r.i32(), _r.i32(), _r.i32(), _r.u8() != 0, _r.i32(), _r.i32(), _r.i32(), _r.i32()))
    }

    /// Enroll ID. Write a caller-supplied EM4100 ID onto the card in the field. Wire: `w\p\n`
    pub fn enroll_id(&mut self, id: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(id);
        let _r = crate::transport::call(533 /* CMD_WIRELESS_RFID_ENROLL_ID */, &a)?;
        Ok(())
    }

    /// Clone Capture. Read a card and hold its ID for a later clone write. Wire: `w\p\k`
    pub fn clone_capture(&mut self, timeout_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(timeout_ms);
        let _r = crate::transport::call(532 /* CMD_WIRELESS_RFID_CLONE_CAPTURE */, &a)?;
        Ok(())
    }

    /// Clone Write. Write the captured ID onto the card now on the coil. Wire: `w\p\j`
    pub fn clone_write(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(531 /* CMD_WIRELESS_RFID_CLONE_WRITE */, &a)?;
        Ok(())
    }
}
