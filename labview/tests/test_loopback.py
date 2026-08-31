"""Round-trip marshalling against the scripted loopback device.

Needs a DLL built with -DONEWILI_LV_LOOPBACK=ON; otherwise every test here
skips. The 540 forwarders under test are identical in the shipped build -- the
loopback only swaps the transport -- so this covers the real code path for
each argument shape the generator emits:

    scalar in / string in / byte-array in / bool in
    scalar out / bool out / string out / byte-array out

Each test asserts both halves: the wire line the encoder produced, and the
values the decoder handed back.
"""

from __future__ import annotations

import ctypes

import pytest

from conftest import (ERR_BUFFER, ERR_FAILED, ERR_LIMIT, ERR_SESSION, OK,
                      last_command, sbuf, set_response, text)


def test_session_opens_and_reports_its_label(lib, loop):
    buf = sbuf()
    assert lib.owlv_session_port(loop, buf, 256) == OK
    assert text(buf) == "loopback"
    valid = ctypes.c_int()
    assert lib.owlv_session_valid(loop, ctypes.byref(valid)) == OK
    assert valid.value == 1


def test_open_does_not_leave_a_stale_response_behind(lib, loop):
    """ow_open writes a bare 0x02 reset. If the device answered it, that frame
    would be read as the reply to the first real command."""
    set_response(lib, loop, "0000002A")
    gpio = ctypes.c_uint(0)
    assert lib.owlv_io_gpio_read_all(loop, ctypes.byref(gpio)) == OK
    assert gpio.value == 0x2A


# ---- argument encoding ------------------------------------------------------

def test_int_argument_encoding(lib, loop):
    set_response(lib, loop, "")
    assert lib.owlv_io_gpio_set_io_toggle(loop, 25) == OK
    assert last_command(lib, loop) == r"i\g\t 25"


def test_double_argument_encoding(lib, loop):
    lib.owlv_io_gpio_set_pwm.argtypes = [ctypes.c_int, ctypes.c_int,
                                         ctypes.c_double, ctypes.c_double]
    set_response(lib, loop, "")
    assert lib.owlv_io_gpio_set_pwm(loop, 25, 1000.0, 50.5) == OK
    assert last_command(lib, loop) == r"i\g\p 25 1000 50.5"


def test_string_argument_encoding(lib, loop):
    set_response(lib, loop, "")
    assert lib.owlv_apps_run_app(loop, b"blinky.uf2") == OK
    assert last_command(lib, loop) == r"a\r blinky.uf2"


def test_byte_array_argument_encoding(lib, loop):
    """`const unsigned char* x, int x_len` is wired as an Array Data Pointer
    plus its length; the encoder must hex-format exactly x_len bytes."""
    data = (ctypes.c_ubyte * 4)(0xDE, 0xAD, 0xBE, 0xEF)
    set_response(lib, loop, "")
    assert lib.owlv_io_i2c_i2c_write(loop, 0x50, 0x10, data, 4) == OK
    assert last_command(lib, loop) == r"i\i\w 50 10 DE AD BE EF"


def test_byte_array_length_shorter_than_the_array(lib, loop):
    data = (ctypes.c_ubyte * 4)(0xDE, 0xAD, 0xBE, 0xEF)
    set_response(lib, loop, "")
    assert lib.owlv_io_i2c_i2c_write(loop, 0x50, 0x10, data, 2) == OK
    assert last_command(lib, loop) == r"i\i\w 50 10 DE AD"


def test_empty_byte_array(lib, loop):
    set_response(lib, loop, "")
    assert lib.owlv_io_i2c_i2c_write(loop, 0x50, 0x10, None, 0) == OK
    assert last_command(lib, loop) == r"i\i\w 50 10"


def test_negative_length_is_clamped_not_wrapped(lib, loop):
    """LabVIEW hands us a signed I32. A negative value must not become a huge
    size_t and walk off the end of the array."""
    data = (ctypes.c_ubyte * 4)(1, 2, 3, 4)
    set_response(lib, loop, "")
    assert lib.owlv_io_i2c_i2c_write(loop, 0x50, 0x10, data, -1) == OK
    assert last_command(lib, loop) == r"i\i\w 50 10"


def test_bool_argument_encoding(lib, loop):
    """`bool` becomes `int` in the ABI; any non-zero must encode as 1."""
    set_response(lib, loop, "")
    assert lib.owlv_hardware_power_management_set_zone(loop, 6, 1) == OK
    assert last_command(lib, loop) == r"h\p\s 6 1"


# ---- output marshalling -----------------------------------------------------

def test_unsigned_scalar_output(lib, loop):
    set_response(lib, loop, "DEADBEEF")
    gpio = ctypes.c_uint(0)
    assert lib.owlv_io_gpio_read_all(loop, ctypes.byref(gpio)) == OK
    assert gpio.value == 0xDEADBEEF


def test_bool_output_is_normalised_to_0_or_1(lib, loop):
    enabled = ctypes.c_int(-1)
    set_response(lib, loop, "1")
    assert lib.owlv_hardware_system_event_host_streaming(
        loop, 1, ctypes.byref(enabled)) == OK
    assert enabled.value == 1
    set_response(lib, loop, "0")
    assert lib.owlv_hardware_system_event_host_streaming(
        loop, 0, ctypes.byref(enabled)) == OK
    assert enabled.value == 0


def test_string_output(lib, loop):
    set_response(lib, loop, "AA:BB:CC:DD:EE:FF")
    buf = sbuf(64)
    assert lib.owlv_wireless_esp32_flasher_read_esp32mac(loop, buf, 64) == OK
    assert text(buf) == "AA:BB:CC:DD:EE:FF"


def test_string_output_into_a_short_buffer(lib, loop):
    set_response(lib, loop, "AA:BB:CC:DD:EE:FF")
    buf = sbuf(8)
    rc = lib.owlv_wireless_esp32_flasher_read_esp32mac(loop, buf, 8)
    assert rc == ERR_BUFFER
    assert buf.raw[7] == 0


def test_byte_array_output(lib, loop):
    set_response(lib, loop, "41 42 43")
    buf = (ctypes.c_ubyte * 32)()
    n = ctypes.c_int(-1)
    assert lib.owlv_io_i2c_i2c_read(loop, buf, 32, ctypes.byref(n)) == OK
    assert n.value == 3
    assert bytes(buf[:3]) == b"ABC"


def test_byte_array_output_into_a_short_buffer(lib, loop):
    set_response(lib, loop, "41 42 43 44 45")
    buf = (ctypes.c_ubyte * 2)()
    n = ctypes.c_int(-1)
    assert lib.owlv_io_i2c_i2c_read(loop, buf, 2, ctypes.byref(n)) == ERR_BUFFER


def test_mixed_outputs_in_one_call(lib, loop):
    """Two signed scalars followed by a byte array -- the shape most likely to
    expose an off-by-one in the generated epilogue."""
    set_response(lib, loop, "-42 7 01 02 03 04")
    rssi, seq, n = ctypes.c_int(0), ctypes.c_int(0), ctypes.c_int(0)
    buf = (ctypes.c_ubyte * 16)()
    rc = lib.owlv_wireless_radio_packet_read(
        loop, ctypes.byref(rssi), ctypes.byref(seq), buf, 16, ctypes.byref(n))
    assert rc == OK
    assert rssi.value == -42
    assert seq.value == 7
    assert n.value == 4
    assert bytes(buf[:4]) == b"\x01\x02\x03\x04"


def test_outputs_are_left_alone_when_the_device_says_no(lib, loop):
    gpio = ctypes.c_uint(0x11111111)
    set_response(lib, loop, "DEADBEEF", ok=0)
    assert lib.owlv_io_gpio_read_all(loop, ctypes.byref(gpio)) == ERR_FAILED
    assert gpio.value == 0x11111111


def test_unwired_scalar_output_is_accepted(lib, loop):
    """LabVIEW users leave outputs they do not need unwired."""
    set_response(lib, loop, "DEADBEEF")
    assert lib.owlv_io_gpio_read_all(loop, None) == OK


def test_byte_array_outputs_are_not_optional(lib, loop):
    """Unlike scalars and strings, a byte-array output must be wired: the
    decoder in the C package rejects a missing buffer or length pointer.
    Pinned here because it is the one asymmetry in the ABI, and LabVIEW gives
    no hint that an unwired array terminal is different from an unwired
    numeric one."""
    from conftest import ERR_ARG
    set_response(lib, loop, "41 42")
    assert lib.owlv_io_i2c_i2c_read(loop, None, 0, None) == ERR_ARG


# ---- status and errors ------------------------------------------------------

def test_device_rejection_becomes_err_failed(lib, loop):
    set_response(lib, loop, "EPOWERZONE 6 FPGA", ok=0)
    assert lib.owlv_io_gpio_set_io_toggle(loop, 25) == ERR_FAILED
    buf = sbuf()
    assert lib.owlv_last_error(loop, buf, 256) == OK
    assert "device reported failure" in text(buf)


def test_last_error_clears_after_a_good_call(lib, loop):
    set_response(lib, loop, "", ok=0)
    assert lib.owlv_io_gpio_set_io_toggle(loop, 25) == ERR_FAILED
    set_response(lib, loop, "", ok=1)
    assert lib.owlv_io_gpio_set_io_toggle(loop, 25) == OK
    buf = sbuf()
    lib.owlv_last_error(loop, buf, 256)
    assert text(buf) == ""


# ---- events -----------------------------------------------------------------

def poll_event(lib, session):
    got, seq = ctypes.c_int(-1), ctypes.c_int(0)
    lo, hi = ctypes.c_uint(0), ctypes.c_uint(0)
    ident, args = sbuf(64), sbuf(512)
    rc = lib.owlv_poll_text_event(session, ctypes.byref(got), ident, 64,
                                  args, 512, ctypes.byref(lo), ctypes.byref(hi),
                                  ctypes.byref(seq))
    return rc, got.value, text(ident), text(args), (hi.value << 32) | lo.value, seq.value


def test_no_event_pending_is_not_an_error(lib, loop):
    rc, got, _, _, _, _ = poll_event(lib, loop)
    assert (rc, got) == (OK, 0)


def test_text_event_is_split_into_name_payload_and_metadata(lib, loop):
    """A real event line is a full frame: "[*name <hexTs> <seq> <payload> <ok>]".
    The wrapper hands LabVIEW the payload on its own so a diagram does not have
    to strip the frame's own fields."""
    lib.owlv_test_push_event(loop, b"uart1", b"41 42 43")
    rc, got, ident, args, ts, seq = poll_event(lib, loop)
    assert (rc, got) == (OK, 1)
    assert ident == "uart1"
    assert args == "41 42 43"
    assert ts == 0xDECAFBAD
    assert seq == 7


def test_event_with_an_empty_payload(lib, loop):
    lib.owlv_test_push_event(loop, b"filedl", b"")
    rc, got, ident, args, _, _ = poll_event(lib, loop)
    assert (rc, got, ident, args) == (OK, 1, "filedl", "")


def test_events_are_delivered_in_order(lib, loop):
    for i in range(3):
        lib.owlv_test_push_event(loop, b"power", str(i).encode())
    seen = []
    for _ in range(3):
        rc, got, ident, args, _, _ = poll_event(lib, loop)
        assert (rc, got, ident) == (OK, 1, "power")
        seen.append(args)
    assert seen == ["0", "1", "2"]


def test_event_arriving_during_a_command_is_queued_not_lost(lib, loop):
    """The device interleaves events with responses on the same port. An event
    that lands mid-command must be queued, not swallowed by the reply."""
    lib.owlv_test_push_event(loop, b"power", b"42")
    set_response(lib, loop, "0000000F")
    gpio = ctypes.c_uint()
    assert lib.owlv_io_gpio_read_all(loop, ctypes.byref(gpio)) == OK
    assert gpio.value == 0x0F

    rc, got, ident, args, _, _ = poll_event(lib, loop)
    assert (rc, got, ident, args) == (OK, 1, "power", "42")


def test_binary_calls_need_the_binary_port_open(lib, loop):
    from conftest import ERR_STATE
    kind = ctypes.c_int()
    assert lib.owlv_binary_poll(loop, ctypes.byref(kind)) == ERR_STATE
    ts_lo, ts_hi, bits = ctypes.c_uint(), ctypes.c_uint(), ctypes.c_uint()
    err = ctypes.c_int()
    assert lib.owlv_last_gpio_report(loop, ctypes.byref(ts_lo),
                                     ctypes.byref(ts_hi), ctypes.byref(bits),
                                     ctypes.byref(err)) == ERR_STATE


# ---- session lifecycle ------------------------------------------------------

def test_sessions_are_independent(lib, has_loopback):
    if not has_loopback:
        pytest.skip("built without -DONEWILI_LV_LOOPBACK=ON")
    a, b = ctypes.c_int(), ctypes.c_int()
    assert lib.owlv_test_open(ctypes.byref(a)) == OK
    assert lib.owlv_test_open(ctypes.byref(b)) == OK
    assert a.value != b.value
    try:
        set_response(lib, a.value, "AAAAAAAA")
        set_response(lib, b.value, "BBBBBBBB")
        ga, gb = ctypes.c_uint(), ctypes.c_uint()
        assert lib.owlv_io_gpio_read_all(a.value, ctypes.byref(ga)) == OK
        assert lib.owlv_io_gpio_read_all(b.value, ctypes.byref(gb)) == OK
        assert (ga.value, gb.value) == (0xAAAAAAAA, 0xBBBBBBBB)
    finally:
        lib.owlv_test_close(a.value)
        lib.owlv_test_close(b.value)


def test_handles_are_not_reused_after_close(lib, has_loopback):
    """A stale handle from an aborted VI must not address a new session."""
    if not has_loopback:
        pytest.skip("built without -DONEWILI_LV_LOOPBACK=ON")
    first = ctypes.c_int()
    assert lib.owlv_test_open(ctypes.byref(first)) == OK
    stale = first.value
    assert lib.owlv_test_close(stale) == OK
    second = ctypes.c_int()
    assert lib.owlv_test_open(ctypes.byref(second)) == OK
    try:
        assert second.value != stale
        assert lib.owlv_io_gpio_set_io_toggle(stale, 25) == ERR_SESSION
    finally:
        lib.owlv_test_close(second.value)


def test_the_session_table_has_a_hard_limit(lib, has_loopback):
    if not has_loopback:
        pytest.skip("built without -DONEWILI_LV_LOOPBACK=ON")
    lib.owlv_close_all()
    opened = []
    try:
        while True:
            s = ctypes.c_int()
            rc = lib.owlv_test_open(ctypes.byref(s))
            if rc == ERR_LIMIT:
                break
            assert rc == OK
            opened.append(s.value)
            assert len(opened) <= 32
        assert len(opened) == 32
    finally:
        for s in opened:
            lib.owlv_test_close(s)


def test_close_all_releases_everything(lib, has_loopback):
    if not has_loopback:
        pytest.skip("built without -DONEWILI_LV_LOOPBACK=ON")
    sessions = []
    for _ in range(4):
        s = ctypes.c_int()
        assert lib.owlv_test_open(ctypes.byref(s)) == OK
        sessions.append(s.value)
    assert lib.owlv_close_all() == OK
    for s in sessions:
        valid = ctypes.c_int(1)
        lib.owlv_session_valid(s, ctypes.byref(valid))
        assert valid.value == 0


# ---- known upstream defect --------------------------------------------------

@pytest.mark.xfail(reason="upstream: c/src/onewili.c decodes the first of three "
                          "fields with ow__rest_str, which swallows the rest of "
                          "the response. Affects ow_hardware_system_device_state "
                          "and ow_scripting_app_signals_app_signal_get.",
                   strict=True)
def test_device_state_decodes_three_fields(lib, loop):
    set_response(lib, loop, "main 1 0")
    sd, mask = sbuf(64), sbuf(64)
    host = ctypes.c_int(-1)
    rc = lib.owlv_hardware_system_device_state(loop, sd, 64,
                                               ctypes.byref(host), mask, 64)
    assert rc == OK
    assert text(sd) == "main"
    assert host.value == 1
    assert text(mask) == "0"
