"""Scripting Functions menu - generated from fwMenuScripting. Do not edit."""
from __future__ import annotations

from result import Result

from ..menubase import MenuBase
from ..transport import Transport
from .app_signals import AppSignals
from .wili_files import WiliFiles
from .zoom_io import ZoomIO
from .wasm_debug import WasmDebug
from .rthon_debug import RthonDebug


class Scripting(MenuBase):
    r"""Scripting Functions (``s``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "script": {"binary": False, "payload": [("data", "string")], "description": "Script engine output / status line (wasm and rThon runners)"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.app_signals = AppSignals(transport, nav_path + "\\i")
        self.wili_files = WiliFiles(transport, nav_path + "\\f")
        self.zoom_io = ZoomIO(transport, nav_path + "\\b")
        self.wasm_debug = WasmDebug(transport, nav_path + "\\w")
        self.rthon_debug = RthonDebug(transport, nav_path + "\\r")

    def launch_script(self) -> Result:
        r"""Launch Script.

        Wire: ``s\a``

        Not yet implemented; always reports failure

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def power_cycle_debugger(self) -> Result:
        r"""Power Cycle Debugger.

        Wire: ``s\c``

        Powers debugger zone 16 off for 500 ms, then powers it back on.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])
