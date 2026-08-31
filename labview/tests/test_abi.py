"""ABI behaviour of the shipped DLL: no hardware, no loopback build needed.

Everything here runs against the release binary, so it is testing exactly the
artifact a LabVIEW user loads.
"""

from __future__ import annotations

import ctypes
import threading

import pytest

from conftest import (ERR_ARG, ERR_BUFFER, ERR_SESSION, OK, sbuf, text)

STATUS_CODES = [0, 1, 2, 3, 4, 5, 6, 100, 101, 102, 103, 104, 105]


def test_pointer_size_reports_the_build_bitness(lib):
    assert lib.owlv_pointer_size() in (4, 8)


def test_version(lib):
    a, b, c = (ctypes.c_int() for _ in range(3))
    assert lib.owlv_version(ctypes.byref(a), ctypes.byref(b),
                            ctypes.byref(c)) == OK
    assert (a.value, b.value, c.value) >= (1, 0, 0)


@pytest.mark.parametrize("code", STATUS_CODES)
def test_every_status_code_has_a_message(lib, code):
    buf = sbuf()
    assert lib.owlv_status_message(code, buf, 256) == OK
    assert text(buf), f"status {code} has no message"


def test_unknown_status_still_returns_something(lib):
    buf = sbuf()
    assert lib.owlv_status_message(4242, buf, 256) == OK
    assert text(buf) == "unknown status"


def test_short_output_buffer_truncates_and_reports(lib):
    """The single most common LabVIEW mistake is an undersized output array.
    It must never overrun, and must say so rather than fail silently."""
    buf = sbuf(4)
    buf.raw = b"\xff" * 4
    rc = lib.owlv_status_message(3, buf, 4)
    assert rc == ERR_BUFFER
    assert len(text(buf)) == 3
    assert buf.raw[3] == 0


def test_zero_and_null_outputs_are_accepted(lib):
    """LabVIEW users leave optional outputs unwired; that must not be fatal."""
    assert lib.owlv_status_message(0, None, 0) == OK
    assert lib.owlv_status_message(0, sbuf(), 0) == OK
    assert lib.owlv_version(None, None, None) == OK


def test_port_enumeration_bounds(lib):
    count = ctypes.c_int(-1)
    assert lib.owlv_refresh_ports(ctypes.byref(count)) == OK
    assert count.value >= 0
    buf = sbuf()
    assert lib.owlv_port_name(-1, buf, 256) == ERR_ARG
    assert lib.owlv_port_name(count.value, buf, 256) == ERR_ARG
    kind = ctypes.c_int()
    assert lib.owlv_port_kind(10_000, ctypes.byref(kind)) == ERR_ARG


def test_port_fields_are_consistent(lib):
    count = ctypes.c_int()
    lib.owlv_refresh_ports(ctypes.byref(count))
    for i in range(count.value):
        name, desc = sbuf(64), sbuf(192)
        kind, vid, pid = (ctypes.c_int() for _ in range(3))
        assert lib.owlv_port_name(i, name, 64) == OK
        assert lib.owlv_port_description(i, desc, 192) == OK
        assert lib.owlv_port_kind(i, ctypes.byref(kind)) == OK
        assert lib.owlv_port_usb_ids(i, ctypes.byref(vid), ctypes.byref(pid)) == OK
        assert text(name)
        assert 0 <= kind.value <= 5
        assert 0 <= vid.value <= 0xFFFF and 0 <= pid.value <= 0xFFFF


def test_bad_session_handles_are_rejected(lib):
    valid = ctypes.c_int(-1)
    for bad in (0, -1, 1 << 30):
        assert lib.owlv_session_valid(bad, ctypes.byref(valid)) == OK
        assert valid.value == 0
        assert lib.owlv_session_port(bad, sbuf(), 256) == ERR_SESSION
    assert lib.owlv_close(4242) == OK          # idempotent, never an error
    assert lib.owlv_close_all() == OK


def test_open_rejects_an_empty_port(lib):
    session = ctypes.c_int(-1)
    assert lib.owlv_open(b"", ctypes.byref(session)) == ERR_ARG
    assert session.value == 0
    assert lib.owlv_open(None, ctypes.byref(session)) == ERR_ARG


def test_every_command_rejects_a_closed_session(lib, decls):
    """Sweep all 540 generated forwarders plus the core calls.

    Each must return OWLV_ERR_SESSION and touch nothing else. This is what
    catches a generator slip that dereferences an output before validating the
    handle -- in LabVIEW that is a hard crash of the whole IDE, not an error
    cluster. cdecl lets us call with only the session argument: every
    forwarder returns before reading a parameter.
    """
    skip = {"owlv_pointer_size", "owlv_version", "owlv_status_message",
            "owlv_open", "owlv_open_auto", "owlv_close", "owlv_close_all",
            "owlv_refresh_ports", "owlv_port_name", "owlv_port_description",
            "owlv_port_kind", "owlv_port_usb_ids", "owlv_find_port",
            "owlv_session_valid", "owlv_last_error"}
    checked, bad = 0, []
    for name in decls:
        if name in skip:
            continue
        fn = getattr(lib, name, None)
        if fn is None:
            bad.append(f"{name}: not exported")
            continue
        rc = fn(0)
        if rc != ERR_SESSION:
            bad.append(f"{name}: returned {rc}, expected {ERR_SESSION}")
        checked += 1
    assert checked >= 540, f"only swept {checked} functions"
    assert not bad, "\n".join(bad[:20])


def test_last_error_for_the_library_itself(lib):
    session = ctypes.c_int()
    lib.owlv_open(b"COM_does_not_exist", ctypes.byref(session))
    buf = sbuf()
    assert lib.owlv_last_error(0, buf, 256) == OK
    assert "COM_does_not_exist" in text(buf)


def test_concurrent_calls_do_not_corrupt_state(lib):
    """LabVIEW parallel loops hit the DLL from several threads at once. The
    session table and the port cache are shared; this is the smoke test that
    the locking holds."""
    errors = []

    def hammer():
        try:
            for _ in range(200):
                count = ctypes.c_int()
                lib.owlv_refresh_ports(ctypes.byref(count))
                buf = sbuf()
                assert lib.owlv_status_message(100, buf, 256) == OK
                assert text(buf) == "invalid or closed session handle"
                assert lib.owlv_io_gpio_set_io_toggle(0, 25) == ERR_SESSION
                assert lib.owlv_close(999) == OK
        except Exception as exc:            # noqa: BLE001 - reported below
            errors.append(exc)

    threads = [threading.Thread(target=hammer) for _ in range(8)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    assert not errors, errors[:3]
