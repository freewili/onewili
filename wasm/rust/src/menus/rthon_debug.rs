//! rThon Debug menu - generated from fwMenuRthonDebug. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct RthonDebug<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> RthonDebug<'a> {
    /// Debug Start. Loads and compiles a script for debugging.. Wire: `s\r\c`
    pub fn debug_start(&mut self, path: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(path);
        let _r = crate::transport::call(374 /* CMD_SCRIPTING_RTHON_DEBUG_DEBUG_START */, &a)?;
        Ok(())
    }

    /// Debug Breakpoints. Replaces the breakpoint set for the active debug session.. Wire: `s\r\j`
    pub fn debug_breakpoints(&mut self, lines: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(lines);
        let _r = crate::transport::call(375 /* CMD_SCRIPTING_RTHON_DEBUG_DEBUG_BREAKPOINTS */, &a)?;
        Ok(())
    }

    /// Debug Step. Single-steps the active debug session.. Wire: `s\r\e`
    pub fn debug_step(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(376 /* CMD_SCRIPTING_RTHON_DEBUG_DEBUG_STEP */, &a)?;
        Ok(())
    }

    /// Debug Continue. Resumes the active debug session until the next breakpoint.. Wire: `s\r\f`
    pub fn debug_continue(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(377 /* CMD_SCRIPTING_RTHON_DEBUG_DEBUG_CONTINUE */, &a)?;
        Ok(())
    }

    /// Debug Pause. Pauses the active debug session at the next statement.. Wire: `s\r\g`
    pub fn debug_pause(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(378 /* CMD_SCRIPTING_RTHON_DEBUG_DEBUG_PAUSE */, &a)?;
        Ok(())
    }

    /// Debug Stop. Stops the active debug session.. Wire: `s\r\t`
    pub fn debug_stop(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(379 /* CMD_SCRIPTING_RTHON_DEBUG_DEBUG_STOP */, &a)?;
        Ok(())
    }

    /// Debug Locals. Dumps the local variables of the active debug session.. Wire: `s\r\i`
    pub fn debug_locals(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(380 /* CMD_SCRIPTING_RTHON_DEBUG_DEBUG_LOCALS */, &a)?;
        Ok(())
    }
}
