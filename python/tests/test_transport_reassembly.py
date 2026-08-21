"""A framed response split across physical lines must still reach `frames`.

Captured during wiliamp bring-up: ``h\\p\\t`` replied with a newline inside the
payload, so the reader split the frame in two, neither piece parsed, both fell
through to the raw `lines` queue and _call timed out. Reassembly is the
transport's job -- see Transport._route.
"""
from __future__ import annotations

import threading
import time

import pytest

from onewili import transport as transport_mod
from onewili.menubase import MenuBase
from onewili.transport import Transport

# The exact bytes off the wire for h\p\t (dev.hardware.power_management.
# get_power_state()). The payload's trailing newline lands before the closing
# " 0]" token, so the frame arrives as two lines.
SPLIT_CAPTURE = (b"[h\\p\\t 00000362DE8A02E8 4 "
                 b"No telemetry sample yet - run Stream Power first\n 0]\n")


class FakeSerial:
    """pyserial stand-in; read()/write() are lock-guarded so the reader thread
    and the test thread can share it like a real serial handle."""

    def __init__(self) -> None:
        self.out = bytearray()
        self.written = bytearray()
        self._lock = threading.Lock()

    def write(self, data: bytes) -> int:
        with self._lock:
            self.written += data
        return len(data)

    def read(self, n: int) -> bytes:
        with self._lock:
            take = bytes(self.out[:n])
            del self.out[:len(take)]
        if not take:
            time.sleep(0.002)  # real Serial blocks for its timeout
        return take

    def close(self) -> None:
        pass

    def queue(self, data: bytes) -> None:
        with self._lock:
            self.out += data


@pytest.fixture
def wired() -> "tuple[Transport, FakeSerial]":
    fake = FakeSerial()
    t = Transport("FAKE")
    t._serial = fake
    t._running = True
    t._reader = threading.Thread(target=t._read_loop, daemon=True)
    t._reader.start()
    try:
        yield t, fake
    finally:
        t._running = False
        t._pause_requested.clear()
        t._reader.join(timeout=2.0)
        assert not t._reader.is_alive(), "reader thread failed to stop"


def drain(q) -> "list[str]":
    out = []
    while not q.empty():
        out.append(q.get_nowait())
    return out


def wait_until(predicate, timeout: float = 3.0) -> None:
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        if predicate():
            return
        time.sleep(0.005)
    raise AssertionError("timed out waiting for condition")


def test_payload_newline_still_yields_one_frame(wired):
    t, fake = wired
    fake.queue(SPLIT_CAPTURE)

    frame = t.wait_frame(timeout=2.0)
    assert frame is not None, f"frame lost; lines queue holds {drain(t.lines)!r}"
    assert frame.path == "h\\p\\t"
    assert frame.timestamp_ns == 0x00000362DE8A02E8
    assert frame.sequence == 4
    assert frame.response == "No telemetry sample yet - run Stream Power first"
    # ok=0 here is an app-level "nothing cached yet", not a transport failure.
    assert frame.success is False
    assert drain(t.lines) == [], "frame pieces leaked into the raw lines queue"


def test_split_frame_reaches_the_caller_as_a_result_not_a_timeout(wired):
    # The user-visible symptom: _call used to time out. It must now surface the
    # firmware's own ok=0 message.
    t, fake = wired

    def responder() -> None:
        wait_until(lambda: b"h\\p\\t" in bytes(fake.written))
        fake.queue(SPLIT_CAPTURE)

    threading.Thread(target=responder, daemon=True).start()
    result = MenuBase(t, "h\\p")._call("t", [], [], timeout=2.0)

    assert result.is_err()
    err = result.unwrap_err()
    assert "timeout" not in err, err
    assert "No telemetry sample yet - run Stream Power first" in err, err


def test_payload_may_end_a_line_with_a_bracket(wired):
    # A naive "buffer until a line ends with ']'" would close on the first
    # line here and emit a truncated frame.
    t, fake = wired
    fake.queue(b"[i\\w 1 7 see [note]\nand [more] 1]\n")

    frame = t.wait_frame(timeout=2.0)
    assert frame is not None, f"frame lost; lines={drain(t.lines)!r}"
    assert frame.response == "see [note]\nand [more]"
    assert frame.success is True
    assert drain(t.lines) == []


def test_payload_spanning_four_lines(wired):
    # h\p\t's success path: three newline-terminated printOutAlways calls
    # (fwMenuPowerManagement.cpp:184-191), then "Ok" and the closing token.
    t, fake = wired
    fake.queue(b"[h\\p\\t 00000362DE8A02E8 9 "
               b"soc 88%  current -120 mA  capacity 1800/2000 mAh\n"
               b"vbat 3900 mV  vsys 4100 mV  vbus 0 mV  ichg 0 mA\n"
               b"chg 1  vbus 0  fault 0  zones 0x1FFFF  tier 3/2\n"
               b"Ok 1]\n")

    frame = t.wait_frame(timeout=2.0)
    assert frame is not None, f"frame lost; lines={drain(t.lines)!r}"
    assert frame.sequence == 9
    assert frame.success is True
    assert frame.response.splitlines() == [
        "soc 88%  current -120 mA  capacity 1800/2000 mAh",
        "vbat 3900 mV  vsys 4100 mV  vbus 0 mV  ichg 0 mA",
        "chg 1  vbus 0  fault 0  zones 0x1FFFF  tier 3/2",
        "Ok",
    ]
    assert drain(t.lines) == []


def test_blank_payload_lines_survive_but_bare_ones_do_not(wired):
    # fwMenuNFC.cpp:134 prints "\n=== Card Information ===\n\n", so a payload
    # blank line is real; a blank line outside a frame is still noise.
    t, fake = wired
    fake.queue(b"\n[i\\n\\c 12AB 5 \n=== Card Information ===\n\nuid 04A2 1]\n")

    frame = t.wait_frame(timeout=2.0)
    assert frame is not None, f"frame lost; lines={drain(t.lines)!r}"
    assert frame.response.splitlines() == ["", "=== Card Information ===", "", "uid 04A2"]
    assert drain(t.lines) == []


def test_single_line_frame_is_unaffected(wired):
    t, fake = wired
    fake.queue(b"[i\\w 5 1 ok 1]\n")

    frame = t.wait_frame(timeout=2.0)
    assert frame is not None
    assert (frame.path, frame.response, frame.success) == ("i\\w", "ok", True)
    assert drain(t.lines) == []


def test_unframed_chatter_still_reaches_the_lines_queue(wired):
    t, fake = wired
    fake.queue(b"FreeWili booting\nmenu: pick one\n")

    wait_until(lambda: t.lines.qsize() == 2)
    assert drain(t.lines) == ["FreeWili booting", "menu: pick one"]
    assert t.wait_frame(timeout=0.1) is None


def test_a_new_frame_abandons_a_half_received_one(wired):
    t, fake = wired
    fake.queue(b"[a\\b 1 1 dangling\n[c 2 2 done 1]\n")

    frame = t.wait_frame(timeout=2.0)
    assert frame is not None
    assert (frame.path, frame.response) == ("c", "done")
    wait_until(lambda: t.lines.qsize() == 1)
    assert drain(t.lines) == ["[a\\b 1 1 dangling"]


def test_an_unclosed_frame_expires_instead_of_swallowing_later_frames(wired, monkeypatch):
    t, fake = wired
    monkeypatch.setattr(transport_mod, "PENDING_FRAME_TIMEOUT", 0.05)
    fake.queue(b"[a\\b 1 1 never closes\n")

    wait_until(lambda: t.lines.qsize() == 1)
    assert drain(t.lines) == ["[a\\b 1 1 never closes"]

    fake.queue(b"[c 2 2 done 1]\n")
    frame = t.wait_frame(timeout=2.0)
    assert frame is not None, "the expired frame swallowed the next one"
    assert frame.response == "done"


def test_an_unclosed_frame_is_capped_by_length(wired, monkeypatch):
    t, fake = wired
    monkeypatch.setattr(transport_mod, "PENDING_FRAME_TIMEOUT", 30.0)
    filler = b"x" * 200
    fake.queue(b"[a\\b 1 1 runaway\n" + (filler + b"\n") * 30)

    # 30 * 201 chars is well past MAX_PENDING_FRAME_CHARS, so the buffer is
    # given up on without waiting for the (deliberately long) timeout.
    wait_until(lambda: t.lines.qsize() > 0)
    assert t._router.pending is None
    fake.queue(b"[c 2 2 done 1]\n")
    frame = t.wait_frame(timeout=2.0)
    assert frame is not None
    assert frame.response == "done"


def test_a_split_event_is_reassembled_too(wired):
    # Events come off the same printOutAlways chokepoint as responses, and the
    # NFC menu alone has 39 newline-bearing calls.
    t, fake = wired
    fake.queue(b"[*nfcCard 12AB 5 === Card Information ===\nuid 04A2 1]\n")

    event = t.events.get(timeout=2.0)
    assert event.path == "*nfcCard"
    assert event.response == "=== Card Information ===\nuid 04A2"
    assert event.success is True
    assert t.wait_frame(timeout=0.1) is None
    assert drain(t.lines) == []


def test_a_split_event_displaces_a_half_received_response(wired):
    # The documented cost of holding one frame at a time: a continuation line
    # has no identity, so a split event arriving mid-response gives up on the
    # response rather than guessing. It degrades to the old behaviour (pieces
    # to the raw queue), never to a wrong frame.
    t, fake = wired
    fake.queue(b"[h\\p\\t 1 4 first half\n"
               b"[*nfcCard 1 5 split event\n"
               b"more 1]\n")

    event = t.events.get(timeout=2.0)
    assert event.path == "*nfcCard"
    assert event.response == "split event\nmore"
    assert t.wait_frame(timeout=0.1) is None
    wait_until(lambda: t.lines.qsize() == 1)
    assert drain(t.lines) == ["[h\\p\\t 1 4 first half"]


def test_an_event_may_interleave_a_half_received_frame(wired):
    t, fake = wired
    fake.queue(b"[h\\p\\t 00000362DE8A02E8 4 first half\n"
               b"[*gpioEvent 1 5 24 1 1]\n"
               b"second half 0]\n")

    frame = t.wait_frame(timeout=2.0)
    assert frame is not None, f"frame lost; lines={drain(t.lines)!r}"
    assert frame.path == "h\\p\\t"
    assert frame.response == "first half\nsecond half"
    assert frame.success is False

    event = t.events.get(timeout=1.0)
    assert event.path == "*gpioEvent"
    assert drain(t.lines) == []
