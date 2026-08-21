//! Orca Communication menu - generated from fwMenuOrcaSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct OrcaSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> OrcaSettings<'a> {
    /// Orca Com over UART. Set Communication protocol for connected Orca device over UART. Wire: `h\s\g\u`
    pub fn orca_com_over_uart(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\g\\u");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
