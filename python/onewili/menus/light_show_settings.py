"""LED Show Settings menu - generated from fwMenuLightShowSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class LightShowSettings(MenuBase):
    r"""LED Show Settings (``h\s\l``)."""

    def default_show(self, value: int) -> Result:
        r"""Default Show.

        Wire: ``h\s\l\c``

        The LED show the display boots into

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(value)], [])

    def l_ed_strips_enabled(self, value: int) -> Result:
        r"""LED Strips Enabled.

        Wire: ``h\s\l\s``

        How many external LED strips the built-in show drives

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(value)], [])

    def roku_led_control(self) -> Result:
        r"""Roku LED Control.

        Wire: ``h\s\l\i``

        Allow a Roku remote to cycle LED show patterns

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def brightness(self, value: int) -> Result:
        r"""Brightness.

        Wire: ``h\s\l\b``

        Onboard LED strip brightness divisor, 1 (brightest) to 16 (dimmest)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])
