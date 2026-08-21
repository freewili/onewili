//! Power Management menu - generated from fwMenuPowerManagement. Do not edit.

use crate::encoding;
use crate::enums::resetLineState;
use crate::transport::{OwError, Transport};

pub struct PowerManagement<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> PowerManagement<'a> {
    /// List Zones. Lists all 17 power zones with their name and rail, then the three control lines (18-20).. Wire: `h\p\l`
    pub fn list_zones(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\p\\l");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Zones. Shows which power zones are currently on, then the reset state of the three control lines.. Wire: `h\p\g`
    pub fn get_zones(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\p\\g");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Zone. Switches one power zone on or off. Zone 9 is the board-manager LED, not a power rail; zones 18-20 are reset lines with their own commands.. Wire: `h\p\s`
    pub fn set_zone(&mut self, zone: i32, on: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\p\\s");
        encoding::push_int(&mut cmd, zone as i64);
        encoding::push_int(&mut cmd, on as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set Zone Mask. Sets every user-controllable zone at once from a bit mask; bit 0 is zone 1.. Wire: `h\p\m`
    pub fn set_zone_mask(&mut self, mask: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\p\\m");
        encoding::push_int(&mut cmd, mask as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Power State. Prints the most recent power telemetry sample.. Wire: `h\p\t`
    pub fn get_power_state(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\p\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Stream Power. Streams battery, charger and power-zone telemetry to the host at the given rate. 0 stops the stream.. Wire: `h\p\o`
    pub fn enable_power_stream(&mut self, stream_rate_ms: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\p\\o");
        encoding::push_int(&mut cmd, stream_rate_ms as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set WIO Reset Line. Holds or releases the LoRa module's reset line (zone 18, WIO_RST). 1 lets the module run, 0 holds it in reset.. Wire: `h\p\w`
    pub fn set_wio_reset_line(&mut self, state: resetLineState) -> Result<(), OwError> {
        let mut cmd = String::from("h\\p\\w");
        encoding::push_int(&mut cmd, state.0);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Set CM0 Run Line. Holds or releases the Linux CPU's run line (zone 19, CM0_RUNPG). 1 lets the module run, 0 holds it in reset.. Wire: `h\p\c`
    pub fn set_cm0_run_line(&mut self, state: resetLineState) -> Result<(), OwError> {
        let mut cmd = String::from("h\\p\\c");
        encoding::push_int(&mut cmd, state.0);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Get Control Lines. Reads back the pin levels of the three control lines, WIO_RST, CM0_RUNPG and MAIN_PWR_RST.. Wire: `h\p\n`
    pub fn get_control_lines(&mut self) -> Result<(bool, bool, bool), OwError> {
        let cmd = String::from("h\\p\\n");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let wio_released = encoding::tok_bool(&mut toks)?;
        let cm0_released = encoding::tok_bool(&mut toks)?;
        let main_rst_high = encoding::tok_bool(&mut toks)?;
        Ok((wio_released, cm0_released, main_rst_high))
    }
}

/// Spontaneous event frames this menu emits:
/// (id, binary, payload "name=wiretype,...", description).
pub const EVENTS: &[(&str, bool, &str, &str)] = &[
    ("power", false, "soc=decS32,current_ma=decS32,remain_mah=decS32,full_mah=decS32,vbus_mv=decS32,vsys_mv=decS32,vbat_mv=decS32,ichg_ma=decS32,chg_stat=decS32,vbus_stat=decS32,fault=decS32,zone_mask=decU32,tier_main=decS32,tier_display=decS32,backlight=decS32,idle_ms=decS32,valid=bool", "Power Telemetry"),
];
