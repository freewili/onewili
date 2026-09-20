"""FTDI binary-stream transport: reader thread decoding WILI frames into a queue."""
from __future__ import annotations

import queue
import threading

from . import binary_framing

BINARY_BAUD = 1_000_000   # hardware unknown: adjust here if the FTDI port differs


class BinaryTransport:
    """decoders: dict[int, tuple[name, payload_size, decode(payload, error)]]."""

    def __init__(self, port_name: str, decoders: dict) -> None:
        self.port_name = port_name
        self.decoders = decoders
        self.events: queue.Queue = queue.Queue()
        self.unknown_frames = 0
        self.size_mismatches = 0
        self._serial = None
        self._reader: "threading.Thread | None" = None
        self._running = False
        self._parser = binary_framing.Parser()

    def open(self) -> None:
        if self._serial is not None and self._running:
            return
        if self._serial is not None:
            self.close()   # reader died (e.g. device unplugged) - reset first
        import serial  # lazy, like the text transport

        self._serial = serial.Serial(self.port_name, BINARY_BAUD, timeout=0.1)
        self._parser.reset()
        self._running = True
        self._reader = threading.Thread(target=self._read_loop, daemon=True)
        self._reader.start()

    def close(self) -> None:
        self._running = False
        if self._reader is not None:
            self._reader.join(timeout=1.0)
            self._reader = None
        if self._serial is not None:
            self._serial.close()
            self._serial = None

    def _read_loop(self) -> None:
        try:
            while self._running:
                try:
                    # see Transport._read_loop: one byte, then whatever is waiting
                    chunk = self._serial.read(1)
                    if chunk:
                        waiting = self._serial.in_waiting
                        if waiting:
                            chunk += self._serial.read(waiting)
                except Exception:
                    break
                if not chunk:
                    continue
                for frame in self._parser.feed(chunk):
                    dec = self.decoders.get(frame.header_type)
                    if dec is None:
                        self.unknown_frames += 1
                        continue
                    if len(frame.payload) != dec[1]:
                        self.size_mismatches += 1
                        continue
                    self.events.put(dec[2](frame.payload, frame.error))
        finally:
            self._running = False
