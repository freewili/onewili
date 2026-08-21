"""RF Analyzer Settings menu - generated from fwMenuRadioFASettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class RadioFASettings(MenuBase):
    r"""RF Analyzer Settings (``h\s\a``)."""

    def default_view(self, value: int) -> Result:
        r"""Default View.

        Wire: ``h\s\a\a``

        Default view for the RF Analyzer

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(value)], [])
