//! ISO-TP Transport menu - generated from fwMenuISOTP. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Isotp<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Isotp<'a> {
    /// Enable ISO-TP. Arms (1) or disarms (0) the ISO-TP transport on CAN channel 0: takes the PSRAM staging window, taps received frames and starts answering flow control for messages sent to rxId. Send Message and Send File arm it automatically.. Wire: `i\c\t\e`
    pub fn iso_tp_enable(&mut self, enable: bool) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(if enable { 1 } else { 0 });
        let _r = crate::transport::call(614 /* CMD_IO_CANFD_ISOTP_ISO_TP_ENABLE */, &a)?;
        Ok(())
    }

    /// Configure Addressing. Sets the CAN ids the transport sends on and listens to, 11/29-bit ids, classic CAN or CAN FD, the TX_DL frame size (8 classic; 8,12,16,20,24,32,48,64 FD), padding and pad byte, and normal (0) or extended (1) addressing with its N_TA byte.. Wire: `i\c\t\c`
    pub fn iso_tp_configure_addressing(&mut self, tx_id: u32, rx_id: u32, extended_id: bool, can_fd: bool, tx_data_length: i32, padding: bool, pad_byte: u32, addressing_mode: i32, ext_address: u32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u32(tx_id);
        a.u32(rx_id);
        a.u8(if extended_id { 1 } else { 0 });
        a.u8(if can_fd { 1 } else { 0 });
        a.i32(tx_data_length);
        a.u8(if padding { 1 } else { 0 });
        a.u32(pad_byte);
        a.i32(addressing_mode);
        a.u32(ext_address);
        let _r = crate::transport::call(615 /* CMD_IO_CANFD_ISOTP_ISO_TP_CONFIGURE_ADDRESSING */, &a)?;
        Ok(())
    }

    /// Configure Flow Control. Sets what this device advertises in its own flow control frames when receiving -- block size (0 = no limit) and the STmin byte (00-7F ms, F1-F9 = 100-900 us) -- and how many consecutive WAIT frames it tolerates from the peer when sending (default 8).. Wire: `i\c\t\f`
    pub fn iso_tp_configure_flow_control(&mut self, block_size: i32, st_min: u32, wft_max: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(block_size);
        a.u32(st_min);
        a.i32(wft_max);
        let _r = crate::transport::call(616 /* CMD_IO_CANFD_ISOTP_ISO_TP_CONFIGURE_FLOW_CONTROL */, &a)?;
        Ok(())
    }

    /// Set STmin Trim. Adjusts how this device paces its consecutive frames: stMinTrimUs is a signed number of microseconds added to the peer's STmin (negative values cancel the SPI write latency of about 100 us); stMinOverrideUs ignores the peer's STmin and paces at exactly that many microseconds (+ trim), -1 follows the peer.. Wire: `i\c\t\t`
    pub fn iso_tp_set_st_min_trim(&mut self, st_min_trim_us: i32, st_min_override_us: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(st_min_trim_us);
        a.i32(st_min_override_us);
        let _r = crate::transport::call(617 /* CMD_IO_CANFD_ISOTP_ISO_TP_SET_ST_MIN_TRIM */, &a)?;
        Ok(())
    }

    /// Send Message. Sends up to 256 bytes as one ISO-TP message (single frame, or first frame + flow-controlled consecutive frames) and blocks until it is delivered, aborted or timed out; returns the result code (0 = Ok), bytes and frames sent, the duration and the measured consecutive-frame gaps in microseconds.. Wire: `i\c\t\s`
    pub fn iso_tp_send_message(&mut self, data: &[u8]) -> Result<(i32, i32, i32, i32, i32, i32, i32), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(data);
        let mut _r = crate::transport::call(618 /* CMD_IO_CANFD_ISOTP_ISO_TP_SEND_MESSAGE */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32()))
    }

    /// Send File. Sends the whole content of an SD card file as one ISO-TP message, paging it from the card through a 16 KiB PSRAM window while the peer is not waiting on a frame; blocks like Send Message and returns the same result fields.. Wire: `i\c\t\x`
    pub fn iso_tp_send_file(&mut self, file_path: &str) -> Result<(i32, i32, i32, i32, i32, i32, i32), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(file_path);
        let mut _r = crate::transport::call(619 /* CMD_IO_CANFD_ISOTP_ISO_TP_SEND_FILE */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32()))
    }

    /// Receive Message. Reports the last ISO-TP message received on rxId: status 0 none, 1 complete, 2 receiving, 3 error; length; inFile=1 when it was longer than 256 bytes and was written to the receive file (then data is empty); otherwise the payload bytes in hex. Reading consumes the message.. Wire: `i\c\t\r`
    pub fn iso_tp_receive_message(&mut self) -> Result<(i32, i32, i32, Vec<u8>), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(620 /* CMD_IO_CANFD_ISOTP_ISO_TP_RECEIVE_MESSAGE */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.i32(), _r.bytes()))
    }

    /// Set Receive File Path. Sets where received ISO-TP messages longer than 256 bytes are written on the SD card (default /isotp/rx.bin); the directory is created when the first such message arrives.. Wire: `i\c\t\p`
    pub fn iso_tp_set_receive_file_path(&mut self, file_path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(file_path);
        let _r = crate::transport::call(621 /* CMD_IO_CANFD_ISOTP_ISO_TP_SET_RECEIVE_FILE_PATH */, &a)?;
        Ok(())
    }

    /// Abort. Terminates whatever ISO-TP transfer is in progress without sending anything, closes any open card file and leaves the transport armed.. Wire: `i\c\t\a`
    pub fn iso_tp_abort(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(622 /* CMD_IO_CANFD_ISOTP_ISO_TP_ABORT */, &a)?;
        Ok(())
    }

    /// Show Status. Reports the transport state (0 idle, 1 waiting for flow control, 2 sending consecutive frames, 3 waiting for a frame to finish, 4 receiving), the last result code (0 = Ok), and the running counts of messages received, sent and failed since power-up.. Wire: `i\c\t\i`
    pub fn iso_tp_show_status(&mut self) -> Result<(i32, i32, i32, i32, i32), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(623 /* CMD_IO_CANFD_ISOTP_ISO_TP_SHOW_STATUS */, &a)?;
        Ok((_r.i32(), _r.i32(), _r.i32(), _r.i32(), _r.i32()))
    }
}
