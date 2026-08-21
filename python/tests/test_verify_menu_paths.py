"""Tests for examples/verify_menu_paths.py (Task 7 review, fix round 1).

Critical 2 of that review: no test exercised `main()`/`probe_settings` at
all, so there was no proof the sweep could ever report a clean pass, and no
proof it could tell a genuine path defect apart from its own frame-in-flight
desync (Critical 1). These tests cover both, using fakes -- no serial port
or hardware required.

`examples/` is a script directory, not a package, so the module under test
is loaded by file path rather than by package import.
"""
from __future__ import annotations

import importlib.util
from pathlib import Path

from onewili import framing
from onewili.device import OneWili
from onewili.menubase import MenuBase

_SCRIPT_PATH = (
    Path(__file__).resolve().parent.parent / "examples" / "verify_menu_paths.py"
)
_spec = importlib.util.spec_from_file_location("verify_menu_paths", _SCRIPT_PATH)
vmp = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(vmp)


def _frame(path: str, sequence: int, response: str = "1", success: bool = True):
    return framing.ResponseFrame(
        path=path, timestamp_ns=0, sequence=sequence, response=response, success=success)


# ---------------------------------------------------------------------------
# Fakes
# ---------------------------------------------------------------------------

class AlwaysCorrectTransport:
    """Every '<path>?' query is answered immediately and correctly, with a
    fresh, monotonically increasing sequence number. Proves a clean pass is
    reachable at all (Critical 2)."""

    def __init__(self) -> None:
        self._pending: "str | None" = None
        self._next_seq = 1
        self.sent: "list[str]" = []

    def flush_queues(self) -> None:
        pass

    def send(self, cmd: str) -> None:
        self.sent.append(cmd)
        assert cmd.endswith("?"), f"sweep must send a query, got {cmd!r}"
        self._pending = cmd[:-1]

    def wait_frame(self, timeout: float):
        if self._pending is None:
            return None
        path, self._pending = self._pending, None
        seq, self._next_seq = self._next_seq, self._next_seq + 1
        return _frame(path, seq)


class FirstSendDelayedTransport:
    """A real, correct reply exists for every command, but the very first
    command ever sent takes one extra wait_frame() call to become visible --
    modelling a one-time initial hiccup. Every later reply (including a
    retry of the same command) is delivered on the very first check.

    Proves Critical 1's primary defense (drain before every send) lets the
    sweep self-heal from a transient lag instead of misattributing another
    probe's answer to this one.
    """

    def __init__(self) -> None:
        self._pending: "str | None" = None
        self._delay_ticks = 0
        self._next_seq = 100
        self.sent: "list[str]" = []

    def flush_queues(self) -> None:
        pass

    def send(self, cmd: str) -> None:
        self.sent.append(cmd)
        self._pending = cmd[:-1]
        self._delay_ticks = 1 if len(self.sent) == 1 else 0

    def wait_frame(self, timeout: float):
        if self._pending is None:
            return None
        if self._delay_ticks > 0:
            self._delay_ticks -= 1
            return None
        path, self._pending = self._pending, None
        seq, self._next_seq = self._next_seq, self._next_seq + 1
        return _frame(path, seq)


class StaleThenFreshTransport:
    """The first reply handed back happens to echo the *right* path text but
    carries an old sequence number (as if it were a duplicate of some earlier,
    already-consumed exchange) -- proving the sweep does not trust a path
    match by itself. The second (retried) attempt gets a proper fresh reply.
    """

    def __init__(self, path: str, stale_seq: int, fresh_seq: int) -> None:
        self._path = path
        self._stale_seq = stale_seq
        self._fresh_seq = fresh_seq
        self._calls = 0
        self.sent: "list[str]" = []

    def flush_queues(self) -> None:
        pass

    def send(self, cmd: str) -> None:
        self.sent.append(cmd)

    def wait_frame(self, timeout: float):
        self._calls += 1
        if self._calls == 1:
            return None  # attempt 1's pre-send drain: wire looks idle
        if self._calls == 2:
            return _frame(self._path, self._stale_seq)  # attempt 1's reply: stale
        if self._calls == 3:
            return None  # attempt 2's pre-send drain: genuinely idle now
        return _frame(self._path, self._fresh_seq)  # attempt 2's reply: fresh


class PersistentlyStaleTransport:
    """Every reply, including the retry, comes back with an old sequence
    number. Proves an unrecoverable desync is reported as a distinct
    failure reason, not mislabelled as a plain path mismatch."""

    def __init__(self, path: str, stale_seq: int) -> None:
        self._path = path
        self._stale_seq = stale_seq
        self._calls = 0
        self.sent: "list[str]" = []

    def flush_queues(self) -> None:
        pass

    def send(self, cmd: str) -> None:
        self.sent.append(cmd)

    def wait_frame(self, timeout: float):
        self._calls += 1
        if self._calls % 2 == 1:
            return None  # every pre-send drain: wire looks idle
        return _frame(self._path, self._stale_seq)  # every reply: still stale


class AlwaysWrongPathTransport:
    """Every reply arrives promptly with a fresh, ever-increasing sequence
    number, but echoes the wrong path -- a genuine firmware derivation
    defect, not a desync. Proves this is still reported as a real mismatch
    (not swallowed/relabelled as "stale") once sequence-freshness confirms
    it isn't just a leftover frame."""

    def __init__(self, wrong_path: str) -> None:
        self._wrong_path = wrong_path
        self._pending = False
        self._next_seq = 1
        self.sent: "list[str]" = []

    def flush_queues(self) -> None:
        pass

    def send(self, cmd: str) -> None:
        self.sent.append(cmd)
        self._pending = True

    def wait_frame(self, timeout: float):
        if not self._pending:
            return None
        self._pending = False
        seq, self._next_seq = self._next_seq, self._next_seq + 1
        return _frame(self._wrong_path, seq)


# ---------------------------------------------------------------------------
# Critical 2: a clean pass must be reachable, and reachable for every mount
# of a dual-parented class (Important 4).
# ---------------------------------------------------------------------------

def _manifest():
    import json
    return json.loads(vmp.MANIFEST.read_text(encoding="utf-8"))


def test_run_sweep_reports_all_verified_against_correct_firmware():
    manifest = _manifest()
    class_hotkeys = vmp.build_class_hotkeys(manifest)
    device = OneWili(transport=AlwaysCorrectTransport())

    verified, failed, unverified = vmp.run_sweep(device, class_hotkeys)

    assert failed == []
    assert len(verified) > 0
    verified_names = {dotted for dotted, _ in verified}
    # Both mounts of a dual-parented class (Important 4) must be probed and
    # verified independently -- not one verified and one silently
    # "unverified" because the lookup only knew about a single canonical
    # mount's wire path.
    assert "io.uart.settings" in verified_names
    assert "hardware.settings_home.uart_settings" in verified_names


def test_build_class_hotkeys_is_keyed_by_class_not_by_mount_path():
    manifest = _manifest()
    class_hotkeys = vmp.build_class_hotkeys(manifest)
    assert "UARTSettings" in class_hotkeys
    assert set(class_hotkeys["UARTSettings"]) == {"f", "r", "c", "b", "p", "s", "m"}


# ---------------------------------------------------------------------------
# Critical 1: frame-in-flight desync must not be silently misattributed.
# ---------------------------------------------------------------------------

def test_probe_settings_self_heals_a_one_time_initial_lag():
    transport = FirstSendDelayedTransport()
    state = {"last_seq": -1}

    expected, frame, stale, _ = vmp.probe_settings(transport, "h\\s\\u", "f", state)

    assert frame is not None
    assert not stale
    assert frame.path == expected == "h\\s\\u\\f"


def test_probe_settings_does_not_trust_a_stale_frame_with_a_matching_path():
    # Simulate a sweep that has already consumed responses up through seq 500;
    # the first reply to *this* probe happens to echo the right path but is
    # really an old (already-accounted-for) frame.
    transport = StaleThenFreshTransport(path="h\\s\\u\\f", stale_seq=10, fresh_seq=999)
    state = {"last_seq": 500}

    expected, frame, stale, _ = vmp.probe_settings(transport, "h\\s\\u", "f", state)

    assert frame is not None
    assert not stale  # the retry's fresh (seq 999) frame is what gets trusted
    assert frame.sequence == 999
    assert frame.path == expected


def test_probe_settings_reports_persistent_desync_distinctly():
    transport = PersistentlyStaleTransport(path="h\\s\\u\\f", stale_seq=10)
    state = {"last_seq": 500}

    expected, frame, stale, prior_seq = vmp.probe_settings(transport, "h\\s\\u", "f", state)

    assert frame is not None
    assert stale


class _TinyDevice:
    """Minimal fake device: settings-bearing MenuBase attributes at known
    nav_paths, walked and probed via the real run_sweep -- no OneWili/real
    manifest needed for tests that only care about categorization."""

    def __init__(self, transport, nav_paths):
        self._transport = transport
        for name, nav in nav_paths.items():
            setattr(self, name, MenuBase(transport, nav))


class TwoTargetTransport:
    """`first` (nav "a") answers correctly with a high sequence number,
    establishing a baseline the way a real sweep naturally builds one up
    over earlier menus. `second` (nav "b") always echoes back a
    path-matching reply carrying a sequence number *lower* than that
    baseline -- proving run_sweep's own accumulated sequence state (not just
    a per-probe comparison) catches a leftover frame that a naive
    path-only check would have accepted as "second"'s real answer."""

    def __init__(self) -> None:
        self._pending: "str | None" = None
        self.sent: "list[str]" = []

    def flush_queues(self) -> None:
        pass

    def send(self, cmd: str) -> None:
        self.sent.append(cmd)
        self._pending = cmd[:-1]  # strip the trailing '?'

    def wait_frame(self, timeout: float):
        if self._pending is None:
            return None
        path, self._pending = self._pending, None
        if path == "a\\f":
            return _frame(path, 500)
        return _frame(path, 10)  # "b\\f": always a stale sequence, forever


def test_run_sweep_flags_a_stale_frame_distinctly_from_a_real_mismatch():
    transport = TwoTargetTransport()
    device = _TinyDevice(transport, {"first": "a", "second": "b"})
    class_hotkeys = {"MenuBase": ["f"]}

    verified, failed, unverified = vmp.run_sweep(device, class_hotkeys)

    assert unverified == []
    verified_names = {dotted for dotted, _ in verified}
    failed_by_name = {dotted: why for dotted, _, why in failed}
    assert "first" in verified_names
    assert "second" in failed_by_name
    # Distinct wording from a genuine mismatch (never "echoed ..."), so a
    # reader isn't misled into treating a desync as a confirmed firmware bug.
    assert "desync" in failed_by_name["second"]
    assert "echoed" not in failed_by_name["second"]


def test_probe_settings_reports_a_genuine_mismatch_as_a_real_defect_not_stale():
    transport = AlwaysWrongPathTransport(wrong_path="z\\u\\f")
    state = {"last_seq": -1}

    expected, frame, stale, _ = vmp.probe_settings(transport, "h\\s\\u", "f", state)

    assert frame is not None
    assert not stale  # fresh, ever-increasing sequence -- this is not a desync
    assert frame.path != expected
    assert frame.path == "z\\u\\f"


def test_run_sweep_end_to_end_distinguishes_desync_from_a_real_defect():
    """A single dual-mounted class's *two* live mounts: one echoes back
    correctly, the other has a genuine (stale-firmware-style) wrong prefix.
    run_sweep must report exactly one FAIL, with the real-mismatch wording,
    and the other mount verified -- proving the two failure categories don't
    bleed into each other across a real walk of the tree."""
    manifest = _manifest()
    class_hotkeys = vmp.build_class_hotkeys(manifest)

    class MixedTransport:
        """io.uart.settings (i\\u\\s\\f) answers correctly; every other
        query gets the stale literal 'z\\u\\f' seen against real hardware."""

        def __init__(self):
            self._pending = None
            self._next_seq = 1
            self.sent = []

        def flush_queues(self):
            pass

        def send(self, cmd):
            self.sent.append(cmd)
            self._pending = cmd

        def wait_frame(self, timeout):
            if self._pending is None:
                return None
            cmd, self._pending = self._pending, None
            seq, self._next_seq = self._next_seq, self._next_seq + 1
            if cmd == "i\\u\\s\\f?":
                return _frame("i\\u\\s\\f", seq)
            return _frame("z\\u\\f", seq)

    device = OneWili(transport=MixedTransport())
    verified, failed, unverified = vmp.run_sweep(device, class_hotkeys)

    verified_names = {dotted for dotted, _ in verified}
    failed_by_name = {dotted: why for dotted, _, why in failed}

    assert "io.uart.settings" in verified_names
    assert "hardware.settings_home.uart_settings" in failed_by_name
    assert failed_by_name["hardware.settings_home.uart_settings"] == "echoed 'z\\\\u\\\\f'"
