"""SPI Settings menu - generated from fwMenuSPISettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class SPISettings(MenuBase):
    r"""SPI Settings (``h\s\s``)."""

    def frequency(self, value: int) -> Result:
        r"""Frequency.

        Wire: ``h\s\s\f``

        SPI clock frequency in Hz

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def chip_select_pin(self, value: int) -> Result:
        r"""Chip Select Pin.

        Wire: ``h\s\s\c``

        GPIO pin used as SPI chip select

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(value)], [])

    def data_bits(self, value: int) -> Result:
        r"""Data Bits.

        Wire: ``h\s\s\b``

        SPI data bits per transfer

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])

    def c_pol(self) -> Result:
        r"""CPOL.

        Wire: ``h\s\s\p``

        SPI clock polarity

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [], [])

    def c_pha(self) -> Result:
        r"""CPHA.

        Wire: ``h\s\s\a``

        SPI clock phase

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])
