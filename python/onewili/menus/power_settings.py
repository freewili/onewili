"""Power Settings menu - generated from fwMenuPowerSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class PowerSettings(MenuBase):
    r"""Power Settings (``h\s\m``)."""

    def brightness_powered(self, value: int) -> Result:
        r"""Brightness Powered.

        Wire: ``h\s\m\a``

        Display backlight percent while on USB power

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(value)], [])

    def brightness_batt_gt70(self, value: int) -> Result:
        r"""Brightness Batt Gt 70.

        Wire: ``h\s\m\b``

        Display backlight percent with battery above 70 percent

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])

    def brightness_batt_gt30(self, value: int) -> Result:
        r"""Brightness Batt Gt 30.

        Wire: ``h\s\m\c``

        Display backlight percent with battery between 30 and 70 percent

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(value)], [])

    def brightness_batt_lt30(self, value: int) -> Result:
        r"""Brightness Batt Lt 30.

        Wire: ``h\s\m\g``

        Display backlight percent with battery below 30 percent

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_int(value)], [])

    def battery_timeout(self, value: int) -> Result:
        r"""Battery Timeout.

        Wire: ``h\s\m\e``

        Seconds of inactivity before the display shuts off on battery

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(value)], [])

    def powered_timeout(self, value: int) -> Result:
        r"""Powered Timeout.

        Wire: ``h\s\m\f``

        Seconds of inactivity before the display shuts off on USB power, 0 keeps it on

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def wake_on_sound(self) -> Result:
        r"""Wake On Sound.

        Wire: ``h\s\m\s``

        Wake the display when the mic hears a sound

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def wake_on_move(self) -> Result:
        r"""Wake On Move.

        Wire: ``h\s\m\m``

        Wake the display when the accelerometer sees movement

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [], [])

    def auto_power_zones(self) -> Result:
        r"""Auto Power Zones.

        Wire: ``h\s\m\o``

        Auto-manage power zones; off keeps every live rail on, commands still raise rails on demand

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [], [])
