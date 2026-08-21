"""UART Settings menu - generated from fwMenuUARTSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class UARTSettings(MenuBase):
    r"""UART Settings (``i\u\s``)."""

    def baud_rate(self, value: int) -> Result:
        r"""Baud Rate.

        Wire: ``i\u\s\f``

        UART baud rate in bits per second

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def r_ts_hand_shaking(self) -> Result:
        r"""RTS Hand Shaking.

        Wire: ``i\u\s\r``

        Enable RTS hardware handshaking

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def c_ts_hand_shaking(self) -> Result:
        r"""CTS Hand Shaking.

        Wire: ``i\u\s\c``

        Enable CTS hardware handshaking

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def data_bits(self, value: int) -> Result:
        r"""Data Bits.

        Wire: ``i\u\s\b``

        UART data bits

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])

    def parity(self, value: int) -> Result:
        r"""Parity.

        Wire: ``i\u\s\p``

        UART parity mode

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(value)], [])

    def stop_bits(self, value: int) -> Result:
        r"""Stop Bits.

        Wire: ``i\u\s\s``

        UART stop bits

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(value)], [])

    def module(self, value: int) -> Result:
        r"""Module.

        Wire: ``i\u\s\m``

        Which UART module handles the port

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])
