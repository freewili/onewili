//! CANFD Functions menu - generated from fwMenuCANFD. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Canfd<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Canfd<'a> {
    /// Stream CAN(FD). Streams received CAN frames and errors to the host.. Wire: `i\c\o`
    pub fn enable_canfd_stream(&mut self, channel: i32, enabled: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\o");
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_int(&mut cmd, enabled as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Transmit CAN(FD). Transmits a CAN(FD) frame.. Wire: `i\c\w`
    pub fn write_canfd(&mut self, channel: i32, arb_id: u32, can_fd: i32, xtd_id: i32, data_in: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\w");
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_hex(&mut cmd, arb_id as u64, 8);
        encoding::push_int(&mut cmd, can_fd as i64);
        encoding::push_int(&mut cmd, xtd_id as i64);
        encoding::push_bytes(&mut cmd, data_in);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Transmit CAN(FD) Periodic. Transmits a CAN(FD) frame periodically (period in us; 0 = as fast as possible).. Wire: `i\c\p`
    pub fn write_canfd_periodic(&mut self, index: i32, enable: i32, period: i32, channel: i32, arb_id: u32, can_fd: i32, xtd_id: i32, data_in: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\p");
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, enable as i64);
        encoding::push_int(&mut cmd, period as i64);
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_hex(&mut cmd, arb_id as u64, 8);
        encoding::push_int(&mut cmd, can_fd as i64);
        encoding::push_int(&mut cmd, xtd_id as i64);
        encoding::push_bytes(&mut cmd, data_in);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Setup Filter. Sets up a hardware receive filter (the byte-filter args are optional).. Wire: `i\c\f`
    pub fn setup_filter(&mut self, channel: i32, index: i32, enable: i32, xtd_id: i32, mask: u32, accept: u32, maskb0: u32, accept_b0: u32, maskb1: u32, accept_b1: u32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\f");
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_int(&mut cmd, index as i64);
        encoding::push_int(&mut cmd, enable as i64);
        encoding::push_int(&mut cmd, xtd_id as i64);
        encoding::push_hex(&mut cmd, mask as u64, 8);
        encoding::push_hex(&mut cmd, accept as u64, 8);
        encoding::push_hex(&mut cmd, maskb0 as u64, 8);
        encoding::push_hex(&mut cmd, accept_b0 as u64, 8);
        encoding::push_hex(&mut cmd, maskb1 as u64, 8);
        encoding::push_hex(&mut cmd, accept_b1 as u64, 8);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Read CAN Register(s). Reads 32-bit words from CAN controller SFR registers.. Wire: `i\c\r`
    pub fn read_can_registers(&mut self, channel: i32, start_address: u32, word_count: i32) -> Result<String, OwError> {
        let mut cmd = String::from("i\\c\\r");
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_hex(&mut cmd, start_address as u64, 8);
        encoding::push_int(&mut cmd, word_count as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let registers = encoding::rest_str(&mut toks);
        Ok(registers)
    }

    /// Set CAN Register. Sets a CAN controller register.. Wire: `i\c\s`
    pub fn set_can_register(&mut self, channel: i32, start_address: u32, byte_count: i32, word_to_write: u32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\c\\s");
        encoding::push_int(&mut cmd, channel as i64);
        encoding::push_hex(&mut cmd, start_address as u64, 8);
        encoding::push_int(&mut cmd, byte_count as i64);
        encoding::push_hex(&mut cmd, word_to_write as u64, 8);
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("can0", false, "arb_id=string,data_bytes=hexbytes", "CAN RX frame on channel 0 (hex arb id, 'x' suffix = extended, then hex data)"),
    ("can1", false, "arb_id=string,data_bytes=hexbytes", "CAN RX frame on channel 1 (hex arb id, 'x' suffix = extended, then hex data)"),
    ("canTx0", false, "arb_id=string,data_bytes=hexbytes", "CAN TX echo on channel 0 (hex arb id, 'x' suffix = extended, then hex data)"),
    ("canTx1", false, "arb_id=string,data_bytes=hexbytes", "CAN TX echo on channel 1 (hex arb id, 'x' suffix = extended, then hex data)"),
    ("canRxReport", true, "time_stamp_ns=hexU64,gpio_bitfield=hexU32,can_id=hexU32,header_bits=hexU32,data_words=hexbytes", "CAN RX frame report (binary API, MCP2518 memory-map layout)"),
];
