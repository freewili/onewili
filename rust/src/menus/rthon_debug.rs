//! rThon Debug menu - generated from fwMenuRthonDebug. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct RthonDebug<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> RthonDebug<'a> {
    /// Debug Start. Loads and compiles a script for debugging.. Wire: `s\r\c`
    pub fn debug_start(&mut self, path: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\r\\c");
        encoding::push_str(&mut cmd, path);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Debug Breakpoints. Replaces the breakpoint set for the active debug session.. Wire: `s\r\j`
    pub fn debug_breakpoints(&mut self, lines: &str) -> Result<(), OwError> {
        let mut cmd = String::from("s\\r\\j");
        encoding::push_str(&mut cmd, lines);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Debug Step. Single-steps the active debug session.. Wire: `s\r\e`
    pub fn debug_step(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\r\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Debug Continue. Resumes the active debug session until the next breakpoint.. Wire: `s\r\f`
    pub fn debug_continue(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\r\\f");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Debug Pause. Pauses the active debug session at the next statement.. Wire: `s\r\g`
    pub fn debug_pause(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\r\\g");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Debug Stop. Stops the active debug session.. Wire: `s\r\t`
    pub fn debug_stop(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\r\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Debug Locals. Dumps the local variables of the active debug session.. Wire: `s\r\i`
    pub fn debug_locals(&mut self) -> Result<(), OwError> {
        let cmd = String::from("s\\r\\i");
        self.t.call(&cmd)?;
        Ok(())
    }
}
