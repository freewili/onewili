//! CANFD Functions menu - generated from fwMenuCANFD. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Canfd<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Canfd<'a> {
    /// Stream CAN(FD). Streams received CAN frames and errors to the host.. Wire: `i\c\o`
    pub fn enable_canfd_stream(&mut self, channel: i32, enabled: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(channel);
        a.i32(enabled);
        let _r = crate::transport::call(58 /* CMD_IO_CANFD_ENABLE_CANFD_STREAM */, &a)?;
        Ok(())
    }

    /// Transmit CAN(FD). Transmits a CAN(FD) frame.. Wire: `i\c\w`
    pub fn write_canfd(&mut self, channel: i32, arb_id: u32, can_fd: i32, xtd_id: i32, data_in: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(channel);
        a.u32(arb_id);
        a.i32(can_fd);
        a.i32(xtd_id);
        a.bytes(data_in);
        let _r = crate::transport::call(59 /* CMD_IO_CANFD_WRITE_CANFD */, &a)?;
        Ok(())
    }

    /// Transmit CAN(FD) Periodic. Transmits a CAN(FD) frame periodically (period in us; 0 = as fast as possible).. Wire: `i\c\p`
    pub fn write_canfd_periodic(&mut self, index: i32, enable: i32, period: i32, channel: i32, arb_id: u32, can_fd: i32, xtd_id: i32, data_in: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(index);
        a.i32(enable);
        a.i32(period);
        a.i32(channel);
        a.u32(arb_id);
        a.i32(can_fd);
        a.i32(xtd_id);
        a.bytes(data_in);
        let _r = crate::transport::call(60 /* CMD_IO_CANFD_WRITE_CANFD_PERIODIC */, &a)?;
        Ok(())
    }

    /// Setup Filter. Sets up a hardware receive filter (the byte-filter args are optional).. Wire: `i\c\f`
    pub fn setup_filter(&mut self, channel: i32, index: i32, enable: i32, xtd_id: i32, mask: u32, accept: u32, maskb0: u32, accept_b0: u32, maskb1: u32, accept_b1: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(channel);
        a.i32(index);
        a.i32(enable);
        a.i32(xtd_id);
        a.u32(mask);
        a.u32(accept);
        a.u32(maskb0);
        a.u32(accept_b0);
        a.u32(maskb1);
        a.u32(accept_b1);
        let _r = crate::transport::call(61 /* CMD_IO_CANFD_SETUP_FILTER */, &a)?;
        Ok(())
    }

    /// Read CAN Register(s). Reads 32-bit words from CAN controller SFR registers.. Wire: `i\c\r`
    pub fn read_can_registers(&mut self, channel: i32, start_address: u32, word_count: i32) -> Result<String, OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(channel);
        a.u32(start_address);
        a.i32(word_count);
        let mut _r = crate::transport::call(62 /* CMD_IO_CANFD_READ_CAN_REGISTERS */, &a)?;
        Ok(_r.string())
    }

    /// Set CAN Register. Sets a CAN controller register.. Wire: `i\c\s`
    pub fn set_can_register(&mut self, channel: i32, start_address: u32, byte_count: i32, word_to_write: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(channel);
        a.u32(start_address);
        a.i32(byte_count);
        a.u32(word_to_write);
        let _r = crate::transport::call(63 /* CMD_IO_CANFD_SET_CAN_REGISTER */, &a)?;
        Ok(())
    }
}
