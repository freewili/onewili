# CANFD Functions

`dev.io.canfd` - wire path `i\c` - generated from `fwMenuCANFD`.

## enable_canfd_stream

Stream CAN(FD). Streams received CAN frames and errors to the host.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Stream CAN(FD)

Enables or disables streaming of received CAN/CAN-FD frames and bus errors from the selected channel to the host.

###### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
- `enabled` — stream state
  - `0` — disable streaming
  - `1` — enable streaming

###### Returns

- `success` — `1` if both arguments parsed correctly, `0` otherwise.

###### Example

```
o 0 1   # enable streaming on channel 0
o 1 0   # disable streaming on channel 1
```

###### Notes

- Each channel streams independently; toggling one channel does not affect the other.
- If only `channel` parses successfully but `enabled` does not, streaming on that channel is forced **off** as a safety fallback.
- All frames will be received unless a receive filter is configured (see `Setup Filter`) so the controller actually accepts the frames you want to observe.
- FreeWili2 only has one CAN channel (channel 2 is reserved for future orcas)

Wire command: `i\c\o`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| enabled | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.canfd.enable_canfd_stream(channel: int, enabled: int) -> Result
```
```c
ow_status ow_io_canfd_enable_canfd_stream(ow_device* dev, int32_t channel, int32_t enabled);
```
```rust
dev.io().canfd().enable_canfd_stream(channel: i32, enabled: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.enable_canfd_stream(channel, enabled)   # check dev.ok
```

## write_canfd

Transmit CAN(FD). Transmits a CAN(FD) frame.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Transmit CAN(FD)

Transmits a single CAN 2.0 or CAN-FD frame on the selected channel. The frame is queued to the controller's transmit FIFO and sent as soon as the bus permits.

###### Arguments

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

###### Valid payload lengths (DLC)

- Classic CAN (`canFd = 0`): `0`–`8` bytes.
- CAN-FD (`canFd = 1`): `0`–`8`, `12`, `16`, `20`, `24`, `32`, `48`, or `64` bytes.

Any other byte count is rejected and the command reports `Invalid`.

###### Returns

- `success` — `1` if the frame was accepted by the controller for transmission, `0` otherwise.

###### Examples

```
#### Classic CAN, standard ID 0x123, 4 data bytes
w 0 0x123 0 0 DE AD BE EF

#### Classic CAN, extended ID 0x1ABCDEF, no data (remote/empty frame)
w 0 0x01ABCDEF 0 1

#### CAN-FD, standard ID 0x200, 16-byte payload
w 0 0x200 1 0 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
```

###### Notes

- The frame is queued to the transmit FIFO; if the FIFO is full or the bus is unavailable the frame may be delayed.
- Make sure the controller's bit timing (and FD data-phase timing) is configured before transmitting — see the Neptune/Orca setup commands.
- For periodic / repeated transmission use `Transmit CAN(FD) Periodic` instead.

Wire command: `i\c\w`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| arb_id | hexU32 |
| can_fd | decS32 |
| xtd_id | decS32 |
| data_in | byteArray |

Returns: none (Ok/Err only)

```python
dev.io.canfd.write_canfd(channel: int, arb_id: int, can_fd: int, xtd_id: int, data_in: bytes | bytearray) -> Result
```
```c
ow_status ow_io_canfd_write_canfd(ow_device* dev, int32_t channel, uint32_t arb_id, int32_t can_fd, int32_t xtd_id, const uint8_t* data_in, size_t data_in_len);
```
```rust
dev.io().canfd().write_canfd(channel: i32, arb_id: u32, can_fd: i32, xtd_id: i32, data_in: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.write_canfd(channel, arb_id, can_fd, xtd_id, data_in)   # check dev.ok
```

## write_canfd_periodic

Transmit CAN(FD) Periodic. Transmits a CAN(FD) frame periodically (period in us; 0 = as fast as possible).

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Transmit CAN(FD) Periodic

Configures one of the controller's periodic-transmit slots. Each slot holds a CAN/CAN-FD frame that is re-sent automatically at a fixed interval (or as fast as the bus permits) until it is disabled.

###### Arguments

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

###### Valid payload lengths (DLC)

- Classic CAN (`canFd = 0`): `0`–`8` bytes.
- CAN-FD (`canFd = 1`): `0`–`8`, `12`, `16`, `20`, `24`, `32`, `48`, or `64` bytes.

Any other byte count is rejected and the command reports `Invalid`.

###### Short form (toggle only)

If only the first arguments parse — `index`, `enable`, and `channel` — the previously configured frame in that slot is simply enabled or disabled without being re-loaded:

```
p <index> <enable> <period_ignored> <channel>
```

In practice, to just turn a configured slot off, the simplest form is:

```
p 0 0 0 0     # disable slot 0 on channel 0
```

###### Returns

- `success` — `1` if the slot was updated, `0` otherwise.

###### Examples

```
#### Slot 0 on channel 0: send classic standard-ID 0x123 with 4 data bytes every 10 ms
p 0 1 10000 0 0x123 0 0 DE AD BE EF

#### Slot 1 on channel 0: send CAN-FD standard-ID 0x200, 16-byte payload, as fast as possible
p 1 1 0 0 0x200 1 0 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF

#### Slot 2 on channel 0: extended-ID 0x1ABCDEF, no data, every 1 s
p 2 1 1000000 0 0x01ABCDEF 0 1

#### Disable slot 0 on channel 0
p 0 0 0 0
```

###### Notes

- Frames are queued to the transmit FIFO; if the FIFO is full or the bus is unavailable the frame is delayed until room becomes available.
- `period = 0` ("as fast as possible") fills any free TX FIFO slots every service tick and can saturate the bus — use with care.
- Make sure the controller's bit timing (and FD data-phase timing) is configured before transmitting — see the Neptune/Orca setup commands.
- Periodic slots are independent per channel; slot `0` on channel `0` is unrelated to slot `0` on channel `1`.
- To send a single, one-shot frame instead of a periodic stream, use `Transmit CAN(FD)`.

Wire command: `i\c\p`

| Arg | Wire type |
|---|---|
| index | decS32 |
| enable | decS32 |
| period | decS32 |
| channel | decS32 |
| arb_id | hexU32 |
| can_fd | decS32 |
| xtd_id | decS32 |
| data_in | byteArray |

Returns: none (Ok/Err only)

```python
dev.io.canfd.write_canfd_periodic(index: int, enable: int, period: int, channel: int, arb_id: int, can_fd: int, xtd_id: int, data_in: bytes | bytearray) -> Result
```
```c
ow_status ow_io_canfd_write_canfd_periodic(ow_device* dev, int32_t index, int32_t enable, int32_t period, int32_t channel, uint32_t arb_id, int32_t can_fd, int32_t xtd_id, const uint8_t* data_in, size_t data_in_len);
```
```rust
dev.io().canfd().write_canfd_periodic(index: i32, enable: i32, period: i32, channel: i32, arb_id: u32, can_fd: i32, xtd_id: i32, data_in: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.write_canfd_periodic(index, enable, period, channel, arb_id, can_fd, xtd_id, data_in)   # check dev.ok
```

## setup_filter

Setup Filter. Sets up a hardware receive filter (the byte-filter args are optional).

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Setup Filter

Configures one of the CAN controller's hardware receive filters. Frames that do not match an enabled filter are dropped before reaching the stream / FIFO. If there are no filters configure `Stream CAN(FD)` shows all messages.

###### Arguments

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

###### Returns

- `success` — `1` if the filter was updated, `0` otherwise.

###### Examples

```
#### Enable filter 0 on channel 0, standard ID, accept only ID 0x123
f 0 0 1 0 0x7FF 0x123

#### Accept any standard ID 0x100–0x10F (mask off low 4 bits)
f 0 1 1 0 0x7F0 0x100

#### Same as above, but only frames whose first data byte is 0xA5
f 0 1 1 0 0x7F0 0x100 0xFF 0xA5 0x00 0x00

#### Disable filter 2 on channel 0 (only the first three args are needed)
f 0 2 0
```

###### Notes

- Omitting `mask`/`accept` is only valid when `enable` is `0`; otherwise parsing fails and the command reports `Invalid`.
- If the byte-filter arguments are omitted, all four byte mask/accept values are cleared to `0` (i.e., no byte filtering).
- Byte filtering applies to **standard IDs only**; do not rely on `maskb0`/`maskb1` when `xtdId = 1`.
- Filters are re-applied to the controller immediately (`setupFilters(false)`) after a successful update.
- if no filters are configured all frames will be received.

Wire command: `i\c\f`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| index | decS32 |
| enable | decS32 |
| xtd_id | decS32 |
| mask | hexU32 |
| accept | hexU32 |
| maskb0 | hexU32 |
| accept_b0 | hexU32 |
| maskb1 | hexU32 |
| accept_b1 | hexU32 |

Returns: none (Ok/Err only)

```python
dev.io.canfd.setup_filter(channel: int, index: int, enable: int, xtd_id: int, mask: int, accept: int, maskb0: int, accept_b0: int, maskb1: int, accept_b1: int) -> Result
```
```c
ow_status ow_io_canfd_setup_filter(ow_device* dev, int32_t channel, int32_t index, int32_t enable, int32_t xtd_id, uint32_t mask, uint32_t accept, uint32_t maskb0, uint32_t accept_b0, uint32_t maskb1, uint32_t accept_b1);
```
```rust
dev.io().canfd().setup_filter(channel: i32, index: i32, enable: i32, xtd_id: i32, mask: u32, accept: u32, maskb0: u32, accept_b0: u32, maskb1: u32, accept_b1: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.setup_filter(channel, index, enable, xtd_id, mask, accept, maskb0, accept_b0, maskb1, accept_b1)   # check dev.ok
```

## read_can_registers

Read CAN Register(s). Reads 32-bit words from CAN controller SFR registers.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Read CAN Register(s)

Reads one or more consecutive 32-bit Special Function Registers (SFRs) from the selected CAN controller and returns them to the host as name/value pairs.

###### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.
- `startAddress` — SFR start address (hex). Aligned to the controller's 32-bit register map (e.g. MCP25xxFD-style SFRs on the on-board controller).
- `wordCount` — number of consecutive 32-bit words to read, starting at `startAddress`.

###### Returns

- `registers` — a list of `name=value` pairs, one per 32-bit word, with each value formatted as a 32-bit hex number.

###### Example

```
#### Read 4 words starting at SFR address 0x000 on channel 0
r 0 0x000 4
```

###### Notes

- The command operates directly on the CAN controller's SFR space — be careful when reading registers that have read-side-effects (e.g. interrupt/error status clears).
- Use `Set CAN Register` (`s`) to write a register, and the Neptune/Orca setup commands for normal bit-timing and configuration changes.
- Returns `Invalid` if `channel` is out of range or any argument fails to parse.
- FreeWili GUI decodes these values for helpful debugging

Wire command: `i\c\r`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| start_address | hexU32 |
| word_count | decS32 |

Returns: registers (namevaluepairH32)

```python
dev.io.canfd.read_can_registers(channel: int, start_address: int, word_count: int) -> Result
```
```c
ow_status ow_io_canfd_read_can_registers(ow_device* dev, int32_t channel, uint32_t start_address, int32_t word_count, char* registers, size_t registers_cap);
```
```rust
dev.io().canfd().read_can_registers(channel: i32, start_address: u32, word_count: i32) -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.read_can_registers(channel, start_address, word_count)   # returns value; check dev.ok
```

## set_can_register

Set CAN Register. Sets a CAN controller register.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Set CAN Register

Writes a single 32-bit (or 8-bit) Special Function Register (SFR) on the selected CAN controller. This is a direct register write — bypassing the normal setup commands — and should be used only when you know exactly what the controller expects.

###### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.
- `startAddress` — SFR address (hex). Aligned to the controller's 32-bit register map (e.g. MCP25xxFD-style SFRs on the on-board controller).
- `byteCount` — write width
  - `1` — write the low 8 bits of `wordToWrite` to `startAddress`
  - `4` — write the full 32-bit `wordToWrite` to `startAddress`
- `wordToWrite` — value to write (hex). Only the low `byteCount` bytes are used.

###### Returns

- `success` — `1` if the write was accepted, `0` otherwise.

###### Examples

```
#### Write the 32-bit value 0x00000004 to SFR 0x000 on channel 0
s 0 0x000 4 0x00000004

#### Write the single byte 0xA5 to SFR 0x010 on channel 0
s 0 0x010 1 0xA5
```

###### Notes

- This is a raw register write — be careful with registers that have write-side-effects (mode changes, FIFO control, interrupt clears, etc.).
- For normal bit-timing and configuration changes, prefer the Neptune/Orca setup commands instead of writing SFRs directly.
- Use `Read CAN Register(s)` (`r`) to verify the value after writing.
- Returns `Invalid` if `channel` is out of range or any argument fails to parse.

Wire command: `i\c\s`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| start_address | hexU32 |
| byte_count | decS32 |
| word_to_write | hexU32 |

Returns: none (Ok/Err only)

```python
dev.io.canfd.set_can_register(channel: int, start_address: int, byte_count: int, word_to_write: int) -> Result
```
```c
ow_status ow_io_canfd_set_can_register(ow_device* dev, int32_t channel, uint32_t start_address, int32_t byte_count, uint32_t word_to_write);
```
```rust
dev.io().canfd().set_can_register(channel: i32, start_address: u32, byte_count: i32, word_to_write: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.set_can_register(channel, start_address, byte_count, word_to_write)   # check dev.ok
```

## enable_canfd_receive_queue

Enable CAN(FD) Receive Queue. Enables or disables the on-device receive queue that receive_canfd (v) reads from.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Enable CAN(FD) Receive Queue

Arms or disarms the on-device receive queue for the selected channel. While armed, every frame the controller receives is copied into a small ring on the MAIN CPU, where `Receive CAN(FD)` (`v`) pops it one frame per call. This is the receive path for scripts running **on the device** (rthon, WASM): they drive commands through the local menu executor and only ever see a command's own response, never the spontaneous `[*can0 ...]` event lines that `Stream CAN(FD)` (`o`) sends to a host.

###### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.
- `enabled` — queue state
  - `0` — disable the queue (frees its storage and clears the queued and dropped counts)
  - `1` — enable the queue

###### Returns

- `success` — `1` if the queue is now in the requested state, `0` otherwise. `Invalid` means an argument did not parse; `Failed` means enabling could not get storage (PSRAM unavailable).

###### Example

```
e 0 1   # arm the receive queue on channel 0 before traffic starts
e 0 0   # disarm it and release its storage
```

###### Notes

- Depth is 32 frames per channel. When the queue is full the **oldest** frame is dropped to make room and a per-channel `dropped` counter is incremented; `Receive CAN(FD)` reports that counter with every response so a slow poller can tell how much it missed.
- Storage is allocated in PSRAM when the queue is enabled and released when it is disabled, so an idle queue costs no memory.
- The queue is independent of the host stream (`o`): both can be on at once and neither steals frames from the other.
- Enabling is idempotent: enabling an already-armed queue keeps its contents and counters.
- `Receive CAN(FD)` (`v`) auto-enables the queue on its first use, so calling `e` is only needed to disable it or to pre-arm it before traffic starts (frames received before the queue is armed are not queued).
- Requires power zone 15 (the CAN transceiver zone).

Wire command: `i\c\e`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| enabled | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.canfd.enable_canfd_receive_queue(channel: int, enabled: int) -> Result
```
```c
ow_status ow_io_canfd_enable_canfd_receive_queue(ow_device* dev, int32_t channel, int32_t enabled);
```
```rust
dev.io().canfd().enable_canfd_receive_queue(channel: i32, enabled: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.enable_canfd_receive_queue(channel, enabled)   # check dev.ok
```

## receive_canfd

Receive CAN(FD). Pops the oldest received CAN(FD) frame from the on-device receive queue (frame=0 when empty).

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Receive CAN(FD)

Pops the oldest frame from the selected channel's on-device receive queue and returns it. Intended for scripts running on the device (rthon, WASM) that cannot see the `[*can0 ...]` stream events; a host can use it too. Call it repeatedly until `frame` is `0` to drain the queue.

###### Arguments

- `channel` — CAN controller index
  - `0` — CAN channel 0 (`obCANFD1`)
  - `1` — CAN channel 1 (`obCANFD2`)
  - Note: FreeWili2 only has one CAN channel; channel `1` is reserved for future Orcas.

###### Returns

Fields are returned in this order, space-separated:

- `frame` — `1` if a frame was returned, `0` if the queue was empty. When `0`, every other field is `0` except `dropped`, and `data` is empty.
- `queued` — frames still waiting in the queue **after** this pop.
- `dropped` — running count of frames discarded (oldest first) because the queue was full, since it was enabled.
- `arbId` — arbitration ID in hex, no `0x` prefix.
- `xtdId` — `1` for an extended 29-bit ID, `0` for a standard 11-bit ID.
- `canFd` — `1` for a CAN FD frame, `0` for classic CAN 2.0.
- `timestampUs` — device uptime in microseconds when the frame was drained from the controller, as a 32-bit value (wraps every ~71 minutes).
- `dlc` — payload length in bytes (`0`–`64`).
- `data` — the payload bytes in hex (`dlc` of them), space-separated.

###### Wire layout

```
v 0
1 3 0 123 0 0 4821000 4 DE AD BE EF   # a frame: 3 more queued, none dropped, ID 0x123, classic, 4 bytes
0 0 0 0 0 0 0 0                        # empty queue (dropped may be non-zero, e.g. 0 0 17 0 0 0 0 0)
```

###### Notes

- The queue is enabled automatically on the first call, so no `Enable CAN(FD) Receive Queue` (`e`) call is needed unless you want to pre-arm it before traffic starts or to turn it off again.
- The command services the controller's receive FIFO itself before popping, so it keeps working from a caller that blocks the main loop (a running rthon or WASM script). It honours the same per-channel Neptune mode gate as the main loop. Frames drained by that in-command service are not echoed as `[*can0 ...]` text events (the echo would land in the command's own response); they still reach the binary stream and the panel table. In the normal cooperative case the main loop has already drained them, echo included, and this service finds nothing new.
- Depth is 32 frames; when full the oldest frame is dropped and `dropped` counts it. Poll faster, or use a hardware filter (`f`), if `dropped` keeps growing.
- Independent of the host stream (`o`); both may be on at once.
- `Failed` is returned if the queue could not be enabled (PSRAM unavailable); `Invalid` if `channel` did not parse.
- Requires power zone 15 (the CAN transceiver zone).

###### From a script on the device

rthon returns the fields as one list, the data bytes expanded one element each: `[frame, queued, dropped, arbId, xtdId, canFd, timestampUs, dlc, b0, b1, ...]`:

```
f = dev.io.canfd.receive_canfd(0)
if f[0] == 1:
    for i in range(f[7]):
        b = f[8 + i]
```

WASM decodes the fields into typed outputs, the byte array last:

```
ow_io_canfd_receive_canfd(dev, 0, &frame, &queued, &dropped, &arb_id, &xtd_id, &can_fd,
                          &timestamp_us, &dlc, data, sizeof data, &data_len);
```

Wire command: `i\c\v`

| Arg | Wire type |
|---|---|
| channel | decS32 |

Returns: frame (bool), queued (decS32), dropped (decU32), arb_id (hexU32), xtd_id (decS32), can_fd (decS32), timestamp_us (decU32), dlc (decS32), data (hexbytes)

```python
dev.io.canfd.receive_canfd(channel: int) -> Result
```
```c
ow_status ow_io_canfd_receive_canfd(ow_device* dev, int32_t channel, bool* frame, int32_t* queued, int32_t* dropped, uint32_t* arb_id, int32_t* xtd_id, int32_t* can_fd, int32_t* timestamp_us, int32_t* dlc, uint8_t* data, size_t data_cap, size_t* data_len);
```
```rust
dev.io().canfd().receive_canfd(channel: i32) -> Result<(bool, i32, i32, u32, i32, i32, i32, i32, Vec<u8>), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.receive_canfd(channel)   # returns value; check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `can0` (text)

CAN RX frame on channel 0 (hex arb id, 'x' suffix = extended, then hex data)

| Payload field | Wire type |
|---|---|
| arb_id | string |
| data_bytes | hexbytes |

### `can1` (text)

CAN RX frame on channel 1 (hex arb id, 'x' suffix = extended, then hex data)

| Payload field | Wire type |
|---|---|
| arb_id | string |
| data_bytes | hexbytes |

### `canTx0` (text)

CAN TX echo on channel 0 (hex arb id, 'x' suffix = extended, then hex data)

| Payload field | Wire type |
|---|---|
| arb_id | string |
| data_bytes | hexbytes |

### `canTx1` (text)

CAN TX echo on channel 1 (hex arb id, 'x' suffix = extended, then hex data)

| Payload field | Wire type |
|---|---|
| arb_id | string |
| data_bytes | hexbytes |

### `canRxReport` (binary)

CAN RX frame report (binary API, MCP2518 memory-map layout)

Header type: 1 - payload struct `apiFrame_canRxReport` (84 bytes):

| Field | C type | Offset |
|---|---|---|
| ui64TimeStampNs | uint64_t | 0 |
| uiGpioBitfield | uint32_t | 8 |
| uiR0_CANID | uint32_t | 12 |
| uiR1_Filter_HeaderBits | uint32_t | 16 |
| uiDataWords | uint32_t[16] | 20 |

Receive it:

```python
evt = dev.binary_events.get()   # CanRxReportEvent
```
```c
ow_event ev;
if (ow_binary_poll(&bdev, &ev) == 1 && ev.kind == OW_EV_CAN_RX_REPORT)
    { /* ev.u.can_rx_report.<field> */ }
```
```rust
if let Some(onewili::Event::CanRxReport(e)) = dev.poll_event()? { /* e.<field> */ }
```

## Sub-menus

- [ISO-TP Transport](isotp.md) - `dev.io.canfd.isotp`
