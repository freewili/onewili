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

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `battery` (text)

Battery charger status text

| Payload field | Wire type |
|---|---|
| data | string |
