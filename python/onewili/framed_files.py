"""Bounded, CRC-checked SD transfers over normal OneWili menu commands."""
from __future__ import annotations
import io
import os
from pathlib import Path
import secrets
import tempfile
import zlib


class FramedFiles:
    """Device SD paths are absolute (for example /apps/demo.py).

    Works over USB menu transport and the CM0 mailbox. One file transaction
    owns the file channel; other menu commands can run between chunks.
    Transfers are never retried automatically after ambiguous timeouts.
    """
    CHUNK = 192

    def __init__(self, device):
        self._api = device.hardware.file_system

    @staticmethod
    def _path(path):
        path = str(path).replace("\\", "/")
        if path.startswith("1:/"):
            path = path[2:]
        encoded = path.encode("utf-8")
        if (not path.startswith("/") or len(encoded) > 127 or len(encoded) < 2
                or any(part in ("", ".", "..") for part in path[1:].split("/"))
                or any(ord(c) < 32 or c in ':*?"<>|' for c in path)):
            raise ValueError("Expected an absolute SD file path of 1..127 UTF-8 bytes")
        return encoded.hex()

    @staticmethod
    def _ok(result):
        if result.is_err():
            raise RuntimeError(result.unwrap_err())
        return result.unwrap()

    def _cancel(self, token):
        try:
            self._api.cancel_file_transfer(token)
        except Exception:
            pass  # Preserve the transfer failure; firmware also has a 30s lease.

    def _put_stream(self, path, stream, size, crc, progress, overwrite):
        encoded = self._path(path)
        if not 0 <= size <= 0x7fffffff:
            raise ValueError("File exceeds the 2 GiB transfer size limit")
        token = secrets.randbits(32) or 1
        try:
            accepted = self._ok(self._api.begin_file_write(token, encoded, size, crc, overwrite))
            if accepted != size:
                raise RuntimeError("Upload size acknowledgement mismatch")
            done = 0
            while done < size:
                chunk = stream.read(min(self.CHUNK, size - done))
                if not chunk:
                    raise RuntimeError("Upload source changed or ended early")
                position = self._ok(self._api.write_file_chunk(token, done, chunk.hex()))
                if position != done + len(chunk):
                    raise RuntimeError("Upload offset acknowledgement mismatch")
                done = position
                if progress is not None:
                    progress(done, size)
            received_size, received_crc = self._ok(self._api.finish_file_transfer(token))
            if received_size != size or received_crc != crc:
                raise RuntimeError("Upload completion checksum or size mismatch")
            if size == 0 and progress is not None:
                progress(0, 0)
        except BaseException:
            self._cancel(token)
            raise

    def _get_stream(self, path, stream, progress):
        encoded = self._path(path)
        token = secrets.randbits(32) or 1
        try:
            size = self._ok(self._api.begin_file_read(token, encoded))
            if not isinstance(size, int) or not 0 <= size <= 0x7fffffff:
                raise RuntimeError("Invalid download size")
            done, crc = 0, 0
            while done < size:
                requested = min(self.CHUNK, size - done)
                count, hexdata = self._ok(self._api.read_file_chunk(token, done, requested))
                data = bytes.fromhex(hexdata)
                if count != requested or len(data) != count:
                    raise RuntimeError("Download chunk length mismatch")
                if stream.write(data) != count:
                    raise OSError("Short local file write")
                done += count
                crc = zlib.crc32(data, crc) & 0xffffffff
                if progress is not None:
                    progress(done, size)
            received_size, received_crc = self._ok(self._api.finish_file_transfer(token))
            if received_size != size or received_crc != crc:
                raise RuntimeError("Download completion checksum or size mismatch")
            if size == 0 and progress is not None:
                progress(0, 0)
        except BaseException:
            self._cancel(token)
            raise

    def put(self, dev_path, data, progress=None, *, overwrite=True):
        """Upload bytes; publish only after size and CRC verification."""
        data = bytes(data)
        self._put_stream(dev_path, io.BytesIO(data), len(data),
                         zlib.crc32(data) & 0xffffffff, progress, overwrite)

    def get(self, dev_path, progress=None):
        """Download and checksum-verify bytes."""
        output = io.BytesIO()
        self._get_stream(dev_path, output, progress)
        return output.getvalue()

    def put_file(self, host_path, dev_path, progress=None, *, overwrite=True):
        """Upload a local file using bounded memory, including on Linux."""
        with open(host_path, "rb") as source:
            size, crc = 0, 0
            for block in iter(lambda: source.read(65536), b""):
                size += len(block)
                crc = zlib.crc32(block, crc)
            source.seek(0)
            self._put_stream(dev_path, source, size, crc & 0xffffffff, progress, overwrite)

    def get_file(self, dev_path, host_path, progress=None):
        """Download to a temporary sibling; preserve the old local file on failure."""
        target = Path(host_path)
        staged = None
        try:
            with tempfile.NamedTemporaryFile(mode="wb", prefix=".onewili-",
                                             dir=target.parent, delete=False) as output:
                staged = Path(output.name)
                self._get_stream(dev_path, output, progress)
                output.flush()
                os.fsync(output.fileno())
            os.replace(staged, target)
            staged = None
        finally:
            if staged is not None:
                staged.unlink(missing_ok=True)
