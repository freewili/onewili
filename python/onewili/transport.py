"""Serial transport: background reader thread routing lines into queues."""
from __future__ import annotations

import queue
import threading
import time

from . import framing

BAUD_RATE = 1_000_000
DEFAULT_TIMEOUT = 5.0
# Ctrl-B resets menu navigation to the root AND enters quiet mode (frames are
# still printed). Prefixed to every command so each call starts from a known
# state. (Ctrl-C, 0x03, would reset too but re-enables verbose menu echo.)
RESET_QUIET = b"\x02"
# A handler that prints a newline into its response splits the frame across
# physical lines, so a frame still missing its closing token is held until the
# rest of it arrives. These bound that wait: an unclosed "[" must not
# accumulate forever, and must not stall the lines queue behind it.
MAX_PENDING_FRAME_CHARS = 4096
PENDING_FRAME_TIMEOUT = 1.0


def _parse_or_none(line: str):
    try:
        return framing.ResponseFrame.parse(line)
    except ValueError:
        return None


class FrameRouter:
    """Routes console lines into the frames/events/lines queues, rejoining a
    frame the firmware split by printing a newline into its payload.

    At most one frame is held at a time, response or event. A continuation line
    carries no identity of its own, so with two open at once there would be no
    way to decide which one it belongs to. A *complete* event still passes
    straight through while a response is accumulating, which is the case that
    actually arises since event payloads are usually a single line.
    """

    def __init__(self, frames, events, lines) -> None:
        self.frames = frames
        self.events = events
        self.lines = lines
        self.pending: list[str] | None = None
        self.pending_is_event = False
        self._chars = 0
        self._deadline = 0.0

    def route(self, line: str) -> None:
        self.expire()
        is_event = framing.EVENT_RE.match(line) is not None
        is_frame = framing.FRAME_RE.match(line) is not None
        if self.pending is not None and not is_event and not is_frame:
            self._continue(line)
            return
        if is_event or is_frame:
            if framing.is_frame_closed(line):
                frame = _parse_or_none(line)
                if frame is not None:
                    self._deliver(frame, is_event)
                    return
            else:
                self.abandon()
                self.pending = [line]
                self.pending_is_event = is_event
                self._chars = len(line)
                self._deadline = time.monotonic() + PENDING_FRAME_TIMEOUT
                return
        self.lines.put(line)

    def expire(self) -> None:
        if self.pending is not None and time.monotonic() >= self._deadline:
            self.abandon()

    def abandon(self) -> None:
        """Give up on a half-received frame and hand its lines to the raw
        queue, which is where output that is not a frame goes anyway."""
        if self.pending is None:
            return
        pieces, self.pending = self.pending, None
        for piece in pieces:
            self.lines.put(piece)

    def _continue(self, line: str) -> None:
        self.pending.append(line)
        self._chars += len(line) + 1
        if framing.is_frame_closed(line):
            joined = framing.join_frame_lines(self.pending)
            frame = _parse_or_none(joined)
            if frame is not None:
                is_event = self.pending_is_event
                self.pending = None
                self._deliver(frame, is_event)
                return
            self.abandon()
        elif self._chars > MAX_PENDING_FRAME_CHARS:
            self.abandon()

    def _deliver(self, frame, is_event: bool) -> None:
        if is_event:
            self.events.put(frame)
            return
        self.abandon()  # a held frame can no longer close
        self.frames.put(frame)


class Transport:
    def __init__(self, port_name: str) -> None:
        self.port_name = port_name
        self._serial = None
        self._reader: threading.Thread | None = None
        self._running = False
        self.frames: "queue.Queue[framing.ResponseFrame]" = queue.Queue()
        self.events: "queue.Queue[framing.ResponseFrame]" = queue.Queue()
        self.lines: "queue.Queue[str]" = queue.Queue()
        self._buf = b""
        self._router = FrameRouter(self.frames, self.events, self.lines)
        # See pause_reader(): lets a caller (file transfers) take the serial
        # port over from the background thread for a raw, byte-exact
        # exchange.
        self._pause_requested = threading.Event()
        self._paused_ack = threading.Event()

    def open(self) -> None:
        if self._serial is not None:
            return
        import serial  # lazy: package imports fine without pyserial installed

        self._serial = serial.Serial(self.port_name, BAUD_RATE, timeout=0.1)
        self._running = True
        self._reader = threading.Thread(target=self._read_loop, daemon=True)
        self._reader.start()
        self._serial.write(RESET_QUIET + b"\n")

    def close(self) -> None:
        self._running = False
        self._pause_requested.clear()  # let a stuck pause spin-loop exit
        if self._reader is not None:
            self._reader.join(timeout=1.0)
            self._reader = None
        if self._serial is not None:
            try:
                self._serial.write(RESET_QUIET)
            except Exception:
                pass
            self._serial.close()
            self._serial = None

    def send(self, command: str) -> None:
        if self._serial is None:
            raise RuntimeError("transport is not open")
        self._serial.write(RESET_QUIET + command.encode("ascii") + b"\n")

    def wait_frame(self, timeout: float = DEFAULT_TIMEOUT):
        try:
            return self.frames.get(timeout=timeout)
        except queue.Empty:
            return None

    def flush_queues(self) -> None:
        for q in (self.frames, self.events, self.lines):
            while True:
                try:
                    q.get_nowait()
                except queue.Empty:
                    break

    def pause_reader(self, timeout: float = 1.0) -> None:
        """Stop the background reader thread from touching the serial port so
        a caller can read/write it directly and byte-exactly. Used by file
        transfers: _read_loop below decodes everything as UTF-8 text split on
        '\\n', which is correct for menu responses but would corrupt an
        arbitrary binary payload (embedded '\\n' bytes split it early, and
        non-UTF-8 bytes are lost to errors="replace"). Call resume_reader()
        (ideally via try/finally) once the raw exchange is done.
        """
        if self._serial is None:
            raise RuntimeError("transport is not open")
        self._paused_ack.clear()
        self._pause_requested.set()
        if not self._paused_ack.wait(timeout):
            self._pause_requested.clear()
            raise RuntimeError("reader thread did not pause in time")

    def resume_reader(self) -> None:
        self._pause_requested.clear()

    @property
    def raw_serial(self):
        """The underlying pyserial handle. Only safe to touch between a
        pause_reader() and its matching resume_reader() -- otherwise the
        background thread is reading it concurrently."""
        if self._serial is None:
            raise RuntimeError("transport is not open")
        return self._serial

    def _read_loop(self) -> None:
        while self._running:
            if self._pause_requested.is_set():
                # Any bytes already read into `_buf` but not yet split into a
                # complete line are dropped here, not handed to the paused
                # caller. Unreachable under the intended call pattern --
                # pause_reader() is always called before any write, so there
                # is nothing meaningful in flight yet to lose -- but a caller
                # that paused mid-conversation (outside that pattern) could
                # lose a partial line this way.
                self._buf = b""  # a paused caller owns the wire now; drop the rest
                self._router.abandon()
                self._paused_ack.set()
                while self._pause_requested.is_set() and self._running:
                    time.sleep(0.01)
                continue
            try:
                chunk = self._serial.read(4096)
            except Exception:
                break
            if chunk:
                self._buf += chunk
                while b"\n" in self._buf:
                    raw, self._buf = self._buf.split(b"\n", 1)
                    line = raw.decode("utf-8", errors="replace").rstrip("\r")
                    # Blank lines are noise on their own, but a payload can
                    # contain one (a handler printing "\n\n"), so keep them
                    # while a frame is being accumulated.
                    if line or self._router.pending is not None:
                        self._router.route(line)
            self._router.expire()
