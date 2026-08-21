"""Orca Communication menu - generated from fwMenuOrcaSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class OrcaSettings(MenuBase):
    r"""Orca Communication (``h\s\g``)."""

    def orca_com_over_uart(self, value: int) -> Result:
        r"""Orca Com over UART.

        Wire: ``h\s\g\u``

        Set Communication protocol for connected Orca device over UART

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_int(value)], [])
