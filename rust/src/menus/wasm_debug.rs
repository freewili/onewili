//! WASM Debug menu - generated from fwMenuWasmDebug. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct WasmDebug<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> WasmDebug<'a> {
    /// WASM Debug Start. Loads a .wilwasm for debugging.. Wire: `s\w\c`
    pub fn debug_start(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\w\\c");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WASM Debug Breakpoints. Replaces the byte-PC breakpoint set.. Wire: `s\w\j`
    pub fn debug_breakpoints(&mut self, pcs: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\w\\j");
        encoding::push_str(&mut cmd, pcs);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WASM Debug Step. Steps one opcode, or until the PC leaves [lo,hi).. Wire: `s\w\e`
    pub fn debug_step(&mut self, range: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\w\\e");
        encoding::push_str(&mut cmd, range);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WASM Debug Continue. Resumes until the next breakpoint.. Wire: `s\w\f`
    pub fn debug_continue(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\w\\f");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WASM Debug Pause. Pauses at the next opcode.. Wire: `s\w\g`
    pub fn debug_pause(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\w\\g");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WASM Debug Stop. Stops the active wasm debug session.. Wire: `s\w\t`
    pub fn debug_stop(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\w\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WASM Debug Locals. Dumps stack frames and raw frame-0 locals.. Wire: `s\w\i`
    pub fn debug_locals(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\w\\i");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WASM Debug Memory Read. Reads up to 64 bytes of wasm linear memory (hex).. Wire: `s\w\r`
    pub fn debug_mem_read(&mut self, addr: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\w\\r");
        encoding::push_str(&mut cmd, addr);
        self.t.call(&cmd)?;
        Ok(())
    }
}
