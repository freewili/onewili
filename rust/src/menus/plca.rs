//! PLCA Settings menu - generated from fwMenuPLCA. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Plca<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Plca<'a> {
    /// PLCAEnabled. When on, the PHY runs PLCA (collision-free round-robin transmit opportunities; the node with Local ID 0 coordinates the cycle). When off, the PHY falls back to CSMA/CD. Applied live to a running PHY. Wire: `i\r\p\a`
    pub fn p_lca_enabled(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\r\\p\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Local ID. This node's PLCA ID (0..254). ID 0 is the cycle coordinator -- exactly one node on the segment must be 0. Applied live to a running PHY. Wire: `i\r\p\l`
    pub fn local_id(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\r\\p\\l");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Node Count. Number of transmit opportunities in each PLCA cycle (1..255); only meaningful on the coordinator (Local ID 0). Applied live to a running PHY. Wire: `i\r\p\n`
    pub fn node_count(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\r\\p\\n");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// TO Timer. PLCA transmit-opportunity timer in bit times (1..255, silicon default 32). Written directly to the PHY's PLCA_TOTMR register when it differs from 32. Applied live to a running PHY. Wire: `i\r\p\t`
    pub fn t_o_timer(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\r\\p\\t");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Burst Max. Maximum extra packets this node may send in one transmit opportunity (0..255, 0 = burst off). Takes effect at the next PHY (re)init -- use Reinit PHY (i\r\i) to apply. Wire: `i\r\p\m`
    pub fn burst_max(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\r\\p\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Burst Timer. Idle time in bit times the PHY waits between burst packets before giving up the transmit opportunity (1..255). Takes effect at the next PHY (re)init -- use Reinit PHY (i\r\i) to apply. Wire: `i\r\p\b`
    pub fn burst_timer(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\r\\p\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }
}
