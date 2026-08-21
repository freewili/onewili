"""Every generated wire path must produce a response frame the host can match.

The firmware emits responses as ``printOutAlways("[%s%c ", szMenuPrefix,
m_cCurrentMenuChar)``, i.e. ``"[" + <full path> + " "``. A line that fails
framing.FRAME_RE never reaches Transport.frames, so _call blocks until timeout.
"""
from __future__ import annotations

import json
from pathlib import Path

import pytest

from onewili import framing

MANIFEST = Path(__file__).resolve().parent.parent / "onewili" / "api_manifest.json"


def _commands() -> "list[tuple[str, str, str]]":
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    out = []
    for class_name, cls in manifest["classes"].items():
        for method in cls.get("methods", []):
            wire = method.get("wire")
            if wire:
                out.append((class_name, method["name"], wire))
    return out


COMMANDS = _commands()


def test_manifest_has_commands():
    assert COMMANDS, f"no commands with a wire path found in {MANIFEST}"


@pytest.mark.parametrize("class_name,method,wire", COMMANDS,
                         ids=[f"{c}.{m}" for c, m, _ in COMMANDS])
def test_wire_path_is_frame_matchable(class_name: str, method: str, wire: str):
    line = f"[{wire} 0 0 ok 1]"
    assert framing.FRAME_RE.match(line), (
        f"{class_name}.{method} wire path {wire!r} yields {line!r}, which "
        f"FRAME_RE cannot match -- its responses would be dropped"
    )


@pytest.mark.parametrize("class_name,method,wire", COMMANDS,
                         ids=[f"{c}.{m}" for c, m, _ in COMMANDS])
def test_wire_path_fits_the_firmware_prefix_buffer(class_name: str, method: str, wire: str):
    # fwMenuX::szMenuPrefix is char[16]. The prefix for a command at depth N is
    # the N-1 parent segments each followed by '\', so 2*(N-1) chars, + NUL.
    segments = wire.split("\\")
    prefix_len = 2 * (len(segments) - 1)
    assert prefix_len + 1 <= 16, (
        f"{class_name}.{method} wire path {wire!r} needs a {prefix_len}-char "
        f"prefix, which does not fit fwMenuX::szMenuPrefix[16]"
    )


def test_parsed_frame_round_trips_the_path():
    _, _, wire = COMMANDS[0]
    frame = framing.ResponseFrame.parse(f"[{wire} 0 0 ok 1]")
    assert frame.path == wire


def test_frame_re_permits_digit_hotkeys_but_still_rejects_malformed_structure():
    # Amendment: four shipped commands use digit leaf hotkeys (e.g. "w\\a\\0",
    # "h\\s\\1"). The firmware's hotkey matcher handles digits fine, so FRAME_RE
    # was the bug, not the wire path. Widening it to allow digits must not
    # loosen the structural guarantees: a leading char, exactly one backslash
    # between segments, and a trailing space.
    assert not framing.FRAME_RE.match("[\\i\\a\\s ")  # leading backslash
    assert not framing.FRAME_RE.match("[i\\\\a s ")   # doubled separator
    assert not framing.FRAME_RE.match("[i\\a\\ ")     # trailing backslash, no leaf char
    assert framing.FRAME_RE.match("[w\\a\\0 ")
    assert framing.FRAME_RE.match("[h\\s\\1 ")
