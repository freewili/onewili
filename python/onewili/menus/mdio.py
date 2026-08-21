"""MDIO Functions menu - generated from fwMenuMDIO. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class MDIO(MenuBase):
    r"""MDIO Functions (``i\m``)."""

    def mdio_poll_sfp(self) -> Result:
        r"""SFP Poll.

        Wire: ``i\m\a``

        Polls for SFP Modules on the I2C bus. If any are found, return the PHY's temperature in Celsius and Signal Quality Indicator (SQI)

        Checking for SFP Module

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def mdio_read_sfp(self, device_address: int, register_address: bytes | bytearray) -> Result:
        r"""SFP Read.

        Wire: ``i\m\b``

        Reads a value from a register on the specified device address

        Hex: MDIO Device Address, Register Address Bytes to read Separated by Spaces

        Args:
            device_address: device_address (hex8).
            register_address: register_address (hexbytes).

        Returns:
            Result: Ok(sfp_response: int) or Err(message).
        """
        return self._call("b", [encoding.enc_hex(device_address, 2), encoding.enc_bytes(register_address)], ["hex"])

    def mdio_write_sfp(self, device_address: int, register_address: bytes | bytearray, data_bytes: bytes | bytearray) -> Result:
        r"""SFP Write.

        Wire: ``i\m\c``

        Writes a value to a register on the specified device address

        Hex: MDIO Device Address, Register Address Bytes, Data Bytes to write Separated by Spaces

        Args:
            device_address: device_address (hex8).
            register_address: register_address (hexbytes).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_hex(device_address, 2), encoding.enc_bytes(register_address), encoding.enc_bytes(data_bytes)], [])

    def mdiormwsfp(self, device_address: int, register_address: bytes | bytearray, mask_bytes: bytes | bytearray, data_bytes: bytes | bytearray) -> Result:
        r"""SFP Read-Modify-Write.

        Wire: ``i\m\e``

        Read-Modify-Writes a value to a register on the specified device address. '1' bits in the mask indicate an overwrite

        Hex: MDIO Device Address, Register Address Bytes, Mask Bytes, Data Bytes to read-modify-write Separated by Spaces

        Args:
            device_address: device_address (hex8).
            register_address: register_address (hexbytes).
            mask_bytes: mask_bytes (hexbytes).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_hex(device_address, 2), encoding.enc_bytes(register_address), encoding.enc_bytes(mask_bytes), encoding.enc_bytes(data_bytes)], [])

    def mdio_poll(self) -> Result:
        r"""PHY Address Poll.

        Wire: ``i\m\y``

        Polls all 32 possible PHY addresses. Test for a response from status register. Returns PHY addresses and clause compatibility

        Checking for PHYs

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("y", [], [])

    def mdio_read22(self, phy_address: int, register_address: int) -> Result:
        r"""Clause 22 Read.

        Wire: ``i\m\g``

        Reads a value from a register belonging to a Clause-22-Compatible-PHY

        Hex: PHY Address, Register Address to read Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            register_address: register_address (hex8).

        Returns:
            Result: Ok(mdio_response: int) or Err(message).
        """
        return self._call("g", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(register_address, 2)], ["hex"])

    def mdio_write22(self, phy_address: int, register_address: int, data_bytes: bytes | bytearray) -> Result:
        r"""Clause 22 Write.

        Wire: ``i\m\i``

        Writes a value to a register belonging to a Clause-22-Compatible-PHY

        Hex: PHY Address, Register Address, Data Bytes to write Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            register_address: register_address (hex8).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(register_address, 2), encoding.enc_bytes(data_bytes)], [])

    def mdiormw22(self, phy_address: int, register_address: int, mask_bytes: bytes | bytearray, data_bytes: bytes | bytearray) -> Result:
        r"""Clause 22 Read-Modify-Write.

        Wire: ``i\m\j``

        Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite

        Hex: PHY Address, Register Address, Mask Bytes, Data Bytes to read-modify-write Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            register_address: register_address (hex8).
            mask_bytes: mask_bytes (hexbytes).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(register_address, 2), encoding.enc_bytes(mask_bytes), encoding.enc_bytes(data_bytes)], [])

    def mdio_read45(self, phy_address: int, mmd_address: int, register_address: int) -> Result:
        r"""Clause 45 Read.

        Wire: ``i\m\k``

        Reads a value from a register belonging to a Clause-45-Compatible-PHY

        Hex: PHY Address, MMD Address, Register Address to read Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            mmd_address: mmd_address (hex8).
            register_address: register_address (hex16).

        Returns:
            Result: Ok(mdio_response: int) or Err(message).
        """
        return self._call("k", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(mmd_address, 2), encoding.enc_hex(register_address, 4)], ["hex"])

    def mdio_write45(self, phy_address: int, mmd_address: int, register_address: int, data_bytes: bytes | bytearray) -> Result:
        r"""Clause 45 Write.

        Wire: ``i\m\l``

        Writes a value to a register belonging to a Clause-45-Compatible-PHY

        Hex: PHY Address, MMD Address, Register Address, Data Bytes to write Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            mmd_address: mmd_address (hex8).
            register_address: register_address (hex16).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(mmd_address, 2), encoding.enc_hex(register_address, 4), encoding.enc_bytes(data_bytes)], [])

    def mdiormw45(self, phy_address: int, mmd_address: int, register_address: int, mask_bytes: bytes | bytearray, data_bytes: bytes | bytearray) -> Result:
        r"""Clause 45 Read-Modify-Write.

        Wire: ``i\m\m``

        Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite

        Hex: PHY Address, MMD Address, Register Address, Mask Bytes, Data Bytes to read-modify-write Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            mmd_address: mmd_address (hex8).
            register_address: register_address (hex16).
            mask_bytes: mask_bytes (hexbytes).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(mmd_address, 2), encoding.enc_hex(register_address, 4), encoding.enc_bytes(mask_bytes), encoding.enc_bytes(data_bytes)], [])

    def mdio_read_emu(self, phy_address: int, mmd_address: int, register_address: int) -> Result:
        r"""Clause 22 Access to Clause 45 Read.

        Wire: ``i\m\n``

        Reads a value from a register belonging to a Clause-45-Emulation-Compatible-PHY

        Hex: PHY Address, MMD Address, Register Address to read Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            mmd_address: mmd_address (hex8).
            register_address: register_address (hex16).

        Returns:
            Result: Ok(mdio_response: int) or Err(message).
        """
        return self._call("n", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(mmd_address, 2), encoding.enc_hex(register_address, 4)], ["hex"])

    def mdio_write_emu(self, phy_address: int, mmd_address: int, register_address: int, data_bytes: bytes | bytearray) -> Result:
        r"""Clause 22 Access to Clause 45 Write.

        Wire: ``i\m\o``

        Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY

        Hex: PHY Address, MMD Address, Register Address, Data Bytes to write Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            mmd_address: mmd_address (hex8).
            register_address: register_address (hex16).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(mmd_address, 2), encoding.enc_hex(register_address, 4), encoding.enc_bytes(data_bytes)], [])

    def mdiormw_emu(self, phy_address: int, mmd_address: int, register_address: int, mask_bytes: bytes | bytearray, data_bytes: bytes | bytearray) -> Result:
        r"""Clause 22 Access to Clause 45 Read-Modify-Write.

        Wire: ``i\m\p``

        Read-Modify-Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY. '1' bits in the mask indicate an overwrite

        Hex: PHY Address, MMD Address, Register Address, Mask Bytes, Data Bytes to read-modify-write Separated by Spaces

        Args:
            phy_address: phy_address (hex8).
            mmd_address: mmd_address (hex8).
            register_address: register_address (hex16).
            mask_bytes: mask_bytes (hexbytes).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_hex(phy_address, 2), encoding.enc_hex(mmd_address, 2), encoding.enc_hex(register_address, 4), encoding.enc_bytes(mask_bytes), encoding.enc_bytes(data_bytes)], [])
