r"""Peer streams: best-effort datagrams between OneWili clients, routed by MAIN.

The same calls, with the same meaning, as ow_stream_write / ow_stream_poll /
ow_stream_drops in the C packages (onewili_stream.h). Python runs on clients
with no push channel to MAIN (the PC host, the CM0), so every call rides
MAIN's stream menu commands: h\a\w (write), h\a\p (poll) and h\a\c (status).
A poll fetches a batch of queued datagrams in one round trip -- packed
[src u8][len u8][data] records, exactly as the C text route reads them -- and
serves them one per call.

    from onewili.streams import Streams
    dev.streams.write(Streams.ESP32, b"hi")
    got = dev.streams.poll()        # (src, data) or None
    lost = dev.streams.drops()

- A datagram is 1..MTU bytes, delivered whole or not at all. There is no
  ordering guarantee across senders, no retry and no acknowledgement.
- Nothing waits for the destination: a datagram it cannot take is dropped and
  counted, so a successful write() means MAIN took the datagram, not that it
  arrived. drops() counts every datagram involving this client lost anywhere.
- MAIN (peer 0) is reserved and drops what is addressed to it. Addressing
  yourself is allowed (loopback).
"""
from __future__ import annotations


class Streams:
    """Peer streams for one device; use dev.streams."""

    MAIN = 0
    DISPLAY = 1
    ESP32 = 2
    CM0 = 3
    HOST = 4
    MTU = 128
    # Bytes of packed records one poll asks MAIN for: the C packages'
    # OW_STREAM_STASH default. A transport whose replies can carry more sets
    # stream_poll_bytes (the CM0 mailbox: 1024, like its C package).
    POLL_BYTES = 264

    def __init__(self, device) -> None:
        self._device = device
        self._batch = int(getattr(device._transport, "stream_poll_bytes", self.POLL_BYTES))
        self._stash = b""
        self._pos = 0
        self._local_drops = 0

    @property
    def _system(self):
        return self._device.hardware.system

    def write(self, dst: int, data) -> None:
        """Send one datagram of 1..MTU bytes to peer dst.

        Raises ValueError for a bad peer or length, and RuntimeError if MAIN
        did not take the command. A datagram MAIN had to drop is not an error
        here; it shows up in drops(), as on every other target.
        """
        if isinstance(dst, bool) or not isinstance(dst, int) or not 0 <= dst <= self.HOST:
            raise ValueError(f"stream peer must be an int 0..{self.HOST}, got {dst!r}")
        payload = bytes(data)
        if not 1 <= len(payload) <= self.MTU:
            raise ValueError(f"a datagram is 1..{self.MTU} bytes, got {len(payload)}")
        result = self._system.stream_write(dst, payload)
        if result.is_err():
            raise RuntimeError(f"stream write failed: {result.unwrap_err()}")

    def poll(self):
        """The next datagram as (src, bytes), or None when none is waiting.

        Raises RuntimeError if the poll command fails or MAIN's reply is
        corrupt (the rest of that batch is dropped and counted).
        """
        if self._pos >= len(self._stash):
            self._stash, self._pos = b"", 0
            result = self._system.stream_poll(self._batch)
            if result.is_err():
                raise RuntimeError(f"stream poll failed: {result.unwrap_err()}")
            frames, _queued, _dropped, data = result.unwrap()
            if frames <= 0 or not data:
                return None
            self._stash = bytes(data)
        stash, pos = self._stash, self._pos
        if len(stash) - pos < 2:
            raise self._corrupt("a cut record header")
        src, n = stash[pos], stash[pos + 1]
        if not 1 <= n <= self.MTU or len(stash) - pos - 2 < n or src > self.HOST:
            raise self._corrupt(f"a bad record (src {src}, length {n})")
        self._pos = pos + 2 + n
        return src, stash[pos + 2:pos + 2 + n]

    def drops(self) -> int:
        """Datagrams involving this client lost anywhere: dropped by MAIN on
        their way to it or from it, or dropped here. Free-running.

        Raises RuntimeError if the status command fails.
        """
        result = self._system.stream_status()
        if result.is_err():
            raise RuntimeError(f"stream status failed: {result.unwrap_err()}")
        _mtu, _queued, dropped_to, dropped_from = result.unwrap()
        return dropped_to + dropped_from + self._local_drops

    def _corrupt(self, what: str) -> RuntimeError:
        # MAIN packs whole records only, so the rest of this batch cannot be
        # trusted. Count one lost datagram, as the C runtime does.
        self._pos = len(self._stash)
        self._local_drops += 1
        return RuntimeError(f"stream poll: MAIN sent {what}; dropped the rest of the batch")
