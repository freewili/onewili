//! Power Management menu - generated from fwMenuPowerManagement. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::enums::resetLineState;
use crate::transport::OwError;

pub struct PowerManagement<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> PowerManagement<'a> {
    /// List Zones. Lists all 17 power zones with their name and rail, then the three control lines (18-20).. Wire: `h\p\l`
    pub fn list_zones(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(305 /* CMD_HARDWARE_POWER_MANAGEMENT_LIST_ZONES */, &a)?;
        Ok(())
    }

    /// Get Zones. Shows which power zones are currently on, then the reset state of the three control lines.. Wire: `h\p\g`
    pub fn get_zones(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(306 /* CMD_HARDWARE_POWER_MANAGEMENT_GET_ZONES */, &a)?;
        Ok(())
    }

    /// Set Zone. Switches one power zone on or off. Zone 9 is the board-manager LED, not a power rail; zones 18-20 are reset lines with their own commands.. Wire: `h\p\s`
    pub fn set_zone(&mut self, zone: i32, on: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(zone);
        a.i32(on);
        let _r = crate::transport::call(307 /* CMD_HARDWARE_POWER_MANAGEMENT_SET_ZONE */, &a)?;
        Ok(())
    }

    /// Set Zone Mask. Sets every user-controllable zone at once from a bit mask; bit 0 is zone 1.. Wire: `h\p\m`
    pub fn set_zone_mask(&mut self, mask: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(mask);
        let _r = crate::transport::call(308 /* CMD_HARDWARE_POWER_MANAGEMENT_SET_ZONE_MASK */, &a)?;
        Ok(())
    }

    /// Get Power State. Prints the most recent power telemetry sample.. Wire: `h\p\t`
    pub fn get_power_state(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(309 /* CMD_HARDWARE_POWER_MANAGEMENT_GET_POWER_STATE */, &a)?;
        Ok(())
    }

    /// Stream Power. Streams battery, charger and power-zone telemetry to the host at the given rate. 0 stops the stream.. Wire: `h\p\o`
    pub fn enable_power_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(stream_rate_ms);
        let _r = crate::transport::call(310 /* CMD_HARDWARE_POWER_MANAGEMENT_ENABLE_POWER_STREAM */, &a)?;
        Ok(())
    }

    /// Set WIO Reset Line. Holds or releases the LoRa module's reset line (zone 18, WIO_RST). 1 lets the module run, 0 holds it in reset.. Wire: `h\p\w`
    pub fn set_wio_reset_line(&mut self, state: resetLineState) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(state.0 as i32);
        let _r = crate::transport::call(397 /* CMD_HARDWARE_POWER_MANAGEMENT_SET_WIO_RESET_LINE */, &a)?;
        Ok(())
    }

    /// Set CM0 Run Line. Holds or releases the Linux CPU's run line (zone 19, CM0_RUNPG). 1 lets the module run, 0 holds it in reset.. Wire: `h\p\c`
    pub fn set_cm0_run_line(&mut self, state: resetLineState) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(state.0 as i32);
        let _r = crate::transport::call(395 /* CMD_HARDWARE_POWER_MANAGEMENT_SET_CM0_RUN_LINE */, &a)?;
        Ok(())
    }

    /// Get Control Lines. Reads back the pin levels of the three control lines, WIO_RST, CM0_RUNPG and MAIN_PWR_RST.. Wire: `h\p\n`
    pub fn get_control_lines(&mut self) -> Result<(bool, bool, bool), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(396 /* CMD_HARDWARE_POWER_MANAGEMENT_GET_CONTROL_LINES */, &a)?;
        Ok((_r.u8() != 0, _r.u8() != 0, _r.u8() != 0))
    }
}
