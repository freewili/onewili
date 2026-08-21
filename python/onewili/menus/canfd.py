"""CANFD Functions menu - generated from fwMenuCANFD. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class CANFD(MenuBase):
    r"""CANFD Functions (``i\c``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "can0": {"binary": False, "payload": [("arb_id", "string"), ("data_bytes", "hexbytes")], "description": "CAN RX frame on channel 0 (hex arb id, 'x' suffix = extended, then hex data)"},
        "can1": {"binary": False, "payload": [("arb_id", "string"), ("data_bytes", "hexbytes")], "description": "CAN RX frame on channel 1 (hex arb id, 'x' suffix = extended, then hex data)"},
        "canTx0": {"binary": False, "payload": [("arb_id", "string"), ("data_bytes", "hexbytes")], "description": "CAN TX echo on channel 0 (hex arb id, 'x' suffix = extended, then hex data)"},
        "canTx1": {"binary": False, "payload": [("arb_id", "string"), ("data_bytes", "hexbytes")], "description": "CAN TX echo on channel 1 (hex arb id, 'x' suffix = extended, then hex data)"},
        "canRxReport": {"binary": True, "header_type": 1, "payload": [("time_stamp_ns", "hexU64"), ("gpio_bitfield", "hexU32"), ("can_id", "hexU32"), ("header_bits", "hexU32"), ("data_words", "hexbytes")], "description": "CAN RX frame report (binary API, MCP2518 memory-map layout)"},
    }

    def enable_canfd_stream(self, channel: int, enabled: int) -> Result:
        r"""Stream CAN(FD).

        Wire: ``i\c\o``

        Streams received CAN frames and errors to the host.

        ## Stream CAN(FD)

Enables or disables streaming of received CAN/CAN-FD frames and bus errors from the selected channel to the host.

### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
- `enabled` — stream state
  - `0` — disable streaming
  - `1` — enable streaming

### Returns

- `success` — `1` if both arguments parsed correctly, `0` otherwise.

### Example

```
o 0 1   # enable streaming on channel 0
o 1 0   # disable streaming on channel 1
```

### Notes

- Each channel streams independently; toggling one channel does not affect the other.
- If only `channel` parses successfully but `enabled` does not, streaming on that channel is forced **off** as a safety fallback.
- All frames will be received unless a receive filter is configured (see `Setup Filter`) so the controller actually accepts the frames you want to observe.
- FreeWili2 only has one CAN channel (channel 2 is reserved for future orcas)

        Enter Channel and enable state

        Args:
            channel: channel (decS32).
            enabled: enabled (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(channel), encoding.enc_int(enabled)], [])

    def write_canfd(self, channel: int, arb_id: int, can_fd: int, xtd_id: int, data_in: bytes | bytearray) -> Result:
        r"""Transmit CAN(FD).

        Wire: ``i\c\w``

        Transmits a CAN(FD) frame.

        ## Transmit CAN(FD)

Transmits a single CAN 2.0 or CAN-FD frame on the selected channel. The frame is queued to the controller's transmit FIFO and sent as soon as the bus permits.

### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.
- `arbId` — arbitration ID (hex)
  - 11-bit value (`0x000`–`0x7FF`) when `xtdId = 0`
  - 29-bit value (`0x00000000`–`0x1FFFFFFF`) when `xtdId = 1`
- `canFd` — frame format
  - `0` — classic CAN 2.0 frame
  - `1` — CAN-FD frame (allows >8 data bytes and bit-rate switching as configured on the controller)
- `xtdId` — identifier length
  - `0` — standard 11-bit ID
  - `1` — extended 29-bit ID
- `dataIn` — payload bytes in hex, space-separated (e.g. `DE AD BE EF`). May be empty for a zero-length frame.

### Valid payload lengths (DLC)

- Classic CAN (`canFd = 0`): `0`–`8` bytes.
- CAN-FD (`canFd = 1`): `0`–`8`, `12`, `16`, `20`, `24`, `32`, `48`, or `64` bytes.

Any other byte count is rejected and the command reports `Invalid`.

### Returns

- `success` — `1` if the frame was accepted by the controller for transmission, `0` otherwise.

### Examples

```
# Classic CAN, standard ID 0x123, 4 data bytes
w 0 0x123 0 0 DE AD BE EF

# Classic CAN, extended ID 0x1ABCDEF, no data (remote/empty frame)
w 0 0x01ABCDEF 0 1

# CAN-FD, standard ID 0x200, 16-byte payload
w 0 0x200 1 0 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
```

### Notes

- The frame is queued to the transmit FIFO; if the FIFO is full or the bus is unavailable the frame may be delayed.
- Make sure the controller's bit timing (and FD data-phase timing) is configured before transmitting — see the Neptune/Orca setup commands.
- For periodic / repeated transmission use `Transmit CAN(FD) Periodic` instead.

        Channel ArbID (hex) isCANFD isXtd Bytes (hex)

        Args:
            channel: channel (decS32).
            arb_id: arb_id (hexU32).
            can_fd: can_fd (decS32).
            xtd_id: xtd_id (decS32).
            data_in: data_in (byteArray).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_int(channel), encoding.enc_hex(arb_id, 8), encoding.enc_int(can_fd), encoding.enc_int(xtd_id), encoding.enc_bytes(data_in)], [])

    def write_canfd_periodic(self, index: int, enable: int, period: int, channel: int, arb_id: int, can_fd: int, xtd_id: int, data_in: bytes | bytearray) -> Result:
        r"""Transmit CAN(FD) Periodic.

        Wire: ``i\c\p``

        Transmits a CAN(FD) frame periodically (period in us; 0 = as fast as possible).

        ## Transmit CAN(FD) Periodic

Configures one of the controller's periodic-transmit slots. Each slot holds a CAN/CAN-FD frame that is re-sent automatically at a fixed interval (or as fast as the bus permits) until it is disabled.

### Arguments

- `index` — periodic slot, `0` … `RPCANFD_NUM_PERIODICS-1`. Each channel has its own independent set of slots.
- `enable` — slot state
  - `1` — enable the slot (and load it with the frame defined below)
  - `0` — disable the slot
- `period` — transmit interval in microseconds
  - `0` — send as fast as possible (whenever the TX FIFO has room)
  - `>0` — send one frame every `period` µs
- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.
- `arbId` — arbitration ID (hex)
  - 11-bit (`0x000`–`0x7FF`) when `xtdId = 0`
  - 29-bit (`0x00000000`–`0x1FFFFFFF`) when `xtdId = 1`
- `canFd` — frame format
  - `0` — classic CAN 2.0
  - `1` — CAN-FD
- `xtdId` — identifier length
  - `0` — standard 11-bit ID
  - `1` — extended 29-bit ID
- `dataIn` — payload bytes in hex, space-separated (e.g. `DE AD BE EF`). May be empty for a zero-length frame.

### Valid payload lengths (DLC)

- Classic CAN (`canFd = 0`): `0`–`8` bytes.
- CAN-FD (`canFd = 1`): `0`–`8`, `12`, `16`, `20`, `24`, `32`, `48`, or `64` bytes.

Any other byte count is rejected and the command reports `Invalid`.

### Short form (toggle only)

If only the first arguments parse — `index`, `enable`, and `channel` — the previously configured frame in that slot is simply enabled or disabled without being re-loaded:

```
p <index> <enable> <period_ignored> <channel>
```

In practice, to just turn a configured slot off, the simplest form is:

```
p 0 0 0 0     # disable slot 0 on channel 0
```

### Returns

- `success` — `1` if the slot was updated, `0` otherwise.

### Examples

```
# Slot 0 on channel 0: send classic standard-ID 0x123 with 4 data bytes every 10 ms
p 0 1 10000 0 0x123 0 0 DE AD BE EF

# Slot 1 on channel 0: send CAN-FD standard-ID 0x200, 16-byte payload, as fast as possible
p 1 1 0 0 0x200 1 0 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF

# Slot 2 on channel 0: extended-ID 0x1ABCDEF, no data, every 1 s
p 2 1 1000000 0 0x01ABCDEF 0 1

# Disable slot 0 on channel 0
p 0 0 0 0
```

### Notes

- Frames are queued to the transmit FIFO; if the FIFO is full or the bus is unavailable the frame is delayed until room becomes available.
- `period = 0` ("as fast as possible") fills any free TX FIFO slots every service tick and can saturate the bus — use with care.
- Make sure the controller's bit timing (and FD data-phase timing) is configured before transmitting — see the Neptune/Orca setup commands.
- Periodic slots are independent per channel; slot `0` on channel `0` is unrelated to slot `0` on channel `1`.
- To send a single, one-shot frame instead of a periodic stream, use `Transmit CAN(FD)`.

        index enable period (us) Channel (0-1) ArbID (hex) isCANFD isXtd Bytes (hex)

        Args:
            index: index (decS32).
            enable: enable (decS32).
            period: period (decS32).
            channel: channel (decS32).
            arb_id: arb_id (hexU32).
            can_fd: can_fd (decS32).
            xtd_id: xtd_id (decS32).
            data_in: data_in (byteArray).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(index), encoding.enc_int(enable), encoding.enc_int(period), encoding.enc_int(channel), encoding.enc_hex(arb_id, 8), encoding.enc_int(can_fd), encoding.enc_int(xtd_id), encoding.enc_bytes(data_in)], [])

    def setup_filter(self, channel: int, index: int, enable: int, xtd_id: int, mask: int, accept: int, maskb0: int, accept_b0: int, maskb1: int, accept_b1: int) -> Result:
        r"""Setup Filter.

        Wire: ``i\c\f``

        Sets up a hardware receive filter (the byte-filter args are optional).

        ## Setup Filter

Configures one of the CAN controller's hardware receive filters. Frames that do not match an enabled filter are dropped before reaching the stream / FIFO. If there are no filters configure `Stream CAN(FD)` shows all messages.

### Arguments

Arguments are space-separated. The four byte-filter arguments at the end are **optional**.

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.
- `index` — hardware filter slot, `0` … `RPCANFD_FILTERS_COUNT-1` (up to 32).
- `enable` — `1` to enable the filter, `0` to disable it.
- `xtdId` — `0` = standard 11-bit ID, `1` = extended 29-bit ID.
- `mask` — ID mask (hex). A `1` bit means "this bit must match"; a `0` bit is don't-care.
- `accept` — ID value to match after the mask is applied (hex).
- `maskb0`, `acceptB0` *(optional)* — mask/accept for data byte 0 (hex, 8-bit). **Standard IDs only.**
- `maskb1`, `acceptB1` *(optional)* — mask/accept for data byte 1 (hex, 8-bit). **Standard IDs only.**

A frame is accepted when:

```
(rxId    & mask)    == accept
(rxByte0 & maskb0)  == acceptB0      # if byte filters provided
(rxByte1 & maskb1)  == acceptB1      # if byte filters provided
```

### Returns

- `success` — `1` if the filter was updated, `0` otherwise.

### Examples

```
# Enable filter 0 on channel 0, standard ID, accept only ID 0x123
f 0 0 1 0 0x7FF 0x123

# Accept any standard ID 0x100–0x10F (mask off low 4 bits)
f 0 1 1 0 0x7F0 0x100

# Same as above, but only frames whose first data byte is 0xA5
f 0 1 1 0 0x7F0 0x100 0xFF 0xA5 0x00 0x00

# Disable filter 2 on channel 0 (only the first three args are needed)
f 0 2 0
```

### Notes

- Omitting `mask`/`accept` is only valid when `enable` is `0`; otherwise parsing fails and the command reports `Invalid`.
- If the byte-filter arguments are omitted, all four byte mask/accept values are cleared to `0` (i.e., no byte filtering).
- Byte filtering applies to **standard IDs only**; do not rely on `maskb0`/`maskb1` when `xtdId = 1`.
- Filters are re-applied to the controller immediately (`setupFilters(false)`) after a successful update.
- if no filters are configured all frames will be received.

        channel (0-1), index (0-32), enable, isXTD, mskID, ID, [(optional) mskb0, b0, mskb1, b1]

        Args:
            channel: channel (decS32).
            index: index (decS32).
            enable: enable (decS32).
            xtd_id: xtd_id (decS32).
            mask: mask (hexU32).
            accept: accept (hexU32).
            maskb0: maskb0 (hexU32).
            accept_b0: accept_b0 (hexU32).
            maskb1: maskb1 (hexU32).
            accept_b1: accept_b1 (hexU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(channel), encoding.enc_int(index), encoding.enc_int(enable), encoding.enc_int(xtd_id), encoding.enc_hex(mask, 8), encoding.enc_hex(accept, 8), encoding.enc_hex(maskb0, 8), encoding.enc_hex(accept_b0, 8), encoding.enc_hex(maskb1, 8), encoding.enc_hex(accept_b1, 8)], [])

    def read_can_registers(self, channel: int, start_address: int, word_count: int) -> Result:
        r"""Read CAN Register(s).

        Wire: ``i\c\r``

        Reads 32-bit words from CAN controller SFR registers.

        ## Read CAN Register(s)

Reads one or more consecutive 32-bit Special Function Registers (SFRs) from the selected CAN controller and returns them to the host as name/value pairs.

### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.
- `startAddress` — SFR start address (hex). Aligned to the controller's 32-bit register map (e.g. MCP25xxFD-style SFRs on the on-board controller).
- `wordCount` — number of consecutive 32-bit words to read, starting at `startAddress`.

### Returns

- `registers` — a list of `name=value` pairs, one per 32-bit word, with each value formatted as a 32-bit hex number.

### Example

```
# Read 4 words starting at SFR address 0x000 on channel 0
r 0 0x000 4
```

### Notes

- The command operates directly on the CAN controller's SFR space — be careful when reading registers that have read-side-effects (e.g. interrupt/error status clears).
- Use `Set CAN Register` (`s`) to write a register, and the Neptune/Orca setup commands for normal bit-timing and configuration changes.
- Returns `Invalid` if `channel` is out of range or any argument fails to parse.
- FreeWili GUI decodes these values for helpful debugging

        channel (0-1) address (hex) wordcount

        Args:
            channel: channel (decS32).
            start_address: start_address (hexU32).
            word_count: word_count (decS32).

        Returns:
            Result: Ok(registers: str) or Err(message).
        """
        return self._call("r", [encoding.enc_int(channel), encoding.enc_hex(start_address, 8), encoding.enc_int(word_count)], ["str"])

    def set_can_register(self, channel: int, start_address: int, byte_count: int, word_to_write: int) -> Result:
        r"""Set CAN Register.

        Wire: ``i\c\s``

        Sets a CAN controller register.

        ## Set CAN Register

Writes a single 32-bit (or 8-bit) Special Function Register (SFR) on the selected CAN controller. This is a direct register write — bypassing the normal setup commands — and should be used only when you know exactly what the controller expects.

### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.
- `startAddress` — SFR address (hex). Aligned to the controller's 32-bit register map (e.g. MCP25xxFD-style SFRs on the on-board controller).
- `byteCount` — write width
  - `1` — write the low 8 bits of `wordToWrite` to `startAddress`
  - `4` — write the full 32-bit `wordToWrite` to `startAddress`
- `wordToWrite` — value to write (hex). Only the low `byteCount` bytes are used.

### Returns

- `success` — `1` if the write was accepted, `0` otherwise.

### Examples

```
# Write the 32-bit value 0x00000004 to SFR 0x000 on channel 0
s 0 0x000 4 0x00000004

# Write the single byte 0xA5 to SFR 0x010 on channel 0
s 0 0x010 1 0xA5
```

### Notes

- This is a raw register write — be careful with registers that have write-side-effects (mode changes, FIFO control, interrupt clears, etc.).
- For normal bit-timing and configuration changes, prefer the Neptune/Orca setup commands instead of writing SFRs directly.
- Use `Read CAN Register(s)` (`r`) to verify the value after writing.
- Returns `Invalid` if `channel` is out of range or any argument fails to parse.

        channel (0-1) address (hex) bytesize (1,4) word (hex)

        Args:
            channel: channel (decS32).
            start_address: start_address (hexU32).
            byte_count: byte_count (decS32).
            word_to_write: word_to_write (hexU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(channel), encoding.enc_hex(start_address, 8), encoding.enc_int(byte_count), encoding.enc_hex(word_to_write, 8)], [])
