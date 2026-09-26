//! PLCA Settings menu - generated from fwMenuPLCA. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Plca<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Plca<'a> {
    /// PLCAEnabled. When on, the PHY runs PLCA (collision-free round-robin transmit opportunities; the node with Local ID 0 coordinates the cycle). When off, the PHY falls back to CSMA/CD. Applied live to a running PHY. Wire: `i\r\p\a`
    pub fn p_lca_enabled(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(568 /* CMD_IO_T1S_PLCA_P_LCA_ENABLED */, &a)?;
        Ok(())
    }

    /// Local ID. This node's PLCA ID (0..254). ID 0 is the cycle coordinator -- exactly one node on the segment must be 0. Applied live to a running PHY. Wire: `i\r\p\l`
    pub fn local_id(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(570 /* CMD_IO_T1S_PLCA_LOCAL_ID */, &a)?;
        Ok(())
    }

    /// Node Count. Number of transmit opportunities in each PLCA cycle (1..255); only meaningful on the coordinator (Local ID 0). Applied live to a running PHY. Wire: `i\r\p\n`
    pub fn node_count(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(572 /* CMD_IO_T1S_PLCA_NODE_COUNT */, &a)?;
        Ok(())
    }

    /// TO Timer. PLCA transmit-opportunity timer in bit times (1..255, silicon default 32). Written directly to the PHY's PLCA_TOTMR register when it differs from 32. Applied live to a running PHY. Wire: `i\r\p\t`
    pub fn t_o_timer(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(573 /* CMD_IO_T1S_PLCA_T_O_TIMER */, &a)?;
        Ok(())
    }

    /// Burst Max. Maximum extra packets this node may send in one transmit opportunity (0..255, 0 = burst off). Takes effect at the next PHY (re)init -- use Reinit PHY (i\r\i) to apply. Wire: `i\r\p\m`
    pub fn burst_max(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(571 /* CMD_IO_T1S_PLCA_BURST_MAX */, &a)?;
        Ok(())
    }

    /// Burst Timer. Idle time in bit times the PHY waits between burst packets before giving up the transmit opportunity (1..255). Takes effect at the next PHY (re)init -- use Reinit PHY (i\r\i) to apply. Wire: `i\r\p\b`
    pub fn burst_timer(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(569 /* CMD_IO_T1S_PLCA_BURST_TIMER */, &a)?;
        Ok(())
    }
}
