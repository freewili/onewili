//! ZoomIO Functions menu - generated from fwMenuZoomIO. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct ZoomIo<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> ZoomIo<'a> {
    /// Stream ZoomIO Data. Enables or disables streaming of ZoomIO receive data to the host.. Wire: `s\b\o`
    pub fn enable_rx_stream(&mut self, enable: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        let _r = crate::transport::call(359 /* CMD_SCRIPTING_ZOOM_IO_ENABLE_RX_STREAM */, &a)?;
        Ok(())
    }

    /// Write to FIFO. Sends a single ZoomIO message after the given delay (us).. Wire: `s\b\w`
    pub fn send_data(&mut self, delay: i32, data: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(delay);
        a.bytes(data);
        let _r = crate::transport::call(360 /* CMD_SCRIPTING_ZOOM_IO_SEND_DATA */, &a)?;
        Ok(())
    }

    /// Update Schedule Table. Updates a schedule-table transmit message.. Wire: `s\b\u`
    pub fn update_table_data(&mut self, table_index: i32, delay: i32, data: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(table_index);
        a.i32(delay);
        a.bytes(data);
        let _r = crate::transport::call(361 /* CMD_SCRIPTING_ZOOM_IO_UPDATE_TABLE_DATA */, &a)?;
        Ok(())
    }

    /// Setup Schedule Table. Sets up the schedule table size (0 to disable).. Wire: `s\b\p`
    pub fn enable_schedule_table(&mut self, number_of_entries: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(number_of_entries);
        let _r = crate::transport::call(362 /* CMD_SCRIPTING_ZOOM_IO_ENABLE_SCHEDULE_TABLE */, &a)?;
        Ok(())
    }

    /// Compile test. Compiles built-in ZoomIO milestone program and launches it on core1 as RISC-V. Wire: `s\b\c`
    pub fn compile_test(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(363 /* CMD_SCRIPTING_ZOOM_IO_COMPILE_TEST */, &a)?;
        Ok(())
    }

    /// Run ZoomIO. Compile and run a ZoomIO program on the RISC-V core1. Wire: `s\b\r`
    pub fn run_zio(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(364 /* CMD_SCRIPTING_ZOOM_IO_RUN_ZIO */, &a)?;
        Ok(())
    }

    /// Stop ZoomIO. Reset core1 to stop the running program. Wire: `s\b\s`
    pub fn stop_zio(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(365 /* CMD_SCRIPTING_ZOOM_IO_STOP_ZIO */, &a)?;
        Ok(())
    }
}
