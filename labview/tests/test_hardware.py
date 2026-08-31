"""End-to-end tests against a real FreeWili. Skipped unless you ask for them:

    pytest tests --port auto
    pytest tests --port COM466

These are the only tests that touch a device. They stay read-only by default:
nothing here changes a setting, writes a file, or switches a power rail. The
few tests that would need a rail on are gated behind --power-zones as well,
because a test run should leave the board exactly as it found it.
"""

from __future__ import annotations

import ctypes

import pytest

from conftest import (ERR_FAILED, ERR_NO_DEVICE, ERR_SESSION, OK, sbuf,
                      set_response, text)

PORT_MAIN, PORT_BINARY = 1, 3


@pytest.fixture(scope="module")
def port(request):
    p = request.config.getoption("--port")
    if not p:
        pytest.skip("no --port given (use --port auto)")
    return p


@pytest.fixture
def session(lib, port):
    s = ctypes.c_int(0)
    used = sbuf(64)
    if port == "auto":
        rc = lib.owlv_open_auto(ctypes.byref(s), used, 64)
    else:
        rc = lib.owlv_open(port.encode(), ctypes.byref(s))
    if rc != OK:
        err = sbuf()
        lib.owlv_last_error(0, err, 256)
        pytest.fail(f"could not open {port}: status {rc} ({text(err)})")
    yield s.value
    lib.owlv_close(s.value)


@pytest.fixture
def power_zones(request):
    if not request.config.getoption("--power-zones"):
        pytest.skip("needs --power-zones (the run would change board rails)")


def test_discovery_finds_the_main_port(lib, port):
    """The USB VID/PID table has to keep matching what the board enumerates
    as; this is the test that catches a new PID in a firmware revision."""
    buf = sbuf(64)
    assert lib.owlv_find_port(PORT_MAIN, buf, 64) == OK, \
        "no port identified as the FreeWili main CPU"
    assert text(buf).startswith(("COM", "/dev/"))


def test_discovery_finds_the_binary_port(lib, port):
    buf = sbuf(64)
    rc = lib.owlv_find_port(PORT_BINARY, buf, 64)
    if rc == ERR_NO_DEVICE:
        pytest.skip("no FTDI binary port present on this board")
    assert rc == OK


def test_open_and_close(lib, session):
    valid = ctypes.c_int(0)
    assert lib.owlv_session_valid(session, ctypes.byref(valid)) == OK
    assert valid.value == 1
    buf = sbuf(64)
    assert lib.owlv_session_port(session, buf, 64) == OK
    assert text(buf)


def test_round_trip_against_the_firmware(lib, session):
    """Read-only, needs no power zone: the command that reports which power
    rails are on. Proves framing, timing and the 1 Mbaud link end to end."""
    buf = sbuf(4096)
    rc = lib.owlv_send_raw(session, rb"h\p\g", buf, 4096)
    assert rc == OK, f"h\\p\\g failed: {rc}"
    body = text(buf)
    assert "mask" in body and "Sensors" in body


def test_a_generated_command_round_trips(lib, session):
    """owlv_hardware_power_management_get_zones has no outputs, so this checks
    the status path only -- but through the generated forwarder rather than
    the raw escape hatch."""
    assert lib.owlv_hardware_power_management_get_zones(session) == OK


def test_string_and_bool_outputs_from_the_device(lib, session):
    """The device answers "main 1 0" here. Currently xfail-adjacent: the C
    package's decoder swallows all three fields into the first one, so this
    asserts the status the wrapper actually returns today rather than the one
    it should. See test_loopback.py::test_device_state_decodes_three_fields."""
    sd, mask = sbuf(64), sbuf(64)
    host = ctypes.c_int(-1)
    rc = lib.owlv_hardware_system_device_state(session, sd, 64,
                                               ctypes.byref(host), mask, 64)
    assert rc in (OK, 5), f"unexpected status {rc}"


def test_a_rejected_command_reports_the_device_reason(lib, session):
    """With the FPGA rail off, GPIO is refused. The wrapper must surface that
    as OWLV_ERR_FAILED with the device's own message, not as a timeout."""
    gpio = ctypes.c_uint()
    rc = lib.owlv_io_gpio_read_all(session, ctypes.byref(gpio))
    if rc == OK:
        pytest.skip("the FPGA rail is already on; nothing is being refused")
    assert rc == ERR_FAILED
    err = sbuf()
    lib.owlv_last_error(session, err, 256)
    assert "device reported failure" in text(err)


def test_events_arrive_and_are_split(lib, session):
    """Turn on the power telemetry stream just long enough to catch a few
    events, then turn it off again. This is the only test that changes device
    state, and it restores it in a finally."""
    poll = lib.owlv_poll_text_event
    got, seq = ctypes.c_int(), ctypes.c_int()
    lo, hi = ctypes.c_uint(), ctypes.c_uint()
    ident, args = sbuf(64), sbuf(4096)

    assert lib.owlv_hardware_power_management_enable_power_stream(session, 200) == OK
    try:
        seen = []
        import time
        deadline = time.monotonic() + 3.0
        while time.monotonic() < deadline and len(seen) < 3:
            rc = poll(session, ctypes.byref(got), ident, 64, args, 4096,
                      ctypes.byref(lo), ctypes.byref(hi), ctypes.byref(seq))
            assert rc == OK, f"poll failed: {rc}"
            if got.value:
                seen.append((text(ident), text(args),
                             (hi.value << 32) | lo.value, seq.value))
            else:
                time.sleep(0.02)
    finally:
        lib.owlv_hardware_power_management_enable_power_stream(session, 0)

    assert seen, "no events arrived in 3 s of streaming"
    widths = set()
    for name, payload, ts, sequence in seen:
        assert name == "power"
        fields = payload.split()
        # The power event is a fixed list of integers (soc, current_ma, ...,
        # valid). Every token parsing as one means the frame's own timestamp,
        # sequence and ok flag were all stripped and nothing else was. Note
        # the payload's last field is itself a 0/1 bool, so "does it end in 1"
        # proves nothing -- the field count is what pins the split.
        assert len(fields) >= 10, f"payload looks truncated: {payload!r}"
        for tok in fields:
            int(tok)
        widths.add(len(fields))
        assert ts > 0, "timestamp did not decode"
        assert sequence > 0, "sequence did not decode"
    assert len(widths) == 1, f"payload width varied across events: {widths}"
    assert [s for *_, s in seen] == sorted(s for *_, s in seen), \
        "sequence numbers went backwards"


def test_gpio_toggle(lib, session, power_zones):
    """The canonical example from the OneWili README. Needs the FPGA rail, so
    it only runs with --power-zones; it restores the rail afterwards."""
    was_on = ctypes.c_uint()
    already = lib.owlv_io_gpio_read_all(session, ctypes.byref(was_on)) == OK
    if not already:
        assert lib.owlv_hardware_power_management_set_zone(session, 6, 1) == OK
    try:
        assert lib.owlv_io_gpio_read_all(session, ctypes.byref(was_on)) == OK
        assert lib.owlv_io_gpio_set_io_toggle(session, 25) == OK
        after = ctypes.c_uint()
        assert lib.owlv_io_gpio_read_all(session, ctypes.byref(after)) == OK
        assert (was_on.value ^ after.value) & (1 << 25), \
            "GPIO 25 did not change state"
    finally:
        if not already:
            lib.owlv_hardware_power_management_set_zone(session, 6, 0)


def test_a_closed_session_stops_working(lib, port):
    s = ctypes.c_int()
    if port == "auto":
        assert lib.owlv_open_auto(ctypes.byref(s), None, 0) == OK
    else:
        assert lib.owlv_open(port.encode(), ctypes.byref(s)) == OK
    assert lib.owlv_close(s.value) == OK
    assert lib.owlv_hardware_power_management_get_zones(s.value) == ERR_SESSION


def test_the_port_can_be_reopened_after_close(lib, port):
    """An aborted VI leaves the port open; owlv_close_all must really release
    it, or the next run cannot connect."""
    for _ in range(3):
        s = ctypes.c_int()
        if port == "auto":
            rc = lib.owlv_open_auto(ctypes.byref(s), None, 0)
        else:
            rc = lib.owlv_open(port.encode(), ctypes.byref(s))
        assert rc == OK, f"reopen failed with {rc}"
        assert lib.owlv_close_all() == OK
