# ISO-TP Transport

`dev.io.canfd.isotp` - wire path `i\c\t` - generated from `fwMenuISOTP`.

## iso_tp_enable

Enable ISO-TP. Arms (1) or disarms (0) the ISO-TP transport on CAN channel 0: takes the PSRAM staging window, taps received frames and starts answering flow control for messages sent to rxId. Send Message and Send File arm it automatically.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Enable ISO-TP

Arms or disarms the ISO 15765-2 transport layer on CAN channel 0.

While armed, every frame the controller receives is also inspected by the transport: a single frame or first frame addressed to the configured `rxId` is accepted, flow control is answered automatically and the completed message waits for `Receive Message` (`r`). Arming also takes a 16 KiB staging window in PSRAM for file transfers.

###### Arguments

- `enable` -- `1` to arm, `0` to disarm. Disarming aborts anything in progress, releases the staging window and forgets any unread received message.

###### Returns

- `success` -- `1` if the transport is now in the requested state. `Failed` means arming could not get PSRAM.

###### Example

```
e 1   # arm before the peer starts sending
e 0   # release
```

###### Notes

- `Send Message` (`s`) and `Send File` (`x`) arm the transport on their own, so `e 1` is only needed to receive unsolicited messages or to pre-arm before traffic starts. Frames that arrive before arming are not seen.
- The controller itself must be configured (bit timing, FD data rate) from the Neptune settings, and hardware receive filters (`Setup Filter` in the CAN FD menu) must let `rxId` through.
- Requires power zone 15 (the CAN transceiver zone). The rail is powered on demand: enabling the transport (or the CAN stream) raises it and it takes about two seconds to come up, so arm with `e 1` a moment before the first transfer. A send attempted while the controller is still unpowered returns result 12 (SendFailed) immediately.

Wire command: `i\c\t\e`

| Arg | Wire type |
|---|---|
| enable | bool |

Returns: none (Ok/Err only)

```python
dev.io.canfd.isotp.iso_tp_enable(enable: bool) -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_enable(ow_device* dev, bool enable);
```
```rust
dev.io().canfd().isotp().iso_tp_enable(enable: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_enable(enable)   # check dev.ok
```

## iso_tp_configure_addressing

Configure Addressing. Sets the CAN ids the transport sends on and listens to, 11/29-bit ids, classic CAN or CAN FD, the TX_DL frame size (8 classic; 8,12,16,20,24,32,48,64 FD), padding and pad byte, and normal (0) or extended (1) addressing with its N_TA byte.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Configure Addressing

Defines the ISO-TP address pair and frame format. Defaults: `7E0` / `7E8`, 11-bit, classic CAN, TX_DL 8, padding on with `CC`, normal addressing.

###### Arguments

- `txId` -- CAN id (hex, no `0x`) this device transmits on: its SF/FF/CF data frames and, when receiving, its flow control frames.
- `rxId` -- CAN id (hex) this device listens on: the peer's data frames and the peer's flow control.
- `extendedId` -- `0` for 11-bit ids (max `7FF`), `1` for 29-bit ids (max `1FFFFFFF`). Applies to both ids.
- `canFd` -- `0` classic CAN 2.0 frames, `1` CAN FD frames (bit-rate switch is always used).
- `txDataLength` -- TX_DL, the payload size of every frame this device sends: `8` for classic CAN (any other value is forced to 8); for CAN FD one of `8`, `12`, `16`, `20`, `24`, `32`, `48`, `64` (the largest frames carry 63 bytes per consecutive frame).
- `padding` -- `1` pads single frames, flow control and the last consecutive frame up to TX_DL; `0` sends them short. CAN FD frames whose length is not a valid DLC are always padded up to the next valid one.
- `padByte` -- pad value (hex, `00`-`FF`).
- `addressingMode` -- `0` normal addressing, `1` extended addressing (the first data byte of every frame carries the target address N_TA and one byte less of payload).
- `extAddress` -- the N_TA byte (hex) used and expected when `addressingMode` is `1`.

###### Returns

- `success` -- `1` when stored. `Invalid` if an id exceeds its width, `txDataLength` is not a valid size, or a byte value exceeds `FF`; `Failed` while a transfer is running.

###### Examples

```
#### UDS tester defaults, classic CAN
c 7E0 7E8 0 0 8 1 CC 0 00

#### CAN FD, 64-byte frames, no padding
c 7E0 7E8 0 1 64 0 CC 0 00

#### 29-bit ids with extended addressing, target address 0x21
c 18DA10F1 18DAF110 1 0 8 1 AA 1 21
```

###### Notes

- The peer must use the mirror image (its txId is our rxId and vice versa).
- Changing the format does not touch the controller's bit timing; set that from the Neptune settings.
- Requires power zone 15.

Wire command: `i\c\t\c`

| Arg | Wire type |
|---|---|
| tx_id | hexU32 |
| rx_id | hexU32 |
| extended_id | bool |
| can_fd | bool |
| tx_data_length | decU32 |
| padding | bool |
| pad_byte | hexU32 |
| addressing_mode | decU32 |
| ext_address | hexU32 |

Returns: none (Ok/Err only)

```python
dev.io.canfd.isotp.iso_tp_configure_addressing(tx_id: int, rx_id: int, extended_id: bool, can_fd: bool, tx_data_length: int, padding: bool, pad_byte: int, addressing_mode: int, ext_address: int) -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_configure_addressing(ow_device* dev, uint32_t tx_id, uint32_t rx_id, bool extended_id, bool can_fd, int32_t tx_data_length, bool padding, uint32_t pad_byte, int32_t addressing_mode, uint32_t ext_address);
```
```rust
dev.io().canfd().isotp().iso_tp_configure_addressing(tx_id: u32, rx_id: u32, extended_id: bool, can_fd: bool, tx_data_length: i32, padding: bool, pad_byte: u32, addressing_mode: i32, ext_address: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_configure_addressing(tx_id, rx_id, extended_id, can_fd, tx_data_length, padding, pad_byte, addressing_mode, ext_address)   # check dev.ok
```

## iso_tp_configure_flow_control

Configure Flow Control. Sets what this device advertises in its own flow control frames when receiving -- block size (0 = no limit) and the STmin byte (00-7F ms, F1-F9 = 100-900 us) -- and how many consecutive WAIT frames it tolerates from the peer when sending (default 8).

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Configure Flow Control

Sets the flow control parameters this device advertises to a sending peer, and its tolerance for the peer's WAIT frames when this device is the sender. Defaults: block size 0, STmin `00`, wftMax 8.

###### Arguments

- `blockSize` -- BS, `0`-`255`: how many consecutive frames the peer may send before it must wait for another flow control frame from us. `0` means no limit (one flow control for the whole message). When a message longer than 256 bytes is being written to the SD card and this is `0`, the transport advertises 255 instead so the card is written between blocks.
- `stMin` -- the STmin byte (hex) we ask the peer to keep between its consecutive frames: `00`-`7F` is that many milliseconds, `F1`-`F9` is 100-900 microseconds. Other values are reserved and a standard-conforming peer treats them as 127 ms.
- `wftMax` -- when this device is sending, the number of consecutive flow control WAIT frames it accepts from the peer before giving up with result `TooManyWaits` (11).

###### Returns

- `success` -- `1` when stored; `Invalid` for values out of range; `Failed` while a transfer is running.

###### Examples

```
f 0 00 8      # no block limit, no minimum gap
f 8 0A 8      # 8 frames per block, 10 ms between frames
f 16 F5 4     # 16 frames per block, 500 us between frames, at most 4 WAITs
```

###### Notes

- These values describe what we ASK the peer to do when it sends to us. How this device paces its own consecutive frames is decided by the peer's flow control, adjusted with `Set STmin Trim` (`t`).
- Requires power zone 15.

Wire command: `i\c\t\f`

| Arg | Wire type |
|---|---|
| block_size | decU32 |
| st_min | hexU32 |
| wft_max | decU32 |

Returns: none (Ok/Err only)

```python
dev.io.canfd.isotp.iso_tp_configure_flow_control(block_size: int, st_min: int, wft_max: int) -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_configure_flow_control(ow_device* dev, int32_t block_size, uint32_t st_min, int32_t wft_max);
```
```rust
dev.io().canfd().isotp().iso_tp_configure_flow_control(block_size: i32, st_min: u32, wft_max: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_configure_flow_control(block_size, st_min, wft_max)   # check dev.ok
```

## iso_tp_set_st_min_trim

Set STmin Trim. Adjusts how this device paces its consecutive frames: stMinTrimUs is a signed number of microseconds added to the peer's STmin (negative values cancel the SPI write latency of about 100 us); stMinOverrideUs ignores the peer's STmin and paces at exactly that many microseconds (+ trim), -1 follows the peer.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Set STmin Trim

Tunes the separation time this device keeps between its own consecutive frames when sending a multi-frame message. Defaults: trim 0, override -1.

The separation is measured from the END of one consecutive frame on the bus to the START of the next. The transport waits for the controller to report the frame finished, then schedules the next one `STmin + trim` later. Between that schedule and the frame's first bit there is a constant SPI write latency (about 100 us on this board); the trim exists to cancel it so the measured bus gap lands exactly on STmin.

###### Arguments

- `stMinTrimUs` -- signed microseconds added to the effective STmin. Negative values shorten the gap; the result is never less than 0.
- `stMinOverrideUs` -- `-1` follows the STmin the peer sends in its flow control (the normal, standard-conforming behaviour); any value `0` or more ignores the peer and paces at exactly that many microseconds (plus trim). Use it to test a peer's tolerance or to drive a fixed cadence.

###### Returns

- `success` -- `1` when stored; `Invalid` if the override is below -1; `Failed` while a transfer is running.

###### Examples

```
t 0 -1        # standard behaviour
t -100 -1     # follow the peer, but start each frame 100 us earlier
t 0 2000      # ignore the peer, exactly 2 ms between frames
```

###### Notes

- When the effective gap is 0 the transport does not wait for each frame to finish; it pipelines consecutive frames into the controller's three transmit slots as fast as they free up.
- `Send Message` and `Send File` report the measured minimum, maximum and average gap of the transfer so the trim can be checked without a bus analyzer.
- Requires power zone 15.

Wire command: `i\c\t\t`

| Arg | Wire type |
|---|---|
| st_min_trim_us | decS32 |
| st_min_override_us | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.canfd.isotp.iso_tp_set_st_min_trim(st_min_trim_us: int, st_min_override_us: int) -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_set_st_min_trim(ow_device* dev, int32_t st_min_trim_us, int32_t st_min_override_us);
```
```rust
dev.io().canfd().isotp().iso_tp_set_st_min_trim(st_min_trim_us: i32, st_min_override_us: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_set_st_min_trim(st_min_trim_us, st_min_override_us)   # check dev.ok
```

## iso_tp_send_message

Send Message. Sends up to 256 bytes as one ISO-TP message (single frame, or first frame + flow-controlled consecutive frames) and blocks until it is delivered, aborted or timed out; returns the result code (0 = Ok), bytes and frames sent, the duration and the measured consecutive-frame gaps in microseconds.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Send Message

Transmits one ISO-TP message whose payload is given inline, and waits for the outcome. Short payloads go as a single frame; longer ones as a first frame followed by consecutive frames paced by the peer's flow control.

The command BLOCKS until the message is delivered, fails or the transfer limit (120 s) expires. Nothing else runs on the device meanwhile, and the CAN text stream is muted for the duration so the response cannot overflow.

###### Arguments

- `data` -- 1 to 256 payload bytes in hex, space-separated (e.g. `22 F1 90`). Longer messages are sent from the SD card with `Send File` (`x`).

###### Returns

Seven decimal fields, space-separated:

- `result` -- outcome code: `0` Ok, `1` Busy (a transfer was already running), `2` Aborted, `3` TimeoutBs (no flow control from the peer), `4` TimeoutCr, `5` TimeoutAs (the controller never reported a frame sent), `6` WrongSequence, `7` Overflow (the peer answered OVFLW), `8` InvalidLength, `9` SinkError, `10` SourceError, `11` TooManyWaits, `12` SendFailed (the controller refused a frame).
- `bytes` -- payload bytes transmitted before the transfer ended.
- `frames` -- data frames transmitted (SF, or FF + CFs).
- `durationUs` -- microseconds from the first frame to completion or failure.
- `minGapUs`, `maxGapUs`, `avgGapUs` -- measured separation between consecutive frames (from one frame's completion to the next frame's send), excluding the first frame after each flow control. All `0` for a single-frame message.

###### Wire layout

```
s 22 F1 90
0 3 1 412 0 0 0                  # single frame, delivered

s 2E F1 90 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
0 19 4 3810 1012 1031 1020      # FF + 3 CFs at STmin 1 ms

s 2E F1 90 00 01 02 03 04 05
3 6 1 1000208 0 0 0             # nobody answered the FF: TimeoutBs
```

###### Notes

- The command reports success whenever the transfer could be attempted; read `result` for the outcome. `Invalid` means no bytes or more than 256 were given; `Failed` means the transport could not be armed (no PSRAM).
- The transport is armed automatically if it was not.
- The controller must be up with the right bit timing (Neptune settings) and its receive filters must admit `rxId`, or the peer's flow control never arrives and the result is `3`.
- Requires power zone 15.

Wire command: `i\c\t\s`

| Arg | Wire type |
|---|---|
| data | byteArray |

Returns: result (decU32), bytes (decU32), frames (decU32), duration_us (decU32), min_gap_us (decU32), max_gap_us (decU32), avg_gap_us (decU32)

```python
dev.io.canfd.isotp.iso_tp_send_message(data: bytes | bytearray) -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_send_message(ow_device* dev, const uint8_t* data, size_t data_len, int32_t* result, int32_t* bytes, int32_t* frames, int32_t* duration_us, int32_t* min_gap_us, int32_t* max_gap_us, int32_t* avg_gap_us);
```
```rust
dev.io().canfd().isotp().iso_tp_send_message(data: &[u8]) -> Result<(i32, i32, i32, i32, i32, i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_send_message(data)   # returns value; check dev.ok
```

## iso_tp_send_file

Send File. Sends the whole content of an SD card file as one ISO-TP message, paging it from the card through a 16 KiB PSRAM window while the peer is not waiting on a frame; blocks like Send Message and returns the same result fields.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Send File

Transmits the entire content of a file on the SD card as one ISO-TP message. This is the path for payloads longer than the 256 inline bytes `Send Message` (`s`) accepts; messages up to 4095 bytes use the standard 12-bit length, longer ones the 32-bit first-frame form.

The file is read into a 16 KiB PSRAM window in chunks, and the card is only read while the peer is being waited on (for flow control) or when the next consecutive frame is far enough away (4 ms or more) that the read cannot delay it. On a peer whose STmin leaves no such gap the window runs dry once per 16 KiB and that one frame is late while it refills.

The command BLOCKS until the message is delivered, fails or the transfer limit (120 s) expires, with the CAN text stream muted.

###### Arguments

- `filePath` -- path of the file on the SD card, no spaces (e.g. `/isotp/tx.bin`). The card must be mounted by the device (not handed to the USB reader).

###### Returns

The same seven fields as `Send Message`: `result bytes frames durationUs minGapUs maxGapUs avgGapUs`. `result` `10` (SourceError) means the file could not be opened or read; `8` (InvalidLength) means it is empty.

###### Example

```
x /isotp/tx.bin
0 20000 2858 2903412 1004 1187 1011
```

###### Notes

- Reports success whenever the transfer could be attempted; read `result`. `Invalid` means the path was missing; `Failed` means the transport could not be armed (no PSRAM).
- The transport is armed automatically if it was not.
- Copy the file onto the card first (file-system menu upload, or the SD card in a reader), then hand the card back to the device.
- Requires power zone 15; the SD card zone must be up as well for the file to open.

Wire command: `i\c\t\x`

| Arg | Wire type |
|---|---|
| file_path | string |

Returns: result (decU32), bytes (decU32), frames (decU32), duration_us (decU32), min_gap_us (decU32), max_gap_us (decU32), avg_gap_us (decU32)

```python
dev.io.canfd.isotp.iso_tp_send_file(file_path: str) -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_send_file(ow_device* dev, const char* file_path, int32_t* result, int32_t* bytes, int32_t* frames, int32_t* duration_us, int32_t* min_gap_us, int32_t* max_gap_us, int32_t* avg_gap_us);
```
```rust
dev.io().canfd().isotp().iso_tp_send_file(file_path: &str) -> Result<(i32, i32, i32, i32, i32, i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_send_file(file_path)   # returns value; check dev.ok
```

## iso_tp_receive_message

Receive Message. Reports the last ISO-TP message received on rxId: status 0 none, 1 complete, 2 receiving, 3 error; length; inFile=1 when it was longer than 256 bytes and was written to the receive file (then data is empty); otherwise the payload bytes in hex. Reading consumes the message.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Receive Message

Pops the most recently received ISO-TP message. Messages addressed to `rxId` are accepted whenever the transport is armed (`Enable ISO-TP`, or after any send): the transport answers the sender's first frame with flow control and collects the consecutive frames on its own; this command only reads the result.

###### Returns

Fields in order, space-separated:

- `status` -- `0` nothing received since the last read; `1` a complete message is returned; `2` a multi-frame receive is in progress; `3` the last receive failed (timeout waiting for a consecutive frame, wrong sequence number, refused first frame, card write error or abort). `Show Status` (`i`) gives the failure code.
- `length` -- payload length in bytes when `status` is `1`, otherwise `0`.
- `inFile` -- `1` when the message was longer than 256 bytes and its payload was written to the receive file (`Set Receive File Path`, default `/isotp/rx.bin`); `0` when the payload follows inline.
- `data` -- the payload bytes in hex, space-separated; present only when `status` is `1` and `inFile` is `0`.

###### Wire layout

```
r
1 3 0 62 F1 90          # 3-byte message, inline

r
1 4096 1                # 4096-byte message, in /isotp/rx.bin

r
0 0 0                   # nothing waiting
```

###### Notes

- Reading a complete or failed message consumes it: the next call returns `0` until another message arrives. A new message replaces an unread one.
- A message longer than 256 bytes needs the SD card mounted by the device; otherwise its first frame is refused with flow control OVERFLOW and the sender sees an overflow error. The receive file is created fresh for every such message.
- Requires power zone 15.

Wire command: `i\c\t\r`

Returns: status (decU32), length (decU32), in_file (decU32), data (byteArray)

```python
dev.io.canfd.isotp.iso_tp_receive_message() -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_receive_message(ow_device* dev, int32_t* status, int32_t* length, int32_t* in_file, uint8_t* data, size_t data_cap, size_t* data_len);
```
```rust
dev.io().canfd().isotp().iso_tp_receive_message() -> Result<(i32, i32, i32, Vec<u8>), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_receive_message()   # returns value; check dev.ok
```

## iso_tp_set_receive_file_path

Set Receive File Path. Sets where received ISO-TP messages longer than 256 bytes are written on the SD card (default /isotp/rx.bin); the directory is created when the first such message arrives.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Set Receive File Path

Chooses the SD card file that receives the payload of any ISO-TP message longer than 256 bytes. Default: `/isotp/rx.bin`.

###### Arguments

- `filePath` -- path on the SD card, up to 63 characters, no spaces. Its directory (one level) is created on demand when a message arrives; the file is overwritten by every long message.

###### Returns

- `success` -- `1` when stored; `Invalid` for an empty or over-long path.

###### Example

```
p /isotp/dump.bin
```

###### Notes

- The setting is not persisted across a reboot.
- Requires power zone 15.

Wire command: `i\c\t\p`

| Arg | Wire type |
|---|---|
| file_path | string |

Returns: none (Ok/Err only)

```python
dev.io.canfd.isotp.iso_tp_set_receive_file_path(file_path: str) -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_set_receive_file_path(ow_device* dev, const char* file_path);
```
```rust
dev.io().canfd().isotp().iso_tp_set_receive_file_path(file_path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_set_receive_file_path(file_path)   # check dev.ok
```

## iso_tp_abort

Abort. Terminates whatever ISO-TP transfer is in progress without sending anything, closes any open card file and leaves the transport armed.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Abort

Stops the current ISO-TP transfer. No frame is sent: a peer that is mid-transfer will run into its own timeout. Any card file open for the transfer is closed (a partially received file stays on the card) and the transport remains armed.

Because sends and receives complete before the console accepts the next command, this is mostly useful for clearing a receive that was reported as in progress, or from a second host session.

###### Returns

- `success` -- always `1`.

###### Example

```
a
```

###### Notes

- Requires power zone 15.

Wire command: `i\c\t\a`

Returns: none (Ok/Err only)

```python
dev.io.canfd.isotp.iso_tp_abort() -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_abort(ow_device* dev);
```
```rust
dev.io().canfd().isotp().iso_tp_abort() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_abort()   # check dev.ok
```

## iso_tp_show_status

Show Status. Reports the transport state (0 idle, 1 waiting for flow control, 2 sending consecutive frames, 3 waiting for a frame to finish, 4 receiving), the last result code (0 = Ok), and the running counts of messages received, sent and failed since power-up.

Requires power zone 15 (CAN). See [Errors](errors.md).

##### Show Status

One line of transport state and counters.

###### Returns

- `state` -- `0` idle, `1` a first frame or block is out and the peer's flow control is awaited, `2` the next consecutive frame is scheduled, `3` a consecutive frame is in the controller awaiting its completion, `4` a multi-frame message is being received. Because transfers complete before the console answers, a host normally sees `0`.
- `lastResult` -- outcome of the last transfer or the last failure: `0` Ok, `1` Busy, `2` Aborted, `3` TimeoutBs, `4` TimeoutCr, `5` TimeoutAs, `6` WrongSequence, `7` Overflow, `8` InvalidLength, `9` SinkError, `10` SourceError, `11` TooManyWaits, `12` SendFailed.
- `rxCount` -- messages received completely since power-up.
- `txCount` -- messages sent completely since power-up.
- `errors` -- transfers that ended in any result other than Ok.

###### Example

```
i
0 0 12 7 1
```

###### Notes

- The menu header (shown when the submenu is entered on the console) also prints the configured ids, format and flow control values.
- Requires power zone 15.

Wire command: `i\c\t\i`

Returns: state (decU32), last_result (decU32), rx_count (decU32), tx_count (decU32), errors (decU32)

```python
dev.io.canfd.isotp.iso_tp_show_status() -> Result
```
```c
ow_status ow_io_canfd_isotp_iso_tp_show_status(ow_device* dev, int32_t* state, int32_t* last_result, int32_t* rx_count, int32_t* tx_count, int32_t* errors);
```
```rust
dev.io().canfd().isotp().iso_tp_show_status() -> Result<(i32, i32, i32, i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.canfd.isotp.iso_tp_show_status()   # returns value; check dev.ok
```
