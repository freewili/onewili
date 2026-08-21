"""Wili Files menu - generated from fwMenuWiliFiles. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class WiliFiles(MenuBase):
    r"""Wili Files (``s\f``)."""

    def wili_load(self, filepath: str) -> Result:
        r"""Load.

        Wire: ``s\f\l``

        Loads a .wili project.

        Wili file

        Args:
            filepath: filepath (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_str(filepath)], [])

    def wili_save(self) -> Result:
        r"""Save Current.

        Wire: ``s\f\s``

        Saves the current Wili project to its source path.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def wili_reset(self) -> Result:
        r"""Reset.

        Wire: ``s\f\r``

        Clears the live panels, Wili Blocks, and app signals.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def wili_default(self, filepath: str) -> Result:
        r"""Make Default.

        Wire: ``s\f\m``

        Sets the Wili project loaded at boot.

        Wili file

        Args:
            filepath: filepath (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_str(filepath)], [])

    def wili_remove_default(self) -> Result:
        r"""Remove Default.

        Wire: ``s\f\x``

        Removes the configured boot Wili project.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [], [])
