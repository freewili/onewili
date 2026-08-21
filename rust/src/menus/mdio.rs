//! MDIO Functions menu - generated from fwMenuMDIO. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Mdio<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Mdio<'a> {
    /// SFP Poll. Polls for SFP Modules on the I2C bus. If any are found, return the PHY's temperature in Celsius and Signal Quality Indicator (SQI). Wire: `i\m\a`
    pub fn mdio_poll_sfp(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\m\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SFP Read. Reads a value from a register on the specified device address. Wire: `i\m\b`
    pub fn mdio_read_sfp(&mut self, device_address: u8, register_address: &[u8]) -> Result<u32, OwError> {
        let mut cmd = String::from("i\\m\\b");
        encoding::push_hex(&mut cmd, device_address as u64, 2);
        encoding::push_bytes(&mut cmd, register_address);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let sfp_response = encoding::tok_hex(&mut toks)? as u32;
        Ok(sfp_response)
    }

    /// SFP Write. Writes a value to a register on the specified device address. Wire: `i\m\c`
    pub fn mdio_write_sfp(&mut self, device_address: u8, register_address: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\m\\c");
        encoding::push_hex(&mut cmd, device_address as u64, 2);
        encoding::push_bytes(&mut cmd, register_address);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SFP Read-Modify-Write. Read-Modify-Writes a value to a register on the specified device address. '1' bits in the mask indicate an overwrite. Wire: `i\m\e`
    pub fn mdiormwsfp(&mut self, device_address: u8, register_address: &[u8], mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\m\\e");
        encoding::push_hex(&mut cmd, device_address as u64, 2);
        encoding::push_bytes(&mut cmd, register_address);
        encoding::push_bytes(&mut cmd, mask_bytes);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PHY Address Poll. Polls all 32 possible PHY addresses. Test for a response from status register. Returns PHY addresses and clause compatibility. Wire: `i\m\y`
    pub fn mdio_poll(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\m\\y");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clause 22 Read. Reads a value from a register belonging to a Clause-22-Compatible-PHY. Wire: `i\m\g`
    pub fn mdio_read22(&mut self, phy_address: u8, register_address: u8) -> Result<u32, OwError> {
        let mut cmd = String::from("i\\m\\g");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 2);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let mdio_response = encoding::tok_hex(&mut toks)? as u32;
        Ok(mdio_response)
    }

    /// Clause 22 Write. Writes a value to a register belonging to a Clause-22-Compatible-PHY. Wire: `i\m\i`
    pub fn mdio_write22(&mut self, phy_address: u8, register_address: u8, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\m\\i");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 2);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clause 22 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite. Wire: `i\m\j`
    pub fn mdiormw22(&mut self, phy_address: u8, register_address: u8, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\m\\j");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 2);
        encoding::push_bytes(&mut cmd, mask_bytes);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clause 45 Read. Reads a value from a register belonging to a Clause-45-Compatible-PHY. Wire: `i\m\k`
    pub fn mdio_read45(&mut self, phy_address: u8, mmd_address: u8, register_address: u32) -> Result<u32, OwError> {
        let mut cmd = String::from("i\\m\\k");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, mmd_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 4);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let mdio_response = encoding::tok_hex(&mut toks)? as u32;
        Ok(mdio_response)
    }

    /// Clause 45 Write. Writes a value to a register belonging to a Clause-45-Compatible-PHY. Wire: `i\m\l`
    pub fn mdio_write45(&mut self, phy_address: u8, mmd_address: u8, register_address: u32, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\m\\l");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, mmd_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 4);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clause 45 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite. Wire: `i\m\m`
    pub fn mdiormw45(&mut self, phy_address: u8, mmd_address: u8, register_address: u32, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\m\\m");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, mmd_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 4);
        encoding::push_bytes(&mut cmd, mask_bytes);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clause 22 Access to Clause 45 Read. Reads a value from a register belonging to a Clause-45-Emulation-Compatible-PHY. Wire: `i\m\n`
    pub fn mdio_read_emu(&mut self, phy_address: u8, mmd_address: u8, register_address: u32) -> Result<u32, OwError> {
        let mut cmd = String::from("i\\m\\n");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, mmd_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 4);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let mdio_response = encoding::tok_hex(&mut toks)? as u32;
        Ok(mdio_response)
    }

    /// Clause 22 Access to Clause 45 Write. Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY. Wire: `i\m\o`
    pub fn mdio_write_emu(&mut self, phy_address: u8, mmd_address: u8, register_address: u32, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\m\\o");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, mmd_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 4);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Clause 22 Access to Clause 45 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY. '1' bits in the mask indicate an overwrite. Wire: `i\m\p`
    pub fn mdiormw_emu(&mut self, phy_address: u8, mmd_address: u8, register_address: u32, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut cmd = String::from("i\\m\\p");
        encoding::push_hex(&mut cmd, phy_address as u64, 2);
        encoding::push_hex(&mut cmd, mmd_address as u64, 2);
        encoding::push_hex(&mut cmd, register_address as u64, 4);
        encoding::push_bytes(&mut cmd, mask_bytes);
        encoding::push_bytes(&mut cmd, data_bytes);
        self.t.call(&cmd)?;
        Ok(())
    }
}
