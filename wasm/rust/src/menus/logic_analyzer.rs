//! Logic Analyzer Functions menu - generated from fwMenuLogicAnalyzer. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct LogicAnalyzer<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> LogicAnalyzer<'a> {
    /// configure. Configures the logic analyzer capture.. Wire: `i\b\c`
    pub fn setup_logic_analyzer(&mut self, sample_rate_ns: i32, sample_count: i32, pin_start: i32, pin_stop: i32, trigger_pin: i32, trigger_type: i32, rearm: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(sample_rate_ns);
        a.i32(sample_count);
        a.i32(pin_start);
        a.i32(pin_stop);
        a.i32(trigger_pin);
        a.i32(trigger_type);
        a.i32(rearm);
        let _r = crate::transport::call(81 /* CMD_IO_LOGIC_ANALYZER_SETUP_LOGIC_ANALYZER */, &a)?;
        Ok(())
    }

    /// configure analog. Configures the analog capture inputs.. Wire: `i\b\a`
    pub fn setup_analog(&mut self, analog_mask: i32, analog_rate_ns: i32, analog_res: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(analog_mask);
        a.i32(analog_rate_ns);
        a.i32(analog_res);
        let _r = crate::transport::call(82 /* CMD_IO_LOGIC_ANALYZER_SETUP_ANALOG */, &a)?;
        Ok(())
    }

    /// start. Starts logic analyzer capture.. Wire: `i\b\s`
    pub fn start(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(83 /* CMD_IO_LOGIC_ANALYZER_START */, &a)?;
        Ok(())
    }

    /// stop. Stops logic analyzer capture.. Wire: `i\b\e`
    pub fn stop(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(84 /* CMD_IO_LOGIC_ANALYZER_STOP */, &a)?;
        Ok(())
    }

    /// trigger. Manually triggers the logic analyzer.. Wire: `i\b\t`
    pub fn trigger(&mut self, trigger_type: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(trigger_type);
        let _r = crate::transport::call(85 /* CMD_IO_LOGIC_ANALYZER_TRIGGER */, &a)?;
        Ok(())
    }
}
