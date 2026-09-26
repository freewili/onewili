r"""File transfer and directory listing over the FreeWili filesystem menu.

This is a port of core/runtime_c/ow_files.h, not a binding -- this package
has no C dependency -- so keep the two diffable rather than clever.

Ground truth is firmware MenuX/fwMenuFileSystem.cpp, by way of
fwMenuX::callSubFunction (rmpLib/fwMenuX.cpp), which opens a normal menu
RESPONSE FRAME before calling the handler and closes it right after (neither
handler defers m_bFinishRepsonse). So the handshake text below is each the
BODY of a framed response, not a bare line on its own -- e.g. the real put
handshake line reads "[h\\x\\f <hexTimestampNs> <seq> Send File Now 1]", not
just "Send File Now". The handshake frame's tag is the full navigation
prefix to that menu ("h\\x\\f" for put, "h\\x\\u" for get) -- a DIFFERENT tag
from the later completion frame ("x\\f"/"x\\u" below), which comes from a
direct printMenuResponse call with a fixed literal, not through
callSubFunction. So handshake matching below is tag-tolerant (body content
only); completion matching pins to the exact literal tag, since that one
really is fixed by the firmware source. Both handshake lines are emitted
with printOutAlways, so quiet mode never suppresses them.

PUT (getFileFromPC / doDownload):
    host   -> "h\nx\nf\n<path> <size> <crc>\n"
    device -> framed body "Send File Now" (proceed) or "Invalid" (do NOT
              stream a byte)
    host   -> payload bytes
    device -> framed "x\f" trailer: "success <N> bytes" (ok) or
              "Failed checksum" (ok=0 -- the device deletes the file)

GET (sendFileToPC / doUpload):
    host   -> "h\nx\nu\n<path> \n"   ('/' -> '\\', trailing space kept)
    device -> framed body "RxFile <size>" (size only, no crc yet) or
              "Invalid" / "CantOpenFile"
    device -> payload bytes
    device -> framed "x\u" trailer: "success <N> bytes <CRC> crc" -- the
              crc arrives HERE, after the payload, not in the handshake

LIST: send "h\nx\nl\n<path>\n", then collect "*fdir" event frames until an
"end" record or silence (a "fdir" event's frame.response is already the
payload text -- "dir <name> <size>" / "fil <name> <size>" / "end <count>" --
with framing.py having stripped the timestamp/sequence/ok wrapper).

The background reader thread (transport.Transport) decodes every incoming
byte as UTF-8 text split on '\n' -- correct for ordinary menu responses, but
it would corrupt an arbitrary binary payload (embedded '\n' bytes split it
early; non-UTF-8 bytes are unrecoverably replaced). So put/get/list pause
that thread and talk to the serial port directly and byte-exactly for the
whole exchange, exactly like the C reference. sd_host_select is the
exception: it has no payload to protect, so it is just an ordinary framed
menu command and goes through the generated binding (mirrors
ow_sd_host_select in the C runtime, which calls
ow_hardware_file_system_set_sd_card_host instead of touching the wire
itself).
"""
from __future__ import annotations

import os
import time
import zlib
from dataclasses import dataclass
from pathlib import Path

from . import framing

_CHUNK = 64          # payload bytes per write -- matches fwSerial / ow_files.h
_IDLE_S = 1.0        # silence that ends a read phase (ow_files.h: 2 x 500ms)
_RESET = b"\x03"     # Ctrl-C: full reset incl. verbose echo -- NOT RESET_QUIET


@dataclass
class DirEntry:
    name: str
    is_dir: bool
    size: int


class _RawIO:
    """Byte-buffered reader/writer against a paused Transport's raw serial
    handle. Ports ow_files_rx (ow_files.h) closely enough to diff by eye;
    silence is detected by elapsed wall time rather than counting empty
    reads, since pyserial's own read() already blocks up to the port's fixed
    per-call timeout.
    """

    def __init__(self, ser) -> None:
        self._ser = ser
        self._buf = b""
        self._pos = 0

    def write(self, data: bytes) -> None:
        self._ser.write(data)

    def _fill(self) -> bool:
        """Refill when drained. True = bytes available, False = silence."""
        if self._pos < len(self._buf):
            return True
        deadline = time.monotonic() + _IDLE_S
        while time.monotonic() < deadline:
            chunk = self._ser.read(4096)
            if chunk:
                self._buf = chunk
                self._pos = 0
                return True
        return False

    def read_line(self) -> "str | None":
        """One newline-terminated line ('\\r' stripped, terminator dropped),
        or None on silence (mirrors ow_files_read_line)."""
        out = bytearray()
        while True:
            if not self._fill():
                if not out:
                    return None
                break
            b = self._buf[self._pos]
            self._pos += 1
            if b == 0x0A:
                break
            if b != 0x0D:
                out.append(b)
        return out.decode("utf-8", errors="replace")

    def read_payload(self, n: int, progress=None) -> bytes:
        """Exactly n raw payload bytes. Calls progress(done, n) as chunks
        arrive and once more when done (even for n == 0), mirroring
        ow_files_get's per-chunk-plus-final callback. Raises RuntimeError on
        silence before n bytes have arrived.
        """
        out = bytearray()
        while len(out) < n:
            if not self._fill():
                raise RuntimeError(
                    f"timeout waiting for {n - len(out)} more payload byte(s)")
            take = min(n - len(out), len(self._buf) - self._pos)
            out += self._buf[self._pos:self._pos + take]
            self._pos += take
            if progress is not None:
                progress(len(out), n)
        if progress is not None:
            progress(len(out), n)
        return bytes(out)


def _build_put_header(dev_path: str, size: int, crc32: int) -> str:
    return f"h\nx\nf\n{dev_path} {size} {crc32}\n"


def _build_get_header(dev_path: str) -> str:
    # The device wants backslash-separated paths on this command, and the
    # proven implementation emits a space before the newline.
    return "h\nx\nu\n" + dev_path.replace("/", "\\") + " \n"


def _parse_frame(line: str, tag: str) -> "framing.ResponseFrame | None":
    """`line` as a framed <tag> response, or None if it isn't one (including
    event frames such as fdir, whose path is "*fdir", never a plain tag).
    Used for the COMPLETION frames (put's "x\\f" trailer, get's "x\\u"
    trailer). Older firmware printed those with the bare literal tag; current
    firmware prints them through the menu's navigation prefix (that tag
    preceded by "h" and a backslash), so the tag is matched exactly OR as
    the last path segment, which still lets an unrelated framed response be
    skipped rather than misread as this transfer's result."""
    try:
        frame = framing.ResponseFrame.parse(line)
    except ValueError:
        return None
    # The firmware moved from a bare literal ("x\\f") to the navigation-
    # prefixed form ("h\\x\\f") for these trailers; accept either spelling.
    if frame.path == tag or frame.path.endswith("\\" + tag):
        return frame
    return None


def _handshake_body(line: str) -> "str | None":
    """The response text of any framed line, regardless of tag, or None if
    `line` isn't a framed response at all (including event frames). Used for
    the HANDSHAKE frames (put's "Send File Now"/"Invalid", get's
    "RxFile <size>"/"Invalid"/"CantOpenFile"): those are opened by
    fwMenuX::callSubFunction with the full navigation prefix as the tag
    (e.g. "h\\x\\f"), a DIFFERENT tag from the fixed one the completion
    frame uses, so matching here is deliberately tag-blind -- only the body
    content is checked."""
    try:
        frame = framing.ResponseFrame.parse(line)
    except ValueError:
        return None
    return frame.response


_END = object()  # sentinel: list()'s "end <count>" record


def _parse_fdir_entry(payload: str):
    """One fdir payload -- "dir <name> <size>" / "fil <name> <size>" /
    "end <count>" -- already stripped of its timestamp/sequence/ok wrapper by
    framing.ResponseFrame.parse. Returns a DirEntry, the _END sentinel for an
    "end" record, or None for anything else (unrecognised: keep listening).

    Mirrors ow_files_parse_fdir: the size is the payload's LAST token, taken
    only when it is all-digits, so names may contain spaces. Firmware from
    before the listing fix omits the size field entirely, which is where
    ow_files.h documents an unavoidable, wire-level ambiguity for legacy
    multi-word names ending in a numeric word; see that file for the full
    explanation. Current firmware always sends the size field.
    """
    if payload == "end" or payload.startswith("end "):
        return _END
    for prefix, is_dir in (("dir ", True), ("fil ", False)):
        if not payload.startswith(prefix):
            continue
        rest = payload[len(prefix):].strip()
        if not rest:
            return None
        name, _, last = rest.rpartition(" ")
        # ASCII-only: str.isdigit() also accepts non-ASCII digits (e.g.
        # superscript '²') that int() then rejects, raising an unhandled
        # ValueError out of list(). C/Rust are ASCII-only here too.
        if name and last.isascii() and last.isdigit():
            return DirEntry(name=name, is_dir=is_dir, size=int(last))
        return DirEntry(name=rest, is_dir=is_dir, size=0)
    return None


class Files:
    """File transfer and directory listing over the FreeWili filesystem menu."""

    def __init__(self, device) -> None:
        self._device = device

    @property
    def framed(self):
        """CRC-checked transfers through regular menu commands (USB or CM0)."""
        from .framed_files import FramedFiles
        return FramedFiles(self._device)

    def put(self, dev_path: str, data: bytes, progress=None) -> None:
        """Upload `data` to `dev_path` on the device. progress(done, total),
        if given, is called after every chunk written."""
        transport = self._device._transport
        if getattr(transport, "framed_files", False):
            return self.framed.put(dev_path, data, progress)
        payload = bytes(data)
        crc = zlib.crc32(payload) & 0xFFFFFFFF
        header = _build_put_header(dev_path, len(payload), crc)

        transport.flush_queues()
        transport.pause_reader()
        try:
            raw = _RawIO(transport.raw_serial)
            raw.write(_RESET + b"\n")
            raw.write(header.encode("utf-8"))

            # Framed handshake (tag-tolerant, body-matched -- see the
            # module docstring): "Send File Now" clears us to stream;
            # "Invalid" (a rejected path/size/crc line) means not one payload
            # byte should go out.
            while True:
                line = raw.read_line()
                if line is None:
                    raise RuntimeError(f"put {dev_path!r}: timeout waiting for handshake")
                body = _handshake_body(line)
                if body is None:
                    continue  # not a framed response: event, chatter, etc.
                if body == "Send File Now":
                    break
                if body == "Invalid":
                    raise RuntimeError(f"put {dev_path!r}: device rejected the request (Invalid)")
                # some other framed response: unrelated, keep waiting

            sent = 0
            total = len(payload)
            while sent < total:
                chunk = payload[sent:sent + _CHUNK]
                raw.write(chunk)
                sent += len(chunk)
                if progress is not None:
                    progress(sent, total)

            # The device verifies its own CRC after the payload lands and
            # reports the result as a framed x\f response: success on
            # "success <N> bytes", failure (and file deletion) on
            # "Failed checksum".
            while True:
                line = raw.read_line()
                if line is None:
                    raise RuntimeError(f"put {dev_path!r}: timeout waiting for completion")
                frame = _parse_frame(line, "x\\f")
                if frame is None:
                    continue
                if not frame.success:
                    raise RuntimeError(f"put {dev_path!r}: device reported failure: {frame.response}")
                return
        finally:
            transport.resume_reader()

    def get(self, dev_path: str, progress=None) -> bytes:
        """Download `dev_path` from the device. progress(done, total), if
        given, is called as payload bytes arrive."""
        transport = self._device._transport
        if getattr(transport, "framed_files", False):
            return self.framed.get(dev_path, progress)
        header = _build_get_header(dev_path)

        transport.flush_queues()
        transport.pause_reader()
        try:
            raw = _RawIO(transport.raw_serial)
            raw.write(_RESET + b"\n")
            raw.write(header.encode("utf-8"))

            # Framed handshake (tag-tolerant, body-matched -- see the
            # module docstring): "RxFile <size>" clears us to read the
            # payload -- size ONLY, no crc yet; the crc arrives in the
            # trailer, AFTER the payload. "Invalid" (bad path syntax) or
            # "CantOpenFile" (no such file) both mean there is no payload.
            size = None
            while True:
                line = raw.read_line()
                if line is None:
                    raise RuntimeError(f"get {dev_path!r}: timeout waiting for handshake")
                body = _handshake_body(line)
                if body is None:
                    continue  # not a framed response: event, chatter, etc.
                if body in ("Invalid", "CantOpenFile"):
                    raise RuntimeError(f"get {dev_path!r}: device reported {body}")
                if body.startswith("RxFile "):
                    text = body[len("RxFile "):].strip()
                    # ASCII-only: str.isdigit() also accepts non-ASCII digits
                    # (e.g. superscript '²') that int() then rejects,
                    # which would raise an unhandled ValueError instead of
                    # this clean RuntimeError. C/Rust are ASCII-only here too.
                    if not (text.isascii() and text.isdigit()):
                        raise RuntimeError(f"get {dev_path!r}: unparsable handshake {body!r}")
                    size = int(text)
                    break
                # some other framed response: unrelated, keep waiting

            payload = raw.read_payload(size, progress)

            # The crc lives ONLY in this trailer, a framed x\u response with
            # body "success <N> bytes <CRC> crc". No trailer, or one that
            # doesn't parse, means there is no crc to trust.
            while True:
                line = raw.read_line()
                if line is None:
                    raise RuntimeError(f"get {dev_path!r}: timeout waiting for the crc trailer")
                frame = _parse_frame(line, "x\\u")
                if frame is None:
                    continue
                tokens = frame.response.split()
                if len(tokens) < 2 or tokens[-1] != "crc" or not tokens[-2].isdigit():
                    raise RuntimeError(f"get {dev_path!r}: trailer missing a crc: {frame.response!r}")
                crc = int(tokens[-2])
                if zlib.crc32(payload) & 0xFFFFFFFF != crc:
                    raise RuntimeError(f"get {dev_path!r}: crc mismatch")
                return payload
        finally:
            transport.resume_reader()

    def list(self, path: str = "") -> "list[DirEntry]":
        """Entries of `path` ("" for the current directory)."""
        transport = self._device._transport
        cmd = f"h\nx\nl\n{path}\n"

        transport.flush_queues()
        transport.pause_reader()
        try:
            raw = _RawIO(transport.raw_serial)
            raw.write(_RESET + b"\n")
            raw.write(cmd.encode("utf-8"))

            entries: "list[DirEntry]" = []
            while True:
                line = raw.read_line()
                if line is None:
                    break  # silence: legacy firmware, no end record
                frame = _parse_frame(line, "*fdir")
                if frame is None:
                    continue  # the command's own response, unrelated chatter, etc.
                entry = _parse_fdir_entry(frame.response)
                if entry is None:
                    continue
                if entry is _END:
                    break
                entries.append(entry)
            return entries
        finally:
            transport.resume_reader()

    def sd_host_select(self, to_pc: bool) -> None:
        """Connect the SD card to the USB reader / PC (True) or the main CPU
        (False). No payload to protect from the text decoder, so -- like
        ow_sd_host_select in the C runtime -- this is just an ordinary framed
        menu command routed through the generated binding, not a raw
        exchange.
        """
        result = self._device.hardware.file_system.set_sd_card_host(1 if to_pc else 0)
        if result.is_err():
            raise RuntimeError(f"sd_host_select: {result.unwrap_err()}")

    # -- Host convenience, mirroring the C OW_NO_STDIO split. --------------

    def put_file(self, host_path: "str | os.PathLike[str]", dev_path: str, progress=None) -> None:
        """Upload the contents of the host file at `host_path` to `dev_path`."""
        if getattr(self._device._transport, "framed_files", False):
            return self.framed.put_file(host_path, dev_path, progress)
        data = Path(host_path).read_bytes()
        self.put(dev_path, data, progress)

    def get_file(self, dev_path: str, host_path: "str | os.PathLike[str]", progress=None) -> None:
        """Download `dev_path` and write it to the host file at `host_path`."""
        if getattr(self._device._transport, "framed_files", False):
            return self.framed.get_file(dev_path, host_path, progress)
        data = self.get(dev_path, progress)
        Path(host_path).write_bytes(data)
