"""Linux Functions menu - generated from fwMenuLinux. Do not edit."""
from __future__ import annotations

from result import Result

from ..menubase import MenuBase
from ..transport import Transport


class Linux(MenuBase):
    r"""Linux Functions (``l``)."""

    def enable_linux_cpu(self) -> Result:
        r"""Enable Linux CPU.

        Wire: ``l\a``

        Not yet implemented; always reports failure

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def open_shell(self) -> Result:
        r"""Open Shell.

        Wire: ``l\b``

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [], [])
