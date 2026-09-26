import pathlib
import sys
import time
import types

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "python"))
sys.path.insert(0, str(ROOT / "cm0/python"))
import onewili_cm0 as cm0
from onewili import encoding


FAKE = r'''
import sys, time
mode = sys.argv[1]
if mode == 'refuse':
    print('fwcm0: MAIN did not answer HELLO', flush=True)
    sys.exit(1)
if mode == 'silent':
    time.sleep(10)
for raw in sys.stdin.buffer:
    path = raw.lstrip(b'\x02').decode().strip().split(' ')[0]
    if path == 'h\\a\\g':
        print('[h\\a\\g 1 1 main 1 0 150000000 none 1]', flush=True)
        if mode == 'disconnect':
            sys.exit(0)
    else:
        print('[' + path + ' 2 2 first line\nsecond line 1]', flush=True)
if mode == 'graceful':
    time.sleep(.1)  # stand in for the daemon releasing its session on EOF
'''


@pytest.fixture
def connection(tmp_path):
    script = tmp_path / "console.py"
    script.write_text(FAKE, encoding="utf-8")
    opened = []

    def make(mode="normal", timeout=1.0):
        t = cm0.Cm0Transport(timeout=timeout)
        t._argv = [sys.executable, "-u", str(script), mode]
        opened.append(t)
        return t

    yield make
    for t in opened:
        t.close()


def test_open_proves_main_and_reassembles_multiline_response(connection):
    t = connection()
    t.open()
    t.send("i\\g\\u")
    frame = t.wait_frame(1)
    assert frame.path == "i\\g\\u"
    assert frame.success
    assert "first line" in frame.response and "second line" in frame.response


def test_refused_hello_fails_during_open_and_preserves_reason(connection):
    t = connection("refuse")
    with pytest.raises(RuntimeError, match="MAIN did not answer HELLO"):
        t.open()
    assert t._proc is None
    assert t._reader is None


def test_unresponsive_bridge_has_bounded_open_and_is_reaped(connection):
    t = connection("silent", timeout=.15)
    start = time.monotonic()
    with pytest.raises(RuntimeError, match="connection probe"):
        t.open()
    assert time.monotonic() - start < 2
    assert t._proc is None


def test_disconnect_cannot_silently_accept_commands(connection):
    t = connection("disconnect")
    t.open()
    assert t._ended.wait(1)
    with pytest.raises(RuntimeError, match="closed"):
        t.send("i\\g\\u")
    with pytest.raises(RuntimeError, match="closed"):
        t.wait_frame(1)


def test_close_then_reconnect(connection):
    t = connection()
    for _ in range(3):
        t.open()
        t.send("i\\g\\u")
        assert t.wait_frame(1).success
        t.close()


def test_close_allows_cli_to_finish_session_release(connection):
    t = connection("graceful")
    t.open()
    proc = t._proc
    t.close()
    assert proc.returncode == 0


def test_device_state_strings_do_not_consume_scalar_fields():
    assert encoding.decode_returns(["str", "bool", "str", "int"],
                                   "main 1 0 150000000 none") == ("main", True, "0", 150000000)
    assert encoding.decode_returns(["int", "str"], "2 a free form tail") == (2, "a free form tail")
