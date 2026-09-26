"""Argument encoders / response decoders for OneWili wire types."""
from __future__ import annotations


def enc_int(v) -> str:
    return str(int(v))


def enc_hex(v, width: int = 0) -> str:
    return f"{int(v):0{width}X}"


def enc_bytes(data) -> str:
    return " ".join(f"{b:02X}" for b in bytes(data))


def enc_bool(v) -> str:
    return "1" if v else "0"


def enc_str(v) -> str:
    return str(v)


def enc_float(v) -> str:
    return str(float(v))


def enc_color(v) -> str:
    # The firmware reads an UNPREFIXED numeric token as DECIMAL (fwDecodeColor
    # in rmpLib/fwColorDecode.cpp), so bare hex is not a colour: "FF8000" is no
    # known name and decodes to black, and digits that happen to parse as
    # decimal ("102030") give an arbitrary wrong colour. Prefixing makes the
    # int form unambiguous. Strings pass through as written: "#FF8000",
    # "rgb(255,128,0)", "red".
    return f"0x{v:06X}" if isinstance(v, int) else str(v)


def decode_returns(spec: "list[str]", response: str):
    """Decode a frame's response text per the return spec.

    Spec codes: int (decimal token), hex (hex token), bool, float,
    enum:<Name> (int token, mapped to the named IntEnum when known), and the
    greedy tail codes bytes / str which consume all remaining tokens.
    Returns a single value for one-element specs, else a tuple.
    """
    tokens = response.split()
    out = []
    i = 0
    for field, kind in enumerate(spec):
        if kind == "bytes":
            out.append(bytes(int(t, 16) for t in tokens[i:]))
            i = len(tokens)
        elif kind == "str":
            # Only a final string is a free-form tail. Intermediate strings
            # are single tokens (e.g. Device State: sd, bool, mask, clock).
            if field == len(spec) - 1:
                out.append(" ".join(tokens[i:]))
                i = len(tokens)
            else:
                out.append(tokens[i])
                i += 1
        else:
            tok = tokens[i]
            i += 1
            if kind == "int":
                out.append(int(tok, 10))
            elif kind == "hex":
                out.append(int(tok, 16))
            elif kind == "bool":
                out.append(tok == "1")
            elif kind == "float":
                out.append(float(tok))
            elif kind.startswith("enum:"):
                v = int(tok, 10)
                from . import enums as _enums          # lazy: avoids cycles
                cls = getattr(_enums, kind[5:], None)
                if cls is None:
                    out.append(v)
                else:
                    try:
                        out.append(cls(v))
                    except ValueError:                  # newer firmware value
                        out.append(v)
            else:
                out.append(tok)
    return out[0] if len(out) == 1 else tuple(out)
