"""Response-frame parsing for the OneWili serial protocol.

Frames look like ``[<path> <hexTimestampNs> <seq> <response...> <ok>]`` where
path is the echoed menu path (e.g. ``i\\w``), the timestamp is big-endian hex
nanoseconds and ok is a trailing 0/1. Event frames use ``[*name ...]``.

A response whose payload contains a newline reaches the host as several
physical lines. transport.py rejoins those with join_frame_lines() before
parsing, so parse() only ever sees a whole frame.
"""
from __future__ import annotations

import re
from dataclasses import dataclass

# FRAME_RE is deliberately loose enough to match a false positive like
# "[0 1 2 3]" -- the real backstop against that is ResponseFrame.parse's
# stricter grammar (>=4 tokens, trailing "]", hex timestamp, int sequence)
# below, combined with transport.py's `except ValueError` around it.
FRAME_RE = re.compile(r"^\[[a-zA-Z0-9?](\\[a-zA-Z0-9])* ")
EVENT_RE = re.compile(r"^\[\*\w+ ")
# The closing " <ok>]" token, as printed by rpConsole::printMenuResponse.
# Completeness is judged on the whole token rather than a bare "]", because a
# payload can end a line with a "]" of its own.
FRAME_TAIL_RE = re.compile(r"\s+([01])\]\Z")


def is_frame_closed(line: str) -> bool:
    return FRAME_TAIL_RE.search(line) is not None


def join_frame_lines(pieces: list[str]) -> str:
    """Rejoin a frame the firmware split by printing a newline into its
    payload. Newlines inside the payload survive; the one abutting the closing
    token is a line terminator, not payload, so it collapses into that token's
    separating space."""
    return FRAME_TAIL_RE.sub(r" \1]", "\n".join(pieces))


@dataclass
class ResponseFrame:
    path: str
    timestamp_ns: int
    sequence: int
    response: str
    success: bool

    @classmethod
    def parse(cls, line: str) -> "ResponseFrame":
        body = line.strip()
        if not (body.startswith("[") and body.endswith("]")):
            raise ValueError(f"not a framed response: {line!r}")
        tokens = body[1:-1].split(" ")
        if len(tokens) < 4:
            raise ValueError(f"short frame: {line!r}")
        return cls(
            path=tokens[0],
            timestamp_ns=int(tokens[1], 16),
            sequence=int(tokens[2]),
            response=" ".join(tokens[3:-1]),
            success=tokens[-1] == "1",
        )

    def response_bytes(self) -> bytes:
        return bytes(int(t, 16) for t in self.response.split() if t)
