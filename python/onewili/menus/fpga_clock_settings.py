"""FPGA Clock menu - generated from fwMenuFPGAClockSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class FPGAClockSettings(MenuBase):
    r"""FPGA Clock (``h\s\f``)."""

    def clk_source(self, value: int) -> Result:
        r"""Clk Source.

        Wire: ``h\s\f\c``

        Choose the clock source that drives the FPGA (CPU clock, oscillator, USB, or RTC)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(value)], [])

    def clk_divider_int(self, value: int) -> Result:
        r"""Clk Divider (int).

        Wire: ``h\s\f\i``

        Set the integer part of the clock divider used to derive the FPGA clock frequency

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_int(value)], [])

    def clk_divider_frac(self, value: int) -> Result:
        r"""Clk Divider (Frac).

        Wire: ``h\s\f\f``

        Set the fractional part of the clock divider used to fine-tune the FPGA clock frequency

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def comms_mode(self, value: int) -> Result:
        r"""Comms Mode.

        Wire: ``h\s\f\m``

        Choose whether the CPU talks to the FPGA configuration registers over SPI or I2C

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])
