"""WILI binary frame parsing for the FreeWili FTDI (binary API) stream.

Every frame is a 12-byte header followed by a typed payload (see the firmware's
Fw2BinaryAPI.h). Buffer-based resync: garbage before a marker is skipped; a
frame split across reads reassembles on the next feed().
"""
from __future__ import annotations

import struct
from dataclasses import dataclass

MARKER_BYTES = b"WILI"
HEADER_FMT = "<IHHI"          # marker, repeat_count, header_type, errorbit+length
HEADER_SIZE = 12
MAX_PAYLOAD = 1 << 20         # sanity cap; larger claimed lengths force a resync


@dataclass
class RawFrame:
    header_type: int
    repeat_count: int
    payload: bytes
    error: bool


class Parser:
    """Feed bytes, get complete RawFrames. State survives feed() calls."""

    def __init__(self) -> None:
        self.reset()

    def reset(self) -> None:
        self._buf = b""

    def feed(self, data: bytes) -> "list[RawFrame]":
        self._buf += data
        frames: list[RawFrame] = []
        while True:
            i = self._buf.find(MARKER_BYTES)
            if i < 0:
                # a marker may straddle the read boundary - keep the tail
                self._buf = self._buf[-3:] if len(self._buf) > 3 else self._buf
                return frames
            if i:
                self._buf = self._buf[i:]
            if len(self._buf) < HEADER_SIZE:
                return frames
            _marker, repeat, htype, err_len = struct.unpack(
                HEADER_FMT, self._buf[:HEADER_SIZE])
            length = err_len & 0x7FFFFFFF
            if length > MAX_PAYLOAD:
                self._buf = self._buf[1:]     # bad header: drop a byte, resync
                continue
            if len(self._buf) < HEADER_SIZE + length:
                return frames
            frames.append(RawFrame(
                header_type=htype,
                repeat_count=repeat,
                payload=self._buf[HEADER_SIZE:HEADER_SIZE + length],
                error=bool(err_len & 0x80000000),
            ))
            self._buf = self._buf[HEADER_SIZE + length:]
