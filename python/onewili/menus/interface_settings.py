"""Interface Settings menu - generated from fwMenuInterfaceSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class InterfaceSettings(MenuBase):
    r"""Interface Settings (``h\s\x``)."""

    def double_click_ms(self, value: int) -> Result:
        r"""Double Click Ms.

        Wire: ``h\s\x\c``

        Button double-click window in milliseconds (currently inert; the display uses a compile-time window)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(value)], [])

    def roku_gui_control(self) -> Result:
        r"""Roku Gui Control.

        Wire: ``h\s\x\r``

        Allow a Roku remote to drive the display GUI buttons

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])
