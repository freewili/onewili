"""Analog In (TLA2024) Settings menu - generated from fwMenuAnalogInSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class AnalogInSettings(MenuBase):
    r"""Analog In (TLA2024) Settings (``h\s\j``)."""

    def ch0_input(self, value: int) -> Result:
        r"""Ch0 Input.

        Wire: ``h\s\j\0``

        TLA2024 channel 0 input mux

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("0", [encoding.enc_int(value)], [])

    def ch1_input(self, value: int) -> Result:
        r"""Ch1 Input.

        Wire: ``h\s\j\1``

        TLA2024 channel 1 input mux

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("1", [encoding.enc_int(value)], [])

    def ch2_input(self, value: int) -> Result:
        r"""Ch2 Input.

        Wire: ``h\s\j\2``

        TLA2024 channel 2 input mux

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("2", [encoding.enc_int(value)], [])

    def ch3_input(self, value: int) -> Result:
        r"""Ch3 Input.

        Wire: ``h\s\j\3``

        TLA2024 channel 3 input mux

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("3", [encoding.enc_int(value)], [])

    def ch0_range(self, value: int) -> Result:
        r"""Ch0 Range.

        Wire: ``h\s\j\4``

        TLA2024 channel 0 full-scale range

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("4", [encoding.enc_int(value)], [])

    def ch1_range(self, value: int) -> Result:
        r"""Ch1 Range.

        Wire: ``h\s\j\5``

        TLA2024 channel 1 full-scale range

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("5", [encoding.enc_int(value)], [])

    def ch2_range(self, value: int) -> Result:
        r"""Ch2 Range.

        Wire: ``h\s\j\6``

        TLA2024 channel 2 full-scale range

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("6", [encoding.enc_int(value)], [])

    def ch3_range(self, value: int) -> Result:
        r"""Ch3 Range.

        Wire: ``h\s\j\7``

        TLA2024 channel 3 full-scale range

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("7", [encoding.enc_int(value)], [])

    def data_rate(self, value: int) -> Result:
        r"""Data Rate.

        Wire: ``h\s\j\8``

        TLA2024 conversion data rate

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("8", [encoding.enc_int(value)], [])
