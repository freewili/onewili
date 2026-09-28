# System Functions

`dev.hardware.system` - wire path `h\a` - generated from `fwMenuSystem`.

## enable_battery_stream

Stream Battery Info. Enables or disables streaming of battery info to the host.

Wire command: `h\a\o`

| Arg | Wire type |
|---|---|
| enable | decS32 |

Returns: none (Ok/Err only)

```python
dev.hardware.system.enable_battery_stream(enable: int) -> Result
```
```c
ow_status ow_hardware_system_enable_battery_stream(ow_device* dev, int32_t enable);
```
```rust
dev.hardware().system().enable_battery_stream(enable: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.system.enable_battery_stream(enable)   # check dev.ok
```

## read_otp_info

Read OTP Info. Reads bytes from the fused OTP identity blob (bl_otp_info v3). An unprovisioned device reads all zeros. Read in chunks of 256 bytes or less.

Wire command: `h\a\b`

| Arg | Wire type |
|---|---|
| offset | decS32 |
| length | decS32 |

Returns: otp_blob (hexbytes)

```python
dev.hardware.system.read_otp_info(offset: int, length: int) -> Result
```
```c
ow_status ow_hardware_system_read_otp_info(ow_device* dev, int32_t offset, int32_t length, uint8_t* otp_blob, size_t otp_blob_cap, size_t* otp_blob_len);
```
```rust
dev.hardware().system().read_otp_info(offset: i32, length: i32) -> Result<Vec<u8>, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.system.read_otp_info(offset, length)   # returns value; check dev.ok
```

## boot_uf2

Boot UF2. Reboots into the SBL bootloader, which chain-loads the named RAM-app UF2 from the SD card /update directory (card root as fallback). No response is sent on success — the device resets.

Wire command: `h\a\u`

| Arg | Wire type |
|---|---|
| filename | string |

Returns: none (Ok/Err only)

```python
dev.hardware.system.boot_uf2(filename: str) -> Result
```
```c
ow_status ow_hardware_system_boot_uf2(ow_device* dev, const char* filename);
```
```rust
dev.hardware().system().boot_uf2(filename: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.system.boot_uf2(filename)   # check dev.ok
```

## device_state

Device State. Report the device state for host sync: SD card host (none|main|usb), event host-streaming gate (0|1), active-stream mask (hex, bit index = event id), clk_sys in Hz. More space-separated fields may be appended later.

Wire command: `h\a\g`

Returns: sd (string), hoststream (bool), activemask (string), clksyshz (decU32)

```python
dev.hardware.system.device_state() -> Result
```
```c
ow_status ow_hardware_system_device_state(ow_device* dev, char* sd, size_t sd_cap, bool* hoststream, char* activemask, size_t activemask_cap, int32_t* clksyshz);
```
```rust
dev.hardware().system().device_state() -> Result<(String, bool, String, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.system.device_state()   # returns value; check dev.ok
```

## event_host_streaming

Event Host Streaming. Enables or disables streaming of events to the host. When disabled, stream-class events are suppressed at the host output; protocol events still flow. Same gate as control bytes 0x05 (off) and 0x06 (on).

Wire command: `h\a\e`

| Arg | Wire type |
|---|---|
| enable | decS32 |

Returns: enabled (bool)

```python
dev.hardware.system.event_host_streaming(enable: int) -> Result
```
```c
ow_status ow_hardware_system_event_host_streaming(ow_device* dev, int32_t enable, bool* enabled);
```
```rust
dev.hardware().system().event_host_streaming(enable: i32) -> Result<bool, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.system.event_host_streaming(enable)   # returns value; check dev.ok
```

## stream_write

Stream Write. Sends one peer-stream datagram (1-128 bytes) to another OneWili client through MAIN. Best effort: a datagram the destination cannot take now is dropped and counted, never queued behind.

#### Stream Write

Sends one peer-stream datagram to another OneWili client (DISPLAY, ESP32, CM0 or the PC host). MAIN routes it and stamps the sender as the client that issued this command.

- `dst` is the destination peer: 0 main (reserved, always dropped), 1 display, 2 esp32, 3 cm0, 4 host.
- `data` is 1-128 bytes. Longer datagrams are rejected, never split.
- `delivered` is 1 when the datagram left MAIN for the destination (or was queued for a polling client), 0 when it was dropped: the destination is not using streams right now, its link had no room, or its queue is full.

This is the text route of `ow_stream_write`; the DISPLAY and ESP32 have faster push links for the same datagrams. See also `p` (Stream Poll) and `c` (Stream Status).

Wire command: `h\a\w`

| Arg | Wire type |
|---|---|
| dst | decS32 |
| data | hexbytes |

Returns: delivered (bool)

```python
dev.hardware.system.stream_write(dst: int, data: bytes | bytearray) -> Result
```
```c
ow_status ow_hardware_system_stream_write(ow_device* dev, int32_t dst, const uint8_t* data, size_t data_len, bool* delivered);
```
```rust
dev.hardware().system().stream_write(dst: i32, data: &[u8]) -> Result<bool, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.system.stream_write(dst, data)   # returns value; check dev.ok
```

## stream_poll

Stream Poll. Pops peer-stream datagrams queued for the calling client: frames popped, frames still queued, frames dropped for this client so far, then the datagrams packed as [src][len][bytes] records.

#### Stream Poll

Pops the datagrams MAIN has queued for the calling client, oldest first, as many whole ones as fit in `max` bytes.

- `frames` is how many were popped (0 when none are waiting).
- `queued` is how many are still waiting.
- `dropped` is how many datagrams addressed to this client were dropped so far (queue full), free-running.
- `data` packs the popped datagrams as records: source peer (1 byte), length (1 byte), then that many bytes.

Only clients without a push link (the PC host and the CM0) have a queue; for the DISPLAY and ESP32 the datagrams are pushed on their own links and this returns none. This is the text route of `ow_stream_poll`. See also `w` (Stream Write) and `c` (Stream Status).

Wire command: `h\a\p`

| Arg | Wire type |
|---|---|
| max | decS32 |

Returns: frames (decS32), queued (decS32), dropped (decU32), data (hexbytes)

```python
dev.hardware.system.stream_poll(max: int) -> Result
```
```c
ow_status ow_hardware_system_stream_poll(ow_device* dev, int32_t max, int32_t* frames, int32_t* queued, int32_t* dropped, uint8_t* data, size_t data_cap, size_t* data_len);
```
```rust
dev.hardware().system().stream_poll(max: i32) -> Result<(i32, i32, i32, Vec<u8>), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.system.stream_poll(max)   # returns value; check dev.ok
```

## stream_status

Stream Status. Peer-stream state for the calling client: the datagram MTU, datagrams waiting in its queue, datagrams addressed to it that MAIN dropped, and datagrams it sent that MAIN dropped.

#### Stream Status

Reports the peer-stream counters MAIN keeps for the client that issues it.

- `mtu` is the largest datagram in bytes, the same on every link.
- `queued` is how many datagrams wait in this client's queue (always 0 for push-link clients).
- `droppedto` counts datagrams addressed to this client that MAIN dropped; `droppedfrom` counts datagrams this client sent that MAIN dropped. Both are free-running since MAIN booted.

`ow_stream_drops` on a text client is their sum. See also `w` (Stream Write) and `p` (Stream Poll).

Wire command: `h\a\c`

Returns: mtu (decS32), queued (decS32), droppedto (decU32), droppedfrom (decU32)

```python
dev.hardware.system.stream_status() -> Result
```
```c
ow_status ow_hardware_system_stream_status(ow_device* dev, int32_t* mtu, int32_t* queued, int32_t* droppedto, int32_t* droppedfrom);
```
```rust
dev.hardware().system().stream_status() -> Result<(i32, i32, i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.system.stream_status()   # returns value; check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `battery` (text)

Battery charger status text

| Payload field | Wire type |
|---|---|
| data | string |
