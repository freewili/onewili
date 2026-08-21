"""Verify every firmware menu echoes the path it was navigated to.

Each menu's szMenuPrefix is derived at pushSubMenu time from its parent's
prefix plus the hotkey taken. This walks the generated menu tree and checks
the device agrees, using only read-only probes.

Usage:
    python examples/verify_menu_paths.py [--port COM42]

Exit code 0 when every reachable menu verified, 1 otherwise.

Implementation notes (Task 7 review, fix round 1):

- Correlation (Critical 1). `Transport` does not correlate requests with
  responses on its own -- `flush_queues()` only discards what has already
  arrived; a reply still travelling over the wire from the *previous* probe
  can land moments later and be handed to the *next* probe's wait_frame(),
  silently shifting every following result by one. Against real hardware
  this produced `failed: 16` where every "echoed" value was actually the
  answer to the previous probe -- a result that is unreachable to fix no
  matter what the firmware does. Addressed two ways: (1) draining to
  quiescence (polling until nothing arrives for a settle window) before
  every send, and (2) tracking each ResponseFrame's monotonic `sequence`
  number (framing.py ResponseFrame.sequence) so a frame that is not newer
  than the last one consumed is recognized as a leftover rather than
  trusted at face value. A path mismatch or a stale sequence triggers one
  drain-and-retry before a result is recorded.
- Multi-parent classes (Important 4). A firmware menu object can be mounted
  under two different parents (e.g. `UARTSettings` under both
  `io.uart.settings` and `hardware.settings_home.uart_settings`) -- exactly
  the case push-time prefix derivation (Task 2) has to get right at every
  mount. The manifest's `wire` field records only one canonical mount per
  class, so keying probes by `wire`'s nav prefix silently drops every other
  mount. Probes are keyed by class name instead (`build_class_hotkeys`), and
  the expected path is re-derived from the *walked instance's* `_nav_path`
  each time, which is correct regardless of which mount is being visited.
- Raw transport access (Important 5). There is no generated, public
  "settings query" method: generated setters (e.g.
  `UARTSettings.baud_rate(value)`) always *write*, and `MenuBase._call`
  (menubase.py) assumes the echoed path equals the exact string it sent --
  which does not hold for a "<path>?" query, since the firmware echoes
  "<path>" without the trailing "?". Reusing `_call` unmodified would flag
  every successful query as a mismatch. This module therefore still reaches
  into `device._transport` deliberately (not as an oversight), centralized
  into `_drain_to_quiescence`/`probe_settings` below. If `menubase.py`'s
  path-mismatch check (Task 6) ever changes, re-check the logic here by
  hand -- there is no shared code path to keep them in sync automatically.
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

import onewili
from onewili.menubase import MenuBase
from onewili.transport import DEFAULT_TIMEOUT

MANIFEST = Path(__file__).resolve().parent.parent / "onewili" / "api_manifest.json"
# Settle window for _drain_to_quiescence: how long to wait for "nothing more
# is arriving" before trusting the pipe is idle. Independent of the
# request/response timeout (DEFAULT_TIMEOUT) used for the actual probe.
DRAIN_POLL_TIMEOUT = 0.15


def walk_menus(node, path_attr=""):
    """Yield (dotted_attr, menu) for every MenuBase in the tree, depth-first."""
    for name, value in vars(node).items():
        if name.startswith("_"):
            continue
        if isinstance(value, MenuBase):
            dotted = f"{path_attr}.{name}" if path_attr else name
            yield dotted, value
            yield from walk_menus(value, dotted)


def build_class_hotkeys(manifest) -> "dict[str, list[str]]":
    """class shortName -> hotkeys of its settings items (read-only '?' probes).

    Keyed by class, not by the manifest's `wire` path. A class's settings
    hotkeys are intrinsic to that firmware menu object and do not depend on
    which parent mounted it -- unlike `wire`, which records only one
    canonical mount and silently drops every other one (Important 4).
    """
    out: "dict[str, list[str]]" = {}
    for cls_name, cls in manifest["classes"].items():
        hotkeys = [
            method["wire"].rpartition("\\")[2]
            for method in cls.get("methods", [])
            if method.get("wire") and method.get("is_setting")
        ]
        if hotkeys:
            out[cls_name] = hotkeys
    return out


def _drain_to_quiescence(transport, state: dict, poll_timeout: float = DRAIN_POLL_TIMEOUT) -> None:
    """Consume any frames still in flight until none arrive for poll_timeout.

    flush_queues() only discards what has already arrived; a response to an
    earlier command can still be travelling over the wire at that instant
    and land moments later, in time to be handed to the *next* probe. Only a
    settle window proves the pipe is actually idle before we send (Critical 1).
    Every frame consumed here (a genuine leftover) still advances the
    sequence baseline, so it cannot later be mistaken for a fresh reply.
    """
    transport.flush_queues()
    while True:
        frame = transport.wait_frame(poll_timeout)
        if frame is None:
            return
        state["last_seq"] = max(state["last_seq"], frame.sequence)


def probe_settings(transport, nav_path: str, hotkey: str, state: dict,
                    timeout: float = DEFAULT_TIMEOUT):
    """Send '<nav>\\<hk>?' and correlate the reply by sequence number.

    Returns (expected_path, frame_or_None, stale, prior_seq).

    `state["last_seq"]` is the highest ResponseFrame.sequence consumed so
    far across the whole sweep. A reply whose sequence does not exceed it is
    a leftover from an earlier command, not this one's real answer -- a
    path-text match alone is not proof, since a stale frame can coincidentally
    echo the right path (e.g. a duplicate of an earlier, identical query).
    If the first attempt is missing, stale, or path-mismatched, this drains
    fully and retries exactly once before returning a verdict.
    """
    expected = f"{nav_path}\\{hotkey}"
    cmd = f"{expected}?"

    def attempt():
        _drain_to_quiescence(transport, state)
        prior_seq = state["last_seq"]
        transport.send(cmd)
        frame = transport.wait_frame(timeout)
        if frame is not None:
            state["last_seq"] = max(state["last_seq"], frame.sequence)
        stale = frame is not None and frame.sequence <= prior_seq
        return frame, stale, prior_seq

    frame, stale, prior_seq = attempt()
    if frame is None or stale or frame.path != expected:
        # Possibly a leftover frame from a previous probe (Critical 1) --
        # drain fully and retry exactly once before trusting the result.
        frame, stale, prior_seq = attempt()

    return expected, frame, stale, prior_seq


def run_sweep(device, class_hotkeys, timeout: float = DEFAULT_TIMEOUT):
    """Walk `device`'s menu tree and probe every settings-bearing menu.

    Hardware-independent: `device` need only expose MenuBase-typed attributes
    (recursively, via walk_menus) plus a `_transport` duck-typed with
    flush_queues()/send()/wait_frame(), so tests can drive this with a fake
    transport and no serial port at all.
    """
    transport = device._transport
    state = {"last_seq": -1}
    verified, failed, unverified = [], [], []
    for dotted, menu in walk_menus(device):
        nav = menu._nav_path
        hotkeys = class_hotkeys.get(type(menu).__name__) or []
        if not hotkeys:
            unverified.append((dotted, nav))
            continue
        hotkey = hotkeys[0]
        expected, frame, stale, prior_seq = probe_settings(transport, nav, hotkey, state, timeout)
        if frame is None:
            failed.append((dotted, expected, "no frame (dropped or timed out), even after one drain+retry"))
        elif stale:
            failed.append((dotted, expected,
                f"stale/out-of-sequence frame (seq {frame.sequence} <= previously-seen "
                f"{prior_seq}) even after one drain+retry; response pipe may be desynced, "
                f"not a confirmed path defect"))
        elif frame.path != expected:
            failed.append((dotted, expected, f"echoed {frame.path!r}"))
        else:
            verified.append((dotted, expected))
    return verified, failed, unverified


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", default=None, help="serial port (default: auto-detect)")
    args = ap.parse_args()

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    class_hotkeys = build_class_hotkeys(manifest)

    device = onewili.connect(args.port) if args.port else onewili.connect()

    with device:
        verified, failed, unverified = run_sweep(device, class_hotkeys)

    print(f"verified   : {len(verified)}")
    for dotted, path in verified:
        print(f"  OK   {dotted:<40} {path}")
    print(f"failed     : {len(failed)}")
    for dotted, expected, why in failed:
        print(f"  FAIL {dotted:<40} expected {expected} -- {why}")
    print(f"unverified : {len(unverified)}  (no settings item; needs a manual probe)")
    for dotted, nav in unverified:
        print(f"  ????  {dotted:<40} {nav}")

    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
