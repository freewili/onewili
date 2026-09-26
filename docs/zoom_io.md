# ZoomIO Functions

`dev.scripting.zoom_io` - wire path `s\b` - generated from `fwMenuZoomIO`.

## enable_rx_stream

Stream ZoomIO Data. Enables or disables streaming of ZoomIO receive data to the host.

Requires power zone 6 (FPGA). See [Errors](errors.md).

#### Stream ZoomIO Data

Enable or disable forwarding of bytes received on the ZoomIO RX FIFO to the host.

##### Argument

- `enable` (`decS32`) — `1` to start streaming, `0` to stop.

##### Behavior

While streaming is enabled, the menu's `processEvents` loop drains up to 64 bytes per pass from the ZoomIO RX FIFO and emits them to the host as:

- A console event named `zoomio` with the payload as space‑separated hex bytes.
- A `CAN_API_RX_ZOOM_IO` response (binary), when CAN API streaming is also active.

Packet framing is preserved: the first byte of each emitted record is the packet length, followed by the packet bytes. A new packet boundary is detected from the FIFO's first‑byte‑of‑packet flag.

##### Examples

```
o 1   # start streaming RX data to host
o 0   # stop streaming
```

##### Returns

- `success=basic` — `true` if the argument parsed successfully, `false` on invalid input.

##### Notes

- Streaming only affects RX delivery to the host; it does not change ZoomIO transmit behavior or the schedule table.
- Pair with `w` (Write to FIFO), `u` (Update Schedule Table), and `p` (Setup Schedule Table) to drive traffic while observing responses.

Wire command: `s\b\o`

| Arg | Wire type |
|---|---|
| enable | decS32 |

Returns: none (Ok/Err only)

```python
dev.scripting.zoom_io.enable_rx_stream(enable: int) -> Result
```
```c
ow_status ow_scripting_zoom_io_enable_rx_stream(ow_device* dev, int32_t enable);
```
```rust
dev.scripting().zoom_io().enable_rx_stream(enable: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.zoom_io.enable_rx_stream(enable)   # check dev.ok
```

## send_data

Write to FIFO. Sends a single ZoomIO message after the given delay (us).

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `s\b\w`

| Arg | Wire type |
|---|---|
| delay | decS32 |
| data | byteArray |

Returns: none (Ok/Err only)

```python
dev.scripting.zoom_io.send_data(delay: int, data: bytes | bytearray) -> Result
```
```c
ow_status ow_scripting_zoom_io_send_data(ow_device* dev, int32_t delay, const uint8_t* data, size_t data_len);
```
```rust
dev.scripting().zoom_io().send_data(delay: i32, data: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.zoom_io.send_data(delay, data)   # check dev.ok
```

## update_table_data

Update Schedule Table. Updates a schedule-table transmit message.

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `s\b\u`

| Arg | Wire type |
|---|---|
| table_index | decS32 |
| delay | decS32 |
| data | byteArray |

Returns: none (Ok/Err only)

```python
dev.scripting.zoom_io.update_table_data(table_index: int, delay: int, data: bytes | bytearray) -> Result
```
```c
ow_status ow_scripting_zoom_io_update_table_data(ow_device* dev, int32_t table_index, int32_t delay, const uint8_t* data, size_t data_len);
```
```rust
dev.scripting().zoom_io().update_table_data(table_index: i32, delay: i32, data: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.zoom_io.update_table_data(table_index, delay, data)   # check dev.ok
```

## enable_schedule_table

Setup Schedule Table. Sets up the schedule table size (0 to disable).

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `s\b\p`

| Arg | Wire type |
|---|---|
| number_of_entries | decS32 |

Returns: none (Ok/Err only)

```python
dev.scripting.zoom_io.enable_schedule_table(number_of_entries: int) -> Result
```
```c
ow_status ow_scripting_zoom_io_enable_schedule_table(ow_device* dev, int32_t number_of_entries);
```
```rust
dev.scripting().zoom_io().enable_schedule_table(number_of_entries: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.zoom_io.enable_schedule_table(number_of_entries)   # check dev.ok
```

## compile_test

Compile test. Compiles built-in ZoomIO milestone program and launches it on core1 as RISC-V

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `s\b\c`

Returns: none (Ok/Err only)

```python
dev.scripting.zoom_io.compile_test() -> Result
```
```c
ow_status ow_scripting_zoom_io_compile_test(ow_device* dev);
```
```rust
dev.scripting().zoom_io().compile_test() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.zoom_io.compile_test()   # check dev.ok
```

## run_zio

Run ZoomIO. Compile and run a ZoomIO program on the RISC-V core1

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `s\b\r`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.scripting.zoom_io.run_zio(path: str) -> Result
```
```c
ow_status ow_scripting_zoom_io_run_zio(ow_device* dev, const char* path);
```
```rust
dev.scripting().zoom_io().run_zio(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.zoom_io.run_zio(path)   # check dev.ok
```

## stop_zio

Stop ZoomIO. Reset core1 to stop the running program

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `s\b\s`

Returns: none (Ok/Err only)

```python
dev.scripting.zoom_io.stop_zio() -> Result
```
```c
ow_status ow_scripting_zoom_io_stop_zio(ow_device* dev);
```
```rust
dev.scripting().zoom_io().stop_zio() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.zoom_io.stop_zio()   # check dev.ok
```

## exec_probe

Probe Exec Window. Stages a known pattern in ZoomIO's SCRATCH_X exec window, runs the full core1 launch sequence, and reads it back

Wire command: `s\b\x`

Returns: none (Ok/Err only)

```python
dev.scripting.zoom_io.exec_probe() -> Result
```
```c
ow_status ow_scripting_zoom_io_exec_probe(ow_device* dev);
```
```rust
dev.scripting().zoom_io().exec_probe() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.zoom_io.exec_probe()   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `zoomio` (text)

ZoomIO received packet (hex bytes)

| Payload field | Wire type |
|---|---|
| data_bytes | hexbytes |
