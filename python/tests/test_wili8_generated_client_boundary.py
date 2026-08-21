"""Wili8 generated-client calls preserve the exact final path field bytes."""
from __future__ import annotations

import pytest

from onewili import framing
from onewili.menus.gui_controls import GUIControls


class CaptureTransport:
    def __init__(self) -> None:
        self.sent: "list[str]" = []

    def flush_queues(self) -> None:
        pass

    def send(self, command: str) -> None:
        self.sent.append(command)

    def wait_frame(self, timeout: float):
        return framing.ResponseFrame.parse("[g\\b\\n 0 0 1]")


@pytest.mark.parametrize(
    ("path", "suffix"),
    [
        ("", " 0 "),
        ('""', ' 0 ""'),
        ('"\\scripts\\demo.wasm"', ' 255 "\\scripts\\demo.wasm"'),
    ],
)
def test_add_wili8_emits_the_exact_final_path_field(path: str, suffix: str):
    transport = CaptureTransport()
    controls = GUIControls(transport, "g\\b")

    animation = 255 if "demo.wasm" in path else 0
    result = controls.add_wili8(
        0, 1, 2, 128, 128, 1, "#010203", animation, path
    )

    assert result.is_ok()
    assert transport.sent == [
        "g\\b\\n 0 1 2 128 128 1 #010203" + suffix
    ]


def test_add_wili8_rejects_an_omitted_ninth_argument_before_transport():
    transport = CaptureTransport()
    controls = GUIControls(transport, "g\\b")

    with pytest.raises(TypeError):
        controls.add_wili8(0, 1, 2, 128, 128, 1, "#010203", 0)

    assert transport.sent == []
