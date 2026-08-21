"""_call must reject a response whose echoed path is not the path it sent."""
from __future__ import annotations

from onewili import framing
from onewili.menubase import MenuBase


class FakeTransport:
    """Minimal Transport stand-in: hands back one canned frame."""

    def __init__(self, frame_line: str) -> None:
        self._frame_line = frame_line
        self.sent: "list[str]" = []

    def flush_queues(self) -> None:
        pass

    def send(self, cmd: str) -> None:
        self.sent.append(cmd)

    def wait_frame(self, timeout: float):
        return framing.ResponseFrame.parse(self._frame_line)


def test_matching_path_succeeds():
    t = FakeTransport("[i\\j\\s 0 0 1]")
    menu = MenuBase(t, "i\\j")
    result = menu._call("s", [], [])
    assert result.is_ok()
    assert t.sent == ["i\\j\\s"]


def test_mismatched_path_is_an_error():
    # Firmware echoed the stale "e\" prefix instead of the real path.
    t = FakeTransport("[e\\s 0 0 1]")
    menu = MenuBase(t, "i\\j")
    result = menu._call("s", [], [])
    assert result.is_err()
    assert "e\\s" in result.unwrap_err()


def test_root_level_call_matches():
    t = FakeTransport("[i\\g 0 0 1]")
    menu = MenuBase(t, "i")
    result = menu._call("g", [], [])
    assert result.is_ok()
