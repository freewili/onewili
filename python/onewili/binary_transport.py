"""FTDI binary-stream transport: reader thread decoding WILI frames into a queue."""
from __future__ import annotations

import queue
import threading
import struct

from . import binary_framing

BINARY_BAUD = 1_000_000


class BinaryTransport:
    """Bounded event stream. Unknown/malformed events are delivered as RawFrame.

    Set raw=True to receive every frame unchanged. Queue overflow drops the
    oldest event and increments dropped_events. last_error records disconnects.
    A negative decoder size denotes a minimum prefix size (variable payload).
    """

    def __init__(self, port_name: str, decoders: dict, *, raw=False, queue_size=256) -> None:
        if queue_size < 1:
            raise ValueError("queue_size must be positive")
        self.port_name = port_name
        self.decoders = decoders
        self.raw = raw
        self.events: queue.Queue = queue.Queue(maxsize=queue_size)
        self.unknown_frames = 0
        self.size_mismatches = 0
        self.dropped_events = 0
        self.last_error = None
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
        self.last_error = None
        while not self.events.empty():
            try:
                self.events.get_nowait()
            except queue.Empty:
                break
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
                            chunk += self._serial.read(min(waiting, 65536))
                except Exception as exc:
                    if self._running:
                        self.last_error = exc
                    break
                if not chunk:
                    continue
                self.feed(chunk)
        finally:
            self._running = False

    def feed(self, chunk: bytes) -> None:
        """Feed received bytes (also useful with custom transports)."""
        for frame in self._parser.feed(chunk):
            event = frame
            dec = self.decoders.get(frame.header_type)
            if dec is None:
                self.unknown_frames += 1
            elif not self.raw:
                size = dec[1]
                try:
                    if (size >= 0 and len(frame.payload) != size) or len(frame.payload) < abs(size):
                        raise ValueError("invalid payload size")
                    event = dec[2](frame.payload, frame.error)
                except (ValueError, struct.error):
                    self.size_mismatches += 1
            try:
                self.events.put_nowait(event)
            except queue.Full:
                try:
                    self.events.get_nowait()
                    self.dropped_events += 1
                except queue.Empty:
                    pass
                self.events.put_nowait(event)
