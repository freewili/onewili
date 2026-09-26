//! Ethernet 10BaseT1S menu - generated from fwMenuT1S. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct T1s<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> T1s<'a> {
    /// Ethernet Test sub-menu.
    pub fn eth_test(self) -> super::eth_test::EthTest<'a> {
        super::eth_test::EthTest { t: self.t }
    }

    /// PLCA Settings sub-menu.
    pub fn plca(self) -> super::plca::Plca<'a> {
        super::plca::Plca { t: self.t }
    }

    /// TC10 Wake/Sleep sub-menu.
    pub fn tc10(self) -> super::t1s_tc10::T1sTc10<'a> {
        super::t1s_tc10::T1sTc10 { t: self.t }
    }

    /// Status. Prints one line of key=value T1S engine status: state link plca plcaen id cnt to chipRev t1sTx t1sTxDrop t1sRx t1sRxDrop spiAbort errs evts faults lastErr lastEvt ... term tc10 wkgen wksrc. plca=1 means the PLCA cycle is locked; chipRev is 0 until the PHY initialized; tc10 is 0 awake / 1 sleep pending / 2 sleeping, wkgen counts TC10 wake generations requested, wksrc is the last wake source (bit1 MDI, bit0 WAKE_IN). Wire-parseable, append-only. Wire: `i\r\s`
    pub fn t1s_status(&mut self) -> Result<String, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(574 /* CMD_IO_T1S_T1S_STATUS */, &a)?;
        Ok(_r.string())
    }

    /// Link Status. Reports the T1S link (up while the PHY is initialized and running), the engine state name and whether the NCM<->T1S bridge is on. Wire: `i\r\k`
    pub fn t1s_link_status(&mut self) -> Result<String, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(567 /* CMD_IO_T1S_T1S_LINK_STATUS */, &a)?;
        Ok(_r.string())
    }

    /// Reinit PHY. Requests a full PHY reinit: RST pulse plus fresh TC6 init with the current PLCA settings (this is how Burst Max/Burst Timer changes take effect). Also enables the T1S engine and clears the FAULT retry budget; bring-up itself still waits for the IO-header rail (zone 6). Wire: `i\r\i`
    pub fn t1s_reinit_phy(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(566 /* CMD_IO_T1S_T1S_REINIT_PHY */, &a)?;
        Ok(())
    }

    /// Clear Counters. Zeros the T1S TX/RX/drop/error counters (chip revision is kept). The engine state, link and bridge are unaffected. Wire: `i\r\c`
    pub fn t1s_clear_counters(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(564 /* CMD_IO_T1S_T1S_CLEAR_COUNTERS */, &a)?;
        Ok(())
    }

    /// Register Read. Reads one 32-bit register from the LAN865x over the TC6 SPI protocol: MMS is the memory map selector (0..15), Address the 16-bit register address within it. Requires the PHY to be initialized and running. Wire: `i\r\g`
    pub fn t1s_register_read(&mut self, mms: i32, address: u32) -> Result<u32, OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(mms);
        a.u32(address);
        let mut _r = crate::transport::call(565 /* CMD_IO_T1S_T1S_REGISTER_READ */, &a)?;
        Ok(_r.u32())
    }

    /// Bridge. When on, host NCM frames forward to the T1S wire and T1S frames forward to the host (the local classifier/responder/loopback step aside) and the host adapter's link mirrors the T1S link. Always off after a reboot. Wire: `i\r\b`
    pub fn bridge(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(563 /* CMD_IO_T1S_BRIDGE */, &a)?;
        Ok(())
    }
}
