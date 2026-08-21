"""Bluetooth Settings menu - generated from fwMenuBLESettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class BLESettings(MenuBase):
    r"""Bluetooth Settings (``h\s\b``)."""

    def enable_bt(self) -> Result:
        r"""Enable BT.

        Wire: ``h\s\b\s``

        Turn Bluetooth LE on or off

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def b_t_terminal(self) -> Result:
        r"""BT <-> Terminal.

        Wire: ``h\s\b\t``

        Shown in Bluetooth LE status, but not currently used: the firmware always follows the Enable BT setting instead

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def b_t_advert_name(self, value: str) -> Result:
        r"""BT Advert Name.

        Wire: ``h\s\b\a``

        Set the name the device advertises over Bluetooth LE

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(value)], [])
