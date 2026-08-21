"""WASM Debug menu - generated from fwMenuWasmDebug. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class WasmDebug(MenuBase):
    r"""WASM Debug (``s\w``)."""

    def debug_start(self, path: str) -> Result:
        r"""WASM Debug Start.

        Wire: ``s\w\c``

        Loads a .wilwasm for debugging.

        path startPaused [bytePcs...]

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_str(path)], [])

    def debug_breakpoints(self, pcs: str) -> Result:
        r"""WASM Debug Breakpoints.

        Wire: ``s\w\j``

        Replaces the byte-PC breakpoint set.

        [bytePcs...]

        Args:
            pcs: pcs (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_str(pcs)], [])

    def debug_step(self, range: str) -> Result:
        r"""WASM Debug Step.

        Wire: ``s\w\e``

        Steps one opcode, or until the PC leaves [lo,hi).

        [lo hi]

        Args:
            range: range (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_str(range)], [])

    def debug_continue(self) -> Result:
        r"""WASM Debug Continue.

        Wire: ``s\w\f``

        Resumes until the next breakpoint.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [], [])

    def debug_pause(self) -> Result:
        r"""WASM Debug Pause.

        Wire: ``s\w\g``

        Pauses at the next opcode.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [], [])

    def debug_stop(self) -> Result:
        r"""WASM Debug Stop.

        Wire: ``s\w\t``

        Stops the active wasm debug session.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def debug_locals(self) -> Result:
        r"""WASM Debug Locals.

        Wire: ``s\w\i``

        Dumps stack frames and raw frame-0 locals.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def debug_mem_read(self, addr: str) -> Result:
        r"""WASM Debug Memory Read.

        Wire: ``s\w\r``

        Reads up to 64 bytes of wasm linear memory (hex).

        addr len

        Args:
            addr: addr (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_str(addr)], [])
