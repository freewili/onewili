//! WASM Debug menu - generated from fwMenuWasmDebug. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct WasmDebug<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> WasmDebug<'a> {
    /// WASM Debug Start. Loads a .wilwasm for debugging.. Wire: `s\w\c`
    pub fn debug_start(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(366 /* CMD_SCRIPTING_WASM_DEBUG_DEBUG_START */, &a)?;
        Ok(())
    }

    /// WASM Debug Breakpoints. Replaces the byte-PC breakpoint set.. Wire: `s\w\j`
    pub fn debug_breakpoints(&mut self, pcs: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(pcs);
        let _r = crate::transport::call(367 /* CMD_SCRIPTING_WASM_DEBUG_DEBUG_BREAKPOINTS */, &a)?;
        Ok(())
    }

    /// WASM Debug Step. Steps one opcode, or until the PC leaves [lo,hi).. Wire: `s\w\e`
    pub fn debug_step(&mut self, range: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(range);
        let _r = crate::transport::call(368 /* CMD_SCRIPTING_WASM_DEBUG_DEBUG_STEP */, &a)?;
        Ok(())
    }

    /// WASM Debug Continue. Resumes until the next breakpoint.. Wire: `s\w\f`
    pub fn debug_continue(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(369 /* CMD_SCRIPTING_WASM_DEBUG_DEBUG_CONTINUE */, &a)?;
        Ok(())
    }

    /// WASM Debug Pause. Pauses at the next opcode.. Wire: `s\w\g`
    pub fn debug_pause(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(370 /* CMD_SCRIPTING_WASM_DEBUG_DEBUG_PAUSE */, &a)?;
        Ok(())
    }

    /// WASM Debug Stop. Stops the active wasm debug session.. Wire: `s\w\t`
    pub fn debug_stop(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(371 /* CMD_SCRIPTING_WASM_DEBUG_DEBUG_STOP */, &a)?;
        Ok(())
    }

    /// WASM Debug Locals. Dumps stack frames and raw frame-0 locals.. Wire: `s\w\i`
    pub fn debug_locals(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(372 /* CMD_SCRIPTING_WASM_DEBUG_DEBUG_LOCALS */, &a)?;
        Ok(())
    }

    /// WASM Debug Memory Read. Reads up to 64 bytes of wasm linear memory (hex).. Wire: `s\w\r`
    pub fn debug_mem_read(&mut self, addr: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(addr);
        let _r = crate::transport::call(373 /* CMD_SCRIPTING_WASM_DEBUG_DEBUG_MEM_READ */, &a)?;
        Ok(())
    }
}
