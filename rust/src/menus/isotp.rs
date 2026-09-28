//! ISO-TP Transport menu - generated from fwMenuISOTP. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Isotp<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Isotp<'a> {
    /// Enable ISO-TP. Arms (1) or disarms (0) the ISO-TP transport on CAN channel 0: takes the PSRAM staging window, taps received frames and starts answering flow control for messages sent to rxId. Send Message and Send File arm it automatically.. Wire: `i\c\t\e`
    pub fn iso_tp_enable(&mut self, enable: bool) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\t\\e");
        encoding::push_bool(&mut cmd, enable);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Configure Addressing. Sets the CAN ids the transport sends on and listens to, 11/29-bit ids, classic CAN or CAN FD, the TX_DL frame size (8 classic; 8,12,16,20,24,32,48,64 FD), padding and pad byte, and normal (0) or extended (1) addressing with its N_TA byte.. Wire: `i\c\t\c`
    pub fn iso_tp_configure_addressing(&mut self, tx_id: u32, rx_id: u32, extended_id: bool, can_fd: bool, tx_data_length: i32, padding: bool, pad_byte: u32, addressing_mode: i32, ext_address: u32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\t\\c");
        encoding::push_hex(&mut cmd, tx_id as u64, 8);
        encoding::push_hex(&mut cmd, rx_id as u64, 8);
        encoding::push_bool(&mut cmd, extended_id);
        encoding::push_bool(&mut cmd, can_fd);
        encoding::push_int(&mut cmd, tx_data_length as i64);
        encoding::push_bool(&mut cmd, padding);
        encoding::push_hex(&mut cmd, pad_byte as u64, 8);
        encoding::push_int(&mut cmd, addressing_mode as i64);
        encoding::push_hex(&mut cmd, ext_address as u64, 8);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Configure Flow Control. Sets what this device advertises in its own flow control frames when receiving -- block size (0 = no limit) and the STmin byte (00-7F ms, F1-F9 = 100-900 us) -- and how many consecutive WAIT frames it tolerates from the peer when sending (default 8).. Wire: `i\c\t\f`
    pub fn iso_tp_configure_flow_control(&mut self, block_size: i32, st_min: u32, wft_max: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\t\\f");
        encoding::push_int(&mut cmd, block_size as i64);
        encoding::push_hex(&mut cmd, st_min as u64, 8);
        encoding::push_int(&mut cmd, wft_max as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set STmin Trim. Adjusts how this device paces its consecutive frames: stMinTrimUs is a signed number of microseconds added to the peer's STmin (negative values cancel the SPI write latency of about 100 us); stMinOverrideUs ignores the peer's STmin and paces at exactly that many microseconds (+ trim), -1 follows the peer.. Wire: `i\c\t\t`
    pub fn iso_tp_set_st_min_trim(&mut self, st_min_trim_us: i32, st_min_override_us: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\t\\t");
        encoding::push_int(&mut cmd, st_min_trim_us as i64);
        encoding::push_int(&mut cmd, st_min_override_us as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Send Message. Sends up to 256 bytes as one ISO-TP message (single frame, or first frame + flow-controlled consecutive frames) and blocks until it is delivered, aborted or timed out; returns the result code (0 = Ok), bytes and frames sent, the duration and the measured consecutive-frame gaps in microseconds.. Wire: `i\c\t\s`
    pub fn iso_tp_send_message(&mut self, data: &[u8]) -> Result<(i32, i32, i32, i32, i32, i32, i32), OwError> {
        let mut cmd = String::from("i\\c\\t\\s");
        encoding::push_bytes(&mut cmd, data);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let result = encoding::tok_int(&mut toks)? as i32;
        let bytes = encoding::tok_int(&mut toks)? as i32;
        let frames = encoding::tok_int(&mut toks)? as i32;
        let duration_us = encoding::tok_int(&mut toks)? as i32;
        let min_gap_us = encoding::tok_int(&mut toks)? as i32;
        let max_gap_us = encoding::tok_int(&mut toks)? as i32;
        let avg_gap_us = encoding::tok_int(&mut toks)? as i32;
        Ok((result, bytes, frames, duration_us, min_gap_us, max_gap_us, avg_gap_us))
    }

    /// Send File. Sends the whole content of an SD card file as one ISO-TP message, paging it from the card through a 16 KiB PSRAM window while the peer is not waiting on a frame; blocks like Send Message and returns the same result fields.. Wire: `i\c\t\x`
    pub fn iso_tp_send_file(&mut self, file_path: &str) -> Result<(i32, i32, i32, i32, i32, i32, i32), OwError> {
        let mut cmd = String::from("i\\c\\t\\x");
        encoding::push_str(&mut cmd, file_path);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let result = encoding::tok_int(&mut toks)? as i32;
        let bytes = encoding::tok_int(&mut toks)? as i32;
        let frames = encoding::tok_int(&mut toks)? as i32;
        let duration_us = encoding::tok_int(&mut toks)? as i32;
        let min_gap_us = encoding::tok_int(&mut toks)? as i32;
        let max_gap_us = encoding::tok_int(&mut toks)? as i32;
        let avg_gap_us = encoding::tok_int(&mut toks)? as i32;
        Ok((result, bytes, frames, duration_us, min_gap_us, max_gap_us, avg_gap_us))
    }

    /// Receive Message. Reports the last ISO-TP message received on rxId: status 0 none, 1 complete, 2 receiving, 3 error; length; inFile=1 when it was longer than 256 bytes and was written to the receive file (then data is empty); otherwise the payload bytes in hex. Reading consumes the message.. Wire: `i\c\t\r`
    pub fn iso_tp_receive_message(&mut self) -> Result<(i32, i32, i32, Vec<u8>), OwError> {
        let cmd = String::from("i\\c\\t\\r");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let status = encoding::tok_int(&mut toks)? as i32;
        let length = encoding::tok_int(&mut toks)? as i32;
        let in_file = encoding::tok_int(&mut toks)? as i32;
        let data = encoding::rest_bytes(&mut toks)?;
        Ok((status, length, in_file, data))
    }

    /// Set Receive File Path. Sets where received ISO-TP messages longer than 256 bytes are written on the SD card (default /isotp/rx.bin); the directory is created when the first such message arrives.. Wire: `i\c\t\p`
    pub fn iso_tp_set_receive_file_path(&mut self, file_path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\t\\p");
        encoding::push_str(&mut cmd, file_path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Abort. Terminates whatever ISO-TP transfer is in progress without sending anything, closes any open card file and leaves the transport armed.. Wire: `i\c\t\a`
    pub fn iso_tp_abort(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\c\\t\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Show Status. Reports the transport state (0 idle, 1 waiting for flow control, 2 sending consecutive frames, 3 waiting for a frame to finish, 4 receiving), the last result code (0 = Ok), and the running counts of messages received, sent and failed since power-up.. Wire: `i\c\t\i`
    pub fn iso_tp_show_status(&mut self) -> Result<(i32, i32, i32, i32, i32), OwError> {
        let cmd = String::from("i\\c\\t\\i");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let state = encoding::tok_int(&mut toks)? as i32;
        let last_result = encoding::tok_int(&mut toks)? as i32;
        let rx_count = encoding::tok_int(&mut toks)? as i32;
        let tx_count = encoding::tok_int(&mut toks)? as i32;
        let errors = encoding::tok_int(&mut toks)? as i32;
        Ok((state, last_result, rx_count, tx_count, errors))
    }
}
