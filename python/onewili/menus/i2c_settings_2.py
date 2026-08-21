"""I2C Settings menu - generated from fwMenuI2CSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class I2CSettings(MenuBase):
    r"""I2C Settings (``h\s\i``)."""

    def frequency(self, value: int) -> Result:
        r"""Frequency.

        Wire: ``h\s\i\f``

        I2C bus clock frequency in Hz

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def pull_ups(self, value: int) -> Result:
        r"""PullUps.

        Wire: ``h\s\i\p``

        Enable I2C bus pull-up resistors

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(value)], [])
