"""RFID Functions menu - generated from fwMenuRFID. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class RFID(MenuBase):
    r"""RFID Functions (``w\p``)."""

    def enable_reader(self, enable: int) -> Result:
        r"""Enable Reader.

        Wire: ``w\p\r``

        Start or stop the 125 kHz carrier and tag reader

        # Enable Reader

Starts or stops the 125 kHz carrier and demodulator.

```
r 1
```

Enabling claims GPIO46 (envelope) and GPIO34 (carrier). GPIO34 shares PWM slice 9 with the haptic, and clkdiv is per-slice, so a buzz while the reader runs disturbs the carrier.


        Enter 1 to enable, 0 to disable

        Args:
            enable: enable (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_int(enable)], [])

    def get_status(self) -> Result:
        r"""Get Status.

        Wire: ``w\p\g``

        Reader state, carrier frequency and live envelope

        # Get Status

Reader state, measured carrier, live envelope window and the frame/tag counters.

A large `frames` with a near-zero `tags` means the front end produces edges no decoder accepts, which points at bit timing rather than coupling.


        Returns:
            Result: Ok(state: int, flags: int, carrier_hz: int, env_min: int, env_max: int, threshold: int, frames: int, tags: int) or Err(message).
        """
        return self._call("g", [], ["int", "hex", "int", "int", "int", "int", "int", "int"])

    def read_tag(self, timeout_ms: int) -> Result:
        r"""Read Tag.

        Wire: ``w\p\t``

        Block until one tag is decoded or the timeout expires

        # Read Tag

Waits for one tag and returns its format, modulation and id.

```
t 2000
```

An unknown format still returns what was assembled, so an unrecognised tag is visible rather than dropped. Use `b` for its raw bits.


        Enter timeout in milliseconds (default 2000)

        Args:
            timeout_ms: timeout_ms (dec).

        Returns:
            Result: Ok(format: int, modulation: int, id: bytes | bytearray) or Err(message).
        """
        return self._call("t", [encoding.enc_int(timeout_ms)], ["int", "int", "bytes"])

    def stream_tags(self, enable: int) -> Result:
        r"""Stream Tags.

        Wire: ``w\p\s``

        Push each decoded tag to the host as an event

        # Stream Tags

Emits each decoded tag as an event instead of requiring a poll.

Tags drain a few per pass rather than in a burst: the event FIFO is 24 slots and a busy field produces tags faster than the host drains them.


        Enter 1 to stream tags as events, 0 to stop

        Args:
            enable: enable (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(enable)], [])

    def clear_stats(self) -> Result:
        r"""Clear Stats.

        Wire: ``w\p\c``

        Zero the frame and tag counters

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def tune(self, param: int, value: int) -> Result:
        r"""Tune Constant.

        Wire: ``w\p\u``

        Set a demodulator constant live, without reflashing

        # Tune Constant

Sets one demodulator constant at runtime, without a reflash.

```
u <param> <value>
```

- `0` ASK bit period us (512 = RF/64)
- `1` ASK minimum pulse us
- `2` ASK swing divisor
- `3` ASK minimum hysteresis
- `4` PSK tolerance percent
- `5` PSK carrier cycles per bit

Values survive `r 0`/`r 1` and reset on power cycle.


        Enter parameter index and value

        Args:
            param: param (dec).
            value: value (dec).

        Returns:
            Result: Ok(param: int, value: int) or Err(message).
        """
        return self._call("u", [encoding.enc_int(param), encoding.enc_int(value)], ["int", "int"])

    def raw_bits(self) -> Result:
        r"""Raw Bits.

        Wire: ``w\p\b``

        Raw bits of the last assembled frame

        # Raw Bits

Returns the last 64-bit frame as assembled, before any decoder ran.

Useful when a tag is present and framing succeeds but nothing claims it: the raw bits separate a wrong format from a bit-timing error.


        Returns:
            Result: Ok(modulation: int, length: int, bits: bytes | bytearray) or Err(message).
        """
        return self._call("b", [], ["int", "int", "bytes"])

    def write_tag(self, block: int, value: int) -> Result:
        r"""Write Tag.

        Wire: ``w\p\w``

        Write one 32-bit block to a T5577/T5557 tag

        # Write Tag

Writes one 32-bit block to a T5577/T5557 held in the field.

```
w <block 1-7> <hex>
```

The reader must be running. The write is blind: the tag never acknowledges, so success means the frame was transmitted, not accepted. Verify by reading it back.

Block 0 is the configuration block and is refused: a wrong value there is not recoverable by writing again.


        Enter block number (0-7) and a 32-bit value in hex

        Args:
            block: block (dec).
            value: value (hex).

        Returns:
            Result: Ok(result: int, block: int) or Err(message).
        """
        return self._call("w", [encoding.enc_int(block), encoding.enc_hex(value)], ["int", "int"])

    def carrier_info(self) -> Result:
        r"""Carrier Info.

        Wire: ``w\p\i``

        Measured carrier and PSK front-end telemetry

        # Carrier Info

Measured carrier, clk_sys, envelope sample count and capture overruns.

Needs no tag. A climbing `env_samples` proves the ADC was claimed and the front end is being sampled. `frames` staying at zero against a bare carrier is correct.


        Returns:
            Result: Ok(carrier_hz: int, psk_active: bool, psk_events: int, poll_count: int, clk_hz: int, clock_ok: bool, env_samples: int, overruns: int, restarts: int, psk_period: int) or Err(message).
        """
        return self._call("i", [], ["int", "bool", "int", "int", "int", "bool", "int", "int", "int", "int"])

    def enroll_id(self, id: bytes | bytearray) -> Result:
        r"""Enroll ID.

        Wire: ``w\p\n``

        Write a caller-supplied EM4100 ID onto the card in the field

        # Enroll ID

Encodes a 40-bit EM4100 id and writes it into blocks 1 and 2.

```
n A1 B2 C3 D4 E5
```

Five space-separated bytes, most significant first. The card must already be EM4100-configured (block 0 = 00148040); this firmware cannot write block 0, so it cannot convert a blank card.

Blind write. Verify with `t`.


        Enter a 10-hex-digit EM4100 ID as five space-separated bytes

        Args:
            id: id (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_bytes(id)], [])

    def clone_capture(self, timeout_ms: int) -> Result:
        r"""Clone Capture.

        Wire: ``w\p\k``

        Read a card and hold its ID for a later clone write

        # Clone Capture

Reads the card on the coil and holds its id for `j`.

Held separately from the last-tag latch, so the reads `j` performs on the target card cannot overwrite it.


        Enter timeout in milliseconds (default 3000)

        Args:
            timeout_ms: timeout_ms (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_int(timeout_ms)], [])

    def clone_write(self) -> Result:
        r"""Clone Write.

        Wire: ``w\p\j``

        Write the captured ID onto the card now on the coil

        # Clone Write

Writes the id captured by `k` onto the card now on the coil.

Refuses unless the target is currently reading as EM4100, since block 0 cannot be written and a non-EM4100 card would never broadcast the frame. Nothing is transmitted when refused.

Blind write. Verify with `t`.


        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [], [])
