//! TC10 Wake/Sleep menu - generated from fwMenuT1STc10. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct T1sTc10<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> T1sTc10<'a> {
    /// Generate Wake. Emits a TC10 wake-up from the running LAN865x: a 1 ms DME wake burst onto the MDI (Forward to MDI) and/or a 90 us pulse on the WAKE_OUT pin (Forward to WAKE_OUT), per the settings below. The engine polls the PHY until the request completes (see Wake Status gen/done/timeout). Fails when the PHY is not in run, neither forward target is enabled, or a previous wake is still busy. Wire: `i\r\w\g`
    pub fn t1s_tc10_generate_wake(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(578 /* CMD_IO_T1S_TC10_T1S_TC10_GENERATE_WAKE */, &a)?;
        Ok(())
    }

    /// Wake Status. Prints one line of key=value TC10 status: state (engine state, sleep while asleep) gen done busy timeout (wake generations requested/completed/in flight/expired) sleeps woke pulses (sleep entries, wake detections, local WAKE_IN pulses) src (last wake source: none/mdi/wakein/mdi+wakein) sts2 (raw PHY STS2) fwd wake (forward targets and wake sources as configured) inhdly (INH release delay code) sleepms (ms asleep, 0 when awake). Wire-parseable, append-only. Wire: `i\r\w\s`
    pub fn t1s_tc10_wake_status(&mut self) -> Result<String, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(582 /* CMD_IO_T1S_TC10_T1S_TC10_WAKE_STATUS */, &a)?;
        Ok(_r.string())
    }

    /// Enter Sleep. Puts the LAN865x into TC10 sleep with the configured wake sources (Wake on MDI / Wake on WAKE_IN) and forward targets. On Orca the PHY's INH output then cuts its own SPI/IRQ path, so the engine tears the link down after a short grace and parks in the sleep state until the chip wakes (MDI energy, a WAKE_IN pulse, Local Wake Pulse) or Cancel Sleep reinits it. Fails when the PHY is not in run or a sleep is already pending. Wire: `i\r\w\e`
    pub fn t1s_tc10_enter_sleep(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(577 /* CMD_IO_T1S_TC10_T1S_TC10_ENTER_SLEEP */, &a)?;
        Ok(())
    }

    /// Local Wake Pulse. Drives a 200 us HIGH pulse on the PHY's WAKE_IN pin from the board's IO expander (which stays powered while the PHY sleeps). Only a sleeping PHY reacts (it wakes and the engine reinitializes it); harmless when awake. Fails if the expander is not configured or the I2C write fails. Wire: `i\r\w\w`
    pub fn t1s_tc10_local_wake_pulse(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(583 /* CMD_IO_T1S_TC10_T1S_TC10_LOCAL_WAKE_PULSE */, &a)?;
        Ok(())
    }

    /// Cancel Sleep. Abandons a pending sleep or leaves the sleep state by requesting a full PHY reinit (RST pulse + fresh init). Reports 'not sleeping' when no sleep is in progress. Wire: `i\r\w\c`
    pub fn t1s_tc10_cancel_sleep(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(576 /* CMD_IO_T1S_TC10_T1S_TC10_CANCEL_SLEEP */, &a)?;
        Ok(())
    }

    /// Forward to MDI. When on, Generate Wake (and a wake forwarded during sleep) puts a 1 ms wake burst onto the MDI so the far end of the T1S segment wakes. At least one of Forward to MDI / Forward to WAKE_OUT must be on for Generate Wake to do anything. Wire: `i\r\w\m`
    pub fn forward_to_mdi(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(579 /* CMD_IO_T1S_TC10_FORWARD_TO_MDI */, &a)?;
        Ok(())
    }

    /// Forward to WAKE_OUT. When on, Generate Wake (and a wake forwarded during sleep) emits a 90 us pulse on the PHY's WAKE_OUT pin (routed to the header on Orca; the host cannot observe it). At least one of Forward to MDI / Forward to WAKE_OUT must be on for Generate Wake to do anything. Wire: `i\r\w\o`
    pub fn forward_to_wakeout(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(581 /* CMD_IO_T1S_TC10_FORWARD_TO_WAKEOUT */, &a)?;
        Ok(())
    }

    /// Wake on MDI. When on, a sleeping PHY wakes on energy detected on the MDI (any activity, not only a TC10 wake burst). Applied at the next Enter Sleep. Wire: `i\r\w\a`
    pub fn wake_on_mdi(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(575 /* CMD_IO_T1S_TC10_WAKE_ON_MDI */, &a)?;
        Ok(())
    }

    /// Wake on WAKE_IN. When on, a sleeping PHY wakes on a HIGH pulse longer than 40 us on its WAKE_IN pin (Local Wake Pulse drives that pin from the IO expander). Applied at the next Enter Sleep. Wire: `i\r\w\n`
    pub fn wake_on_wakein(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(580 /* CMD_IO_T1S_TC10_WAKE_ON_WAKEIN */, &a)?;
        Ok(())
    }

    /// Sleep Inhibit Delay. Delay before the PHY releases its INH output after entering sleep: 0 = 0 ms, 1 = 50 ms, 2 = 100 ms, 3 = 200 ms. On Orca INH powers the PHY's SPI/IRQ path, so this is how long the link stays reachable after Enter Sleep. Applied at the next Enter Sleep. Wire: `i\r\w\y`
    pub fn sleep_inhibit_delay(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(584 /* CMD_IO_T1S_TC10_SLEEP_INHIBIT_DELAY */, &a)?;
        Ok(())
    }
}
