"""rThon Debug menu - generated from fwMenuRthonDebug. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class RthonDebug(MenuBase):
    r"""rThon Debug (``s\r``)."""

    def debug_start(self, path: str) -> Result:
        r"""Debug Start.

        Wire: ``s\r\c``

        Loads and compiles a script for debugging.

        path startPaused [bpLines...]

        Args:
            path: path (str).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_str(path)], [])

    def debug_breakpoints(self, lines: str) -> Result:
        r"""Debug Breakpoints.

        Wire: ``s\r\j``

        Replaces the breakpoint set for the active debug session.

        [bpLines...]

        Args:
            lines: lines (str).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_str(lines)], [])

    def debug_step(self) -> Result:
        r"""Debug Step.

        Wire: ``s\r\e``

        Single-steps the active debug session.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def debug_continue(self) -> Result:
        r"""Debug Continue.

        Wire: ``s\r\f``

        Resumes the active debug session until the next breakpoint.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [], [])

    def debug_pause(self) -> Result:
        r"""Debug Pause.

        Wire: ``s\r\g``

        Pauses the active debug session at the next statement.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [], [])

    def debug_stop(self) -> Result:
        r"""Debug Stop.

        Wire: ``s\r\t``

        Stops the active debug session.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def debug_locals(self) -> Result:
        r"""Debug Locals.

        Wire: ``s\r\i``

        Dumps the local variables of the active debug session.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])
