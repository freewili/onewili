//! MDIO Functions menu - generated from fwMenuMDIO. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct Mdio<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> Mdio<'a> {
    /// SFP Poll. Polls for SFP Modules on the I2C bus. If any are found, return the PHY's temperature in Celsius and Signal Quality Indicator (SQI). Wire: `i\m\a`
    pub fn mdio_poll_sfp(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(28 /* CMD_IO_MDIO_MDIO_POLL_SFP */, &a)?;
        Ok(())
    }

    /// SFP Read. Reads a value from a register on the specified device address. Wire: `i\m\b`
    pub fn mdio_read_sfp(&mut self, device_address: u8, register_address: &[u8]) -> Result<u32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(device_address);
        a.bytes(register_address);
        let mut _r = crate::transport::call(29 /* CMD_IO_MDIO_MDIO_READ_SFP */, &a)?;
        Ok(_r.u32())
    }

    /// SFP Write. Writes a value to a register on the specified device address. Wire: `i\m\c`
    pub fn mdio_write_sfp(&mut self, device_address: u8, register_address: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(device_address);
        a.bytes(register_address);
        a.bytes(data_bytes);
        let _r = crate::transport::call(30 /* CMD_IO_MDIO_MDIO_WRITE_SFP */, &a)?;
        Ok(())
    }

    /// SFP Read-Modify-Write. Read-Modify-Writes a value to a register on the specified device address. '1' bits in the mask indicate an overwrite. Wire: `i\m\e`
    pub fn mdiormwsfp(&mut self, device_address: u8, register_address: &[u8], mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(device_address);
        a.bytes(register_address);
        a.bytes(mask_bytes);
        a.bytes(data_bytes);
        let _r = crate::transport::call(31 /* CMD_IO_MDIO_MDIORMWSFP */, &a)?;
        Ok(())
    }

    /// PHY Address Poll. Polls all 32 possible PHY addresses. Test for a response from status register. Returns PHY addresses and clause compatibility. Wire: `i\m\y`
    pub fn mdio_poll(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(32 /* CMD_IO_MDIO_MDIO_POLL */, &a)?;
        Ok(())
    }

    /// Clause 22 Read. Reads a value from a register belonging to a Clause-22-Compatible-PHY. Wire: `i\m\g`
    pub fn mdio_read22(&mut self, phy_address: u8, register_address: u8) -> Result<u32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(register_address);
        let mut _r = crate::transport::call(33 /* CMD_IO_MDIO_MDIO_READ22 */, &a)?;
        Ok(_r.u32())
    }

    /// Clause 22 Write. Writes a value to a register belonging to a Clause-22-Compatible-PHY. Wire: `i\m\i`
    pub fn mdio_write22(&mut self, phy_address: u8, register_address: u8, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(register_address);
        a.bytes(data_bytes);
        let _r = crate::transport::call(34 /* CMD_IO_MDIO_MDIO_WRITE22 */, &a)?;
        Ok(())
    }

    /// Clause 22 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite. Wire: `i\m\j`
    pub fn mdiormw22(&mut self, phy_address: u8, register_address: u8, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(register_address);
        a.bytes(mask_bytes);
        a.bytes(data_bytes);
        let _r = crate::transport::call(35 /* CMD_IO_MDIO_MDIORMW22 */, &a)?;
        Ok(())
    }

    /// Clause 45 Read. Reads a value from a register belonging to a Clause-45-Compatible-PHY. Wire: `i\m\k`
    pub fn mdio_read45(&mut self, phy_address: u8, mmd_address: u8, register_address: u32) -> Result<u32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(mmd_address);
        a.u32(register_address);
        let mut _r = crate::transport::call(36 /* CMD_IO_MDIO_MDIO_READ45 */, &a)?;
        Ok(_r.u32())
    }

    /// Clause 45 Write. Writes a value to a register belonging to a Clause-45-Compatible-PHY. Wire: `i\m\l`
    pub fn mdio_write45(&mut self, phy_address: u8, mmd_address: u8, register_address: u32, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(mmd_address);
        a.u32(register_address);
        a.bytes(data_bytes);
        let _r = crate::transport::call(37 /* CMD_IO_MDIO_MDIO_WRITE45 */, &a)?;
        Ok(())
    }

    /// Clause 45 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite. Wire: `i\m\m`
    pub fn mdiormw45(&mut self, phy_address: u8, mmd_address: u8, register_address: u32, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(mmd_address);
        a.u32(register_address);
        a.bytes(mask_bytes);
        a.bytes(data_bytes);
        let _r = crate::transport::call(38 /* CMD_IO_MDIO_MDIORMW45 */, &a)?;
        Ok(())
    }

    /// Clause 22 Access to Clause 45 Read. Reads a value from a register belonging to a Clause-45-Emulation-Compatible-PHY. Wire: `i\m\n`
    pub fn mdio_read_emu(&mut self, phy_address: u8, mmd_address: u8, register_address: u32) -> Result<u32, OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(mmd_address);
        a.u32(register_address);
        let mut _r = crate::transport::call(39 /* CMD_IO_MDIO_MDIO_READ_EMU */, &a)?;
        Ok(_r.u32())
    }

    /// Clause 22 Access to Clause 45 Write. Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY. Wire: `i\m\o`
    pub fn mdio_write_emu(&mut self, phy_address: u8, mmd_address: u8, register_address: u32, data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(mmd_address);
        a.u32(register_address);
        a.bytes(data_bytes);
        let _r = crate::transport::call(40 /* CMD_IO_MDIO_MDIO_WRITE_EMU */, &a)?;
        Ok(())
    }

    /// Clause 22 Access to Clause 45 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY. '1' bits in the mask indicate an overwrite. Wire: `i\m\p`
    pub fn mdiormw_emu(&mut self, phy_address: u8, mmd_address: u8, register_address: u32, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.u8(phy_address);
        a.u8(mmd_address);
        a.u32(register_address);
        a.bytes(mask_bytes);
        a.bytes(data_bytes);
        let _r = crate::transport::call(41 /* CMD_IO_MDIO_MDIORMW_EMU */, &a)?;
        Ok(())
    }
}
