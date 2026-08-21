"""Base class for generated menu bindings."""
from __future__ import annotations

from result import Err, Ok, Result

from . import encoding
from .transport import DEFAULT_TIMEOUT, Transport


class MenuBase:
    def __init__(self, transport: Transport, nav_path: str) -> None:
        self._transport = transport
        self._nav_path = nav_path  # hotkeys from the root, e.g. "i\\j"

    def _call(self, hotkey: str, args: "list[str]", returns: "list[str]",
              timeout: float = DEFAULT_TIMEOUT) -> Result:
        path = f"{self._nav_path}\\{hotkey}" if self._nav_path else hotkey
        cmd = path
        if args:
            cmd += " " + " ".join(args)
        self._transport.flush_queues()
        self._transport.send(cmd)
        frame = self._transport.wait_frame(timeout)
        if frame is None:
            return Err(f"timeout waiting for response to {cmd!r}")
        # The firmware echoes szMenuPrefix + the menu char. A mismatch means the
        # responding menu's prefix disagrees with the path we navigated, so the
        # frame cannot be trusted to belong to this command.
        if frame.path != path:
            # Not !r: repr() would double each backslash in these paths (e.g.
            # "i\\j\\s" -> "i\\\\j\\\\s"), making the message misleading about
            # what actually arrived on the wire.
            return Err(
                f"{cmd!r} got a response echoing path '{frame.path}', expected "
                f"'{path}'; the firmware menu prefix disagrees with the API tree")
        if not frame.success:
            return Err(f"{cmd!r} failed: {frame.response}")
        if not returns:
            return Ok(None)
        try:
            return Ok(encoding.decode_returns(returns, frame.response))
        except Exception as exc:  # short/odd responses decode defensively
            return Err(f"could not decode response {frame.response!r}: {exc}")
