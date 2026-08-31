"""Shared fixtures for the OneWili LabVIEW wrapper tests.

The suite drives the DLL through `ctypes`, which is deliberate: ctypes loads a
library by name, declares argument types by hand and passes pre-allocated
buffers for outputs -- the same model a LabVIEW Call Library Function Node
uses. A signature that ctypes can express and call correctly is one the
wizard can generate a working VI for. It is the closest proxy for LabVIEW
that runs without LabVIEW.

Point the suite at a specific build with ONEWILI_LV_DLL, otherwise it takes
the first of:

    bin/test/onewili_lv.dll     the loopback build (runs every test)
    bin/win64, bin/win32        the shipped builds (loopback tests skip)
    build-mingw, build/x64, build/x86
"""

from __future__ import annotations

import ctypes
import os
import re
import sys
from pathlib import Path

import pytest

LV = Path(__file__).resolve().parent.parent
CORE_HEADER = LV / "include" / "onewili_lv.h"
API_HEADER = LV / "include" / "onewili_lv_api.h"
DEF_FILE = LV / "onewili_lv.def"

CANDIDATES = [
    "bin/test", "bin/win64", "bin/win32",
    "build-mingw", "build/x64", "build/x86", "build",
]
LIB_NAMES = ["onewili_lv.dll", "libonewili_lv.so", "libonewili_lv.dylib",
             "onewili_lv.so", "onewili_lv.dylib"]


def find_library() -> Path:
    env = os.environ.get("ONEWILI_LV_DLL")
    if env:
        p = Path(env)
        if not p.is_file():
            raise FileNotFoundError(f"ONEWILI_LV_DLL={env} does not exist")
        return p
    for rel in CANDIDATES:
        for name in LIB_NAMES:
            p = LV / rel / name
            if p.is_file():
                return p
    raise FileNotFoundError(
        "no onewili_lv library found. Build one first:\n"
        "    cd labview && .\\build.ps1 -Tests\n"
        "or set ONEWILI_LV_DLL to its path."
    )


def declarations() -> dict[str, list[str]]:
    """{function name: [parameter type strings]} from the two public headers.

    Parsed rather than imported so the tests check the file the Import Shared
    Library Wizard actually reads, not a hand-maintained copy of it.
    """
    text = CORE_HEADER.read_text(encoding="utf-8") + \
        API_HEADER.read_text(encoding="utf-8")
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    out: dict[str, list[str]] = {}
    for m in re.finditer(r"\bint\s+(owlv_\w+)\s*\(([^;{)]*)\)\s*;", text, re.S):
        name, raw = m.group(1), " ".join(m.group(2).split())
        params = []
        for chunk in raw.split(","):
            chunk = chunk.strip()
            if not chunk or chunk == "void":
                continue
            params.append(re.sub(r"\s*\b[A-Za-z_]\w*$", "", chunk).strip())
        out[name] = params
    return out


@pytest.fixture(scope="session")
def lib_path() -> Path:
    return find_library()


@pytest.fixture(scope="session")
def lib(lib_path: Path):
    """The DLL, loaded cdecl -- the calling convention LabVIEW calls 'C'."""
    if sys.platform == "win32" and hasattr(os, "add_dll_directory"):
        # MinGW builds can pull in libgcc/libwinpthread from beside them.
        os.add_dll_directory(str(lib_path.parent))
    return ctypes.CDLL(str(lib_path))


@pytest.fixture(scope="session")
def decls() -> dict[str, list[str]]:
    return declarations()


@pytest.fixture(scope="session")
def has_loopback(lib) -> bool:
    return hasattr(lib, "owlv_test_open")


@pytest.fixture
def loop(lib, has_loopback):
    """A session backed by the scripted loopback device, closed on teardown."""
    if not has_loopback:
        pytest.skip("built without -DONEWILI_LV_LOOPBACK=ON")
    session = ctypes.c_int(0)
    rc = lib.owlv_test_open(ctypes.byref(session))
    assert rc == 0, f"owlv_test_open failed: {rc}"
    yield session.value
    lib.owlv_test_close(session.value)


# ---- helpers the tests share ------------------------------------------------

OK, ERR_ARG, ERR_IO, ERR_TIMEOUT = 0, 1, 2, 3
ERR_FAILED, ERR_PROTOCOL, ERR_BUFFER = 4, 5, 6
ERR_SESSION, ERR_NO_DEVICE, ERR_OPEN = 100, 101, 102
ERR_LIMIT, ERR_STATE, ERR_UNSUPPORTED = 103, 104, 105


def sbuf(n: int = 256):
    """An output string buffer, the way a LabVIEW U8 array output is wired."""
    return ctypes.create_string_buffer(n)


def text(buf) -> str:
    """Trim at the first NUL, as a LabVIEW diagram has to."""
    return buf.raw.split(b"\0", 1)[0].decode("ascii", "replace")


def set_response(lib, session, body: str, ok: int = 1):
    rc = lib.owlv_test_set_response(session, body.encode(), ok)
    assert rc == 0, f"owlv_test_set_response failed: {rc}"


def last_command(lib, session) -> str:
    buf = sbuf(1024)
    rc = lib.owlv_test_last_command(session, buf, 1024)
    assert rc == 0, f"owlv_test_last_command failed: {rc}"
    return text(buf)


def pytest_addoption(parser):
    parser.addoption(
        "--port", default=os.environ.get("ONEWILI_LV_PORT"),
        help="serial port of a real FreeWili, or 'auto'; enables the hardware "
             "tests (they are skipped without it)")
    parser.addoption(
        "--power-zones", action="store_true",
        help="allow the hardware tests to switch device power zones on. Off "
             "by default: a test run should not change the board's rails.")
