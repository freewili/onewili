"""A Color argument must reach the firmware as a token its decoder accepts.

The authority is the firmware, not this binding: menutool emits
``obMenuParser.parseColor(...)`` for a color argument, which reaches
``fwDecodeColor`` (rmpLib/fwColorDecode.cpp:228). That function reads an
UNPREFIXED numeric token as DECIMAL, so bare hex is not a colour at all. The
generated signatures advertise ``int | str``, so both forms have to work.

fw_decode_color below is a port of that function, branch for branch, and is
pinned against values measured off-target during the colour investigation.
"""
from __future__ import annotations

import pytest

from onewili import encoding

# fwColorDecode.cpp's table, only the entries used here.
NAMED = {"black": 0x000000, "red": 0xFF0000, "orange": 0xFFA500, "white": 0xFFFFFF}

HEX_DIGITS = "0123456789abcdef"


def _scan_uint(s: str, base: int) -> "int | None":
    """sscanf("%u"/"%x") semantics: consume the leading digit run and ignore
    whatever follows. This is what makes a bare hex token dangerous."""
    digits = HEX_DIGITS[:base]
    run = ""
    for c in s:
        if c not in digits:
            break
        run += c
    return int(run, base) if run else None


def fw_decode_color(token: str) -> int:
    """Port of fwDecodeColor (rmpLib/fwColorDecode.cpp:228-296)."""
    if not token:
        return 0
    if len(token) > 31:  # toLowerBounded's char buf[32]
        return 0
    buf = token.lower()

    if buf[0] == "#":
        body = buf[1:]
        if len(buf) == 7 and all(c in HEX_DIGITS for c in body):
            return int(body, 16) & 0xFFFFFF
        if len(buf) == 4 and all(c in HEX_DIGITS for c in body):
            r, g, b = (int(c, 16) for c in body)
            return ((r * 0x11) << 16) | ((g * 0x11) << 8) | (b * 0x11)
        return 0

    if buf.startswith("0x"):
        v = _scan_uint(buf[2:], 16)
        return (v & 0xFFFFFF) if v is not None else 0

    if buf[0].isdigit():  # decimal, NOT hex
        v = _scan_uint(buf, 10)
        return (v & 0xFFFFFF) if v is not None else 0

    for prefix, count in (("rgba(", 4), ("rgb(", 3)):
        if buf.startswith(prefix):
            parts = buf[len(prefix):].rstrip(")").split(",")
            if len(parts) < count:
                return 0
            try:
                vals = [int(p) for p in parts[:count]]
            except ValueError:
                return 0
            r, g, b = (min(max(v, 0), 255) for v in vals[:3])
            return (r << 16) | (g << 8) | b

    return NAMED.get(buf, 0)


# --- the oracle itself, pinned to the off-target measurements -------------

@pytest.mark.parametrize("token,expected", [
    ("FF8000", 0x000000),    # bare hex is no known name -> black
    ("0xFF8000", 0xFF8000),
    ("#FF8000", 0xFF8000),
    ("16744448", 0xFF8000),  # decimal 16744448
    ("102030", 0x018E8E),    # valid decimal -> silently the WRONG colour
])
def test_oracle_reproduces_the_measured_decoder(token: str, expected: int):
    assert fw_decode_color(token) == expected


# --- the int form: the path that was broken -------------------------------

def test_int_form_is_prefixed():
    assert encoding.enc_color(0xFF8000) == "0xFF8000"


@pytest.mark.parametrize("value", [
    0x000000, 0x0000FF, 0xFF8000, 0xFFFFFF,
    0x102030,  # digits that are also valid decimal: the silent-wrong-colour case
    0x018E8E,  # what the old encoder's "102030" actually painted
])
def test_int_form_round_trips_through_the_decoder(value: int):
    assert fw_decode_color(encoding.enc_color(value)) == value


@pytest.mark.parametrize("value", [0x000000, 0x0000FF, 0xFF8000, 0x102030, 0xFFFFFF])
def test_int_form_is_never_an_unprefixed_token(value: int):
    # The regression guard: an unprefixed token is decimal (or a name lookup),
    # never hex, so the encoder must not emit one for an int.
    token = encoding.enc_color(value)
    assert token.startswith("0x"), token
    assert len(token) >= len("0x000000")


def test_the_old_bare_hex_form_would_have_painted_black():
    # What enc_color used to emit for 0xFF8000, kept as the reason this is a bug.
    assert fw_decode_color(f"{0xFF8000:06X}") == 0x000000
    assert fw_decode_color(f"{0x102030:06X}") != 0x102030


# --- the string form must keep passing through untouched ------------------

@pytest.mark.parametrize("token", [
    "#FF8000", "0xFF8000", "16744448", "rgb(255,128,0)", "rgba(255,128,0,128)",
    "red", "orange", "#f80",
])
def test_string_forms_pass_through_verbatim(token: str):
    assert encoding.enc_color(token) == token


@pytest.mark.parametrize("token,expected", [
    ("#FF8000", 0xFF8000),
    ("0xFF8000", 0xFF8000),
    ("rgb(255,128,0)", 0xFF8000),
    ("rgba(255,128,0,128)", 0xFF8000),
    ("red", 0xFF0000),
    ("orange", 0xFFA500),
    ("#f80", 0xFF8800),
])
def test_string_forms_still_decode(token: str, expected: int):
    assert fw_decode_color(encoding.enc_color(token)) == expected
