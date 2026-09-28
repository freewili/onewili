"""OneWili transport for the CM0 FPGA mailbox (fwcm0 api)."""
from __future__ import annotations

from collections import deque
import queue
import subprocess
import threading
import time

from onewili import framing
from onewili.transport import DEFAULT_TIMEOUT, RESET_QUIET, FrameRouter


class Cm0Transport:
    """A command connection to MAIN through the CM0 bridge.

    Opening probes MAIN with a read-only Device State command. A running Linux
    process alone is not evidence that the FPGA/mailbox session is available.
    """

    framed_files = True
    # onewili.streams batch size: a mailbox reply holds about 4 KB, and MAIN
    # runs one CM0 command per loop pass, so fetch 1 KB of datagrams per poll
    # (the C package's OW_STREAM_STASH).
    stream_poll_bytes = 1024

    def __init__(self, fwcm0: str = "fwcm0", timeout: float = 3.0) -> None:
        self._argv = [fwcm0, "api"]
        self._timeout = timeout
        self._proc = None
        self._reader = None
        self._running = False
        self._ended = threading.Event()
        self._diagnostics = deque(maxlen=8)
        self.frames: "queue.Queue[framing.ResponseFrame]" = queue.Queue()
        self.events: "queue.Queue[framing.ResponseFrame]" = queue.Queue()
        self.lines: "queue.Queue[str]" = queue.Queue()
        self._router = FrameRouter(self.frames, self.events, self.lines)

    def open(self) -> None:
        if self._proc is not None:
            if self._proc.poll() is None and not self._ended.is_set():
                return
            self.close()
        self.flush_queues()
        self._router = FrameRouter(self.frames, self.events, self.lines)
        self._diagnostics.clear()
        self._ended.clear()
        try:
            self._proc = subprocess.Popen(
                self._argv, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT, bufsize=0)
            self._running = True
            self._reader = threading.Thread(target=self._read_loop,
                                            args=(self._proc,), daemon=True)
            self._reader.start()
            self.send("h\\a\\g")
            deadline = time.monotonic() + self._timeout
            while time.monotonic() < deadline:
                frame = self.wait_frame(max(0.0, deadline - time.monotonic()))
                if frame is None:
                    break
                if frame.path == "h\\a\\g":
                    if not frame.success:
                        raise RuntimeError("CM0: MAIN refused the connection probe: " + frame.response)
                    return
            raise RuntimeError(self._error("MAIN did not answer the connection probe"))
        except Exception:
            self.close()
            raise

    def close(self) -> None:
        self._running = False
        proc = self._proc
        if proc is not None:
            try:
                proc.stdin.close()
            except OSError:
                pass
            # EOF lets the CLI wait for the daemon to release this session.
            try:
                proc.wait(timeout=1.0)
            except subprocess.TimeoutExpired:
                proc.terminate()
                try:
                    proc.wait(timeout=0.5)
                except subprocess.TimeoutExpired:
                    proc.kill()
                    proc.wait(timeout=1.0)
        if self._reader is not None:
            self._reader.join(timeout=1.0)
            self._reader = None
        if proc is not None:
            proc.stdout.close()
        self._proc = None
        self._ended.set()

    def _error(self, message: str) -> str:
        detail = "; ".join(self._diagnostics)
        return "CM0: " + message + (": " + detail if detail else "")

    def send(self, command: str) -> None:
        proc = self._proc
        if proc is None or proc.poll() is not None or self._ended.is_set():
            raise RuntimeError(self._error("bridge connection is closed"))
        try:
            proc.stdin.write(RESET_QUIET + command.encode("ascii") + b"\n")
            proc.stdin.flush()
        except (OSError, ValueError) as exc:
            raise RuntimeError(self._error("cannot write to bridge")) from exc

    def wait_frame(self, timeout: float = DEFAULT_TIMEOUT):
        deadline = time.monotonic() + timeout
        while True:
            try:
                return self.frames.get(timeout=max(0.0, min(0.05, deadline - time.monotonic())))
            except queue.Empty:
                if self._ended.is_set():
                    raise RuntimeError(self._error("bridge closed before MAIN replied"))
                if time.monotonic() >= deadline:
                    return None

    def flush_queues(self) -> None:
        for q in (self.frames, self.events, self.lines):
            while True:
                try:
                    q.get_nowait()
                except queue.Empty:
                    break

    def _read_loop(self, proc) -> None:
        try:
            for raw in iter(proc.stdout.readline, b""):
                if not self._running:
                    break
                line = raw.decode("utf-8", errors="replace").rstrip("\r\n")
                if line or self._router.pending is not None:
                    self._router.route(line)
                    if line and not line.startswith("["):
                        self._diagnostics.append(line)
        finally:
            self._ended.set()


def connect_cm0(fwcm0: str = "fwcm0", timeout: float = 3.0):
    """Connect to MAIN, raising promptly if the bridge or mailbox is unavailable."""
    from onewili import OneWili
    return OneWili(transport=Cm0Transport(fwcm0, timeout)).open()
