"""CDC Serial Performance menu - generated from fwMenuCdcPerf. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class CdcPerf(MenuBase):
    r"""CDC Serial Performance (``i\y``)."""

    def cdc_perf_blast(self, bytes: int, chunk: int) -> Result:
        r"""Blast.

        Wire: ``i\y\b``

        Streams Bytes of deterministic XORshift32 pattern data device-to-host as fast as possible in Chunk-sized writes. Prints the line <<<BLAST>>> before the raw binary begins. Returns bytes,elapsed_us,crc32 (CRC-32 of the payload)

        Decimal: Total Bytes (1..134217728), Chunk Size (64..2048) Separated by Spaces

        Args:
            bytes: bytes (dec).
            chunk: chunk (dec).

        Returns:
            Result: Ok(bytes: int, elapsed_us: int, crc32: int) or Err(message).
        """
        return self._call("b", [encoding.enc_int(bytes), encoding.enc_int(chunk)], ["int", "int", "hex"])

    def cdc_perf_sink(self, bytes: int) -> Result:
        r"""Sink.

        Wire: ``i\y\s``

        Receives exactly Bytes of raw binary host-to-device and CRC-32-accumulates them. Prints the line <<<SINK>>> when ready to receive. A 10 second inactivity timeout aborts with failure. Returns bytes,elapsed_us,crc32

        Decimal: Total Bytes to receive (1..134217728)

        Args:
            bytes: bytes (dec).

        Returns:
            Result: Ok(bytes: int, elapsed_us: int, crc32: int) or Err(message).
        """
        return self._call("s", [encoding.enc_int(bytes)], ["int", "int", "hex"])

    def cdc_perf_echo(self, rounds: int, chunk: int) -> Result:
        r"""Echo.

        Wire: ``i\y\e``

        Per round reads exactly Chunk raw bytes from the host then writes them back verbatim, Rounds times. Prints the line <<<ECHO>>> when ready for round 1. A 10 second inactivity timeout aborts with failure. Returns rounds,elapsed_us

        Decimal: Rounds (1..100000), Chunk Size (1..512) Separated by Spaces

        Args:
            rounds: rounds (dec).
            chunk: chunk (dec).

        Returns:
            Result: Ok(rounds: int, elapsed_us: int) or Err(message).
        """
        return self._call("e", [encoding.enc_int(rounds), encoding.enc_int(chunk)], ["int", "int"])
