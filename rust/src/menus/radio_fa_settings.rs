//! RF Analyzer Settings menu - generated from fwMenuRadioFASettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct RadioFaSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> RadioFaSettings<'a> {
    /// Default View. Default view for the RF Analyzer. Wire: `h\s\a\a`
    pub fn default_view(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\a\\a");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
