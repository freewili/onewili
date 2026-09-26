//! ZoomIO Functions menu - generated from fwMenuZoomIO. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct ZoomIo<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> ZoomIo<'a> {
    /// Stream ZoomIO Data. Enables or disables streaming of ZoomIO receive data to the host.. Wire: `s\b\o`
    pub fn enable_rx_stream(&mut self, enable: i32) -> Result<(), OwError> {
        let mut cmd = String::from("s\\b\\o");
        encoding::push_int(&mut cmd, enable as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Write to FIFO. Sends a single ZoomIO message after the given delay (us).. Wire: `s\b\w`
    pub fn send_data(&mut self, delay: i32, data: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("s\\b\\w");
        encoding::push_int(&mut cmd, delay as i64);
        encoding::push_bytes(&mut cmd, data);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Update Schedule Table. Updates a schedule-table transmit message.. Wire: `s\b\u`
    pub fn update_table_data(&mut self, table_index: i32, delay: i32, data: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("s\\b\\u");
        encoding::push_int(&mut cmd, table_index as i64);
        encoding::push_int(&mut cmd, delay as i64);
        encoding::push_bytes(&mut cmd, data);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Setup Schedule Table. Sets up the schedule table size (0 to disable).. Wire: `s\b\p`
    pub fn enable_schedule_table(&mut self, number_of_entries: i32) -> Result<(), OwError> {
        let mut cmd = String::from("s\\b\\p");
        encoding::push_int(&mut cmd, number_of_entries as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Compile test. Compiles built-in ZoomIO milestone program and launches it on core1 as RISC-V. Wire: `s\b\c`
    pub fn compile_test(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\b\\c");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Run ZoomIO. Compile and run a ZoomIO program on the RISC-V core1. Wire: `s\b\r`
    pub fn run_zio(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\b\\r");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stop ZoomIO. Reset core1 to stop the running program. Wire: `s\b\s`
    pub fn stop_zio(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\b\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Probe Exec Window. Stages a known pattern in ZoomIO's SCRATCH_X exec window, runs the full core1 launch sequence, and reads it back. Wire: `s\b\x`
    pub fn exec_probe(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\b\\x");
        self.t.call(&cmd)?;
        Ok(())
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("zoomio", false, "data_bytes=hexbytes", "ZoomIO received packet (hex bytes)"),
];
