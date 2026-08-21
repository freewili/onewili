"""OneWili transport for the CM0 (Raspberry Pi) side of the FreeWili 2.

The CM0 reaches MAIN over the FPGA mailbox, not a serial port, so this transport
pipes OneWili wire bytes through the `fwcm0 console` CLI (the transparent mailbox
console) instead of pyserial. Drop-in for onewili.transport.Transport.

    from onewili_cm0 import connect_cm0
    dev = connect_cm0()
    dev.io.gpio.set_io_high(25)
    dev.close()
"""
from __future__ import annotations

import queue
import subprocess
import threading
import time

from onewili import framing
from onewili.transport import DEFAULT_TIMEOUT, RESET_QUIET, FrameRouter


class Cm0Transport:
    """Pipes OneWili wire bytes through `fwcm0 console` (the FPGA mailbox link)."""

    def __init__(self, fwcm0: str = "fwcm0") -> None:
        self._argv = ["stdbuf", "-oL", fwcm0, "console"]
        self._proc = None
        self._reader = None
        self._running = False
        self.frames: "queue.Queue[framing.ResponseFrame]" = queue.Queue()
        self.events: "queue.Queue[framing.ResponseFrame]" = queue.Queue()
        self.lines: "queue.Queue[str]" = queue.Queue()
        self._router = FrameRouter(self.frames, self.events, self.lines)

    def open(self) -> None:
        if self._proc is not None:
            return
        self._proc = subprocess.Popen(
            self._argv, stdin=subprocess.PIPE, stdout=subprocess.PIPE, bufsize=0)
        self._running = True
        self._reader = threading.Thread(target=self._read_loop, daemon=True)
        self._reader.start()
        self._write(RESET_QUIET + b"\n")
        # `fwcm0 console` runs a HELLO handshake with MAIN before it forwards our
        # bytes; wait until MAIN starts streaming (its menu dump lands) so the
        # first command is not sent before quiet mode is active, then discard the
        # startup noise.
        deadline = time.monotonic() + 3.0
        while (time.monotonic() < deadline
               and self.lines.empty() and self.frames.empty()):
            time.sleep(0.05)
        time.sleep(0.3)
        self.flush_queues()

    def close(self) -> None:
        self._running = False
        if self._proc is not None:
            self._write(RESET_QUIET)
            try:
                self._proc.stdin.close()
            except Exception:
                pass
            self._proc.terminate()
            try:
                self._proc.wait(timeout=1.0)
            except Exception:
                self._proc.kill()
            self._proc = None
        if self._reader is not None:
            self._reader.join(timeout=1.0)
            self._reader = None

    def send(self, command: str) -> None:
        if self._proc is None:
            raise RuntimeError("transport is not open")
        self._write(RESET_QUIET + command.encode("ascii") + b"\n")

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

    def _write(self, data: bytes) -> None:
        try:
            self._proc.stdin.write(data)
            self._proc.stdin.flush()
        except Exception:
            pass

    def _read_loop(self) -> None:
        for raw in iter(self._proc.stdout.readline, b""):
            if not self._running:
                break
            line = raw.decode("utf-8", errors="replace").rstrip("\r\n")
            # FrameRouter rejoins a frame the firmware split across lines by
            # printing a newline into its payload, so a blank line matters while
            # one is being accumulated. readline() blocks, so a frame that never
            # closes is given up on when the next line lands rather than on a
            # timer of its own.
            if line or self._router.pending is not None:
                self._router.route(line)


def connect_cm0(fwcm0: str = "fwcm0"):
    """Open a OneWili device that talks to MAIN over the CM0 FPGA mailbox."""
    from onewili import OneWili
    return OneWili(transport=Cm0Transport(fwcm0)).open()
