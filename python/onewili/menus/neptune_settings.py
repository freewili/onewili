"""Neptune Settings menu - generated from fwMenuNeptuneSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class NeptuneSettings(MenuBase):
    r"""Neptune Settings (``h\s\p``)."""

    def c_an1_mode(self, value: int) -> Result:
        r"""CAN1 Mode.

        Wire: ``h\s\p\a``

        CAN Type or UART over CAN PHY

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(value)], [])

    def c_an1_rate(self, value: int) -> Result:
        r"""CAN1 Rate.

        Wire: ``h\s\p\b``

        Baudrate of CAN or UART over CAN PHY

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])

    def c_an1fdd_rate(self, value: int) -> Result:
        r"""CAN1 FD D Rate.

        Wire: ``h\s\p\c``

        Baud Rate for CANFD Data section

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(value)], [])

    def c_an1_listen_only(self) -> Result:
        r"""CAN1 Listen Only.

        Wire: ``h\s\p\y``

        Enables Listen Only mode

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("y", [], [])

    def c_an1_tx_retry(self, value: int) -> Result:
        r"""CAN1 Tx Retry.

        Wire: ``h\s\p\e``

        CAN Transmit retry options

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(value)], [])

    def c_an1_cust_baud(self, value: str) -> Result:
        r"""CAN1 Cust Baud.

        Wire: ``h\s\p\f``

        Hex Value String for Register C1NBTCFG. Blank to disable.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_str(value)], [])

    def c_an1_cust_data_baud(self, value: str) -> Result:
        r"""CAN1 Cust Data Baud.

        Wire: ``h\s\p\g``

        Hex Value String for Register C1DBTCFG

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_str(value)], [])

    def c_an1_termination(self) -> Result:
        r"""CAN1 Termination.

        Wire: ``h\s\p\1``

        Enables termination for network.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("1", [], [])

    def c_an1api_enabled(self) -> Result:
        r"""CAN1 API Enabled.

        Wire: ``h\s\p\i``

        Set Wili API Base ID

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def c_anapiid(self, value: int) -> Result:
        r"""CAN API ID.

        Wire: ``h\s\p\j``

        Enables Terminal over CANFD

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_int(value)], [])

    def c_an2_mode(self, value: int) -> Result:
        r"""CAN2 Mode.

        Wire: ``h\s\p\k``

        CAN Type or UART over CAN PHY

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_int(value)], [])

    def c_an2_rate(self, value: int) -> Result:
        r"""CAN2 Rate.

        Wire: ``h\s\p\l``

        Baudrate of CAN or UART over CAN PHY

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_int(value)], [])

    def c_an2fdd_rate(self, value: int) -> Result:
        r"""CAN2 FD D Rate.

        Wire: ``h\s\p\m``

        Baud Rate for CANFD Data section

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])

    def c_an2_listen_only(self) -> Result:
        r"""CAN2 Listen Only.

        Wire: ``h\s\p\n``

        Enables Listen Only mode

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [], [])

    def c_an2_tx_retry(self, value: int) -> Result:
        r"""CAN2 Tx Retry.

        Wire: ``h\s\p\o``

        CAN Transmit retry options

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(value)], [])

    def c_an2_cust_baud(self, value: str) -> Result:
        r"""CAN2 Cust Baud.

        Wire: ``h\s\p\p``

        Hex Value String for Register C1NBTCFG. Blank to disable.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_str(value)], [])

    def c_an2_cust_data_baud(self, value: str) -> Result:
        r"""CAN2 Cust Data Baud.

        Wire: ``h\s\p\r``

        Hex Value String for Register C1DBTCFG

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_str(value)], [])

    def c_an2_termination(self) -> Result:
        r"""CAN2 Termination.

        Wire: ``h\s\p\s``

        Enables termination for network.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def c_an2api_enabled(self) -> Result:
        r"""CAN2 API Enabled.

        Wire: ``h\s\p\t``

        Enables Wili API over CANFD

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def l_in_master_en(self) -> Result:
        r"""LIN Master En.

        Wire: ``h\s\p\u``

        Enables LIN Master Pull Resistor

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [], [])

    def l_in_baud_rate(self, value: int) -> Result:
        r"""LIN Baud Rate.

        Wire: ``h\s\p\v``

        Baud Rate for LIN

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_int(value)], [])

    def analog_in_en(self) -> Result:
        r"""Analog In En.

        Wire: ``h\s\p\x``

        Enables analog input measurement

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [], [])
