"""Extra Actions menu - generated from fwMenuNFCExtra. Do not edit."""
from __future__ import annotations

from result import Result

from ..menubase import MenuBase
from ..transport import Transport


class NFCExtra(MenuBase):
    r"""Extra Actions (``w\n\x``)."""

    def halt_card(self) -> Result:
        r"""Halt Card.

        Wire: ``w\n\x\a``

        Send HLTA command to put card in HALT state

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])
