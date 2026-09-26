# Sensor Functions

`dev.io.sensors` - wire path `i\s` - generated from `fwMenuSensors`.

## enable_motion_stream

Stream Motion. Streams accelerometer and gyroscope data to the host at the given rate. 0 stops the stream.

Requires power zone 1 (Sensors). See [Errors](errors.md).

Wire command: `i\s\m`

| Arg | Wire type |
|---|---|
| stream_rate_ms | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.sensors.enable_motion_stream(stream_rate_ms: int) -> Result
```
```c
ow_status ow_io_sensors_enable_motion_stream(ow_device* dev, int32_t stream_rate_ms);
```
```rust
dev.io().sensors().enable_motion_stream(stream_rate_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.sensors.enable_motion_stream(stream_rate_ms)   # check dev.ok
```

## enable_field_stream

Stream Field. Streams magnetometer data to the host at the given rate. 0 stops the stream.

Requires power zone 1 (Sensors). See [Errors](errors.md).

Wire command: `i\s\f`

| Arg | Wire type |
|---|---|
| stream_rate_ms | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.sensors.enable_field_stream(stream_rate_ms: int) -> Result
```
```c
ow_status ow_io_sensors_enable_field_stream(ow_device* dev, int32_t stream_rate_ms);
```
```rust
dev.io().sensors().enable_field_stream(stream_rate_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.sensors.enable_field_stream(stream_rate_ms)   # check dev.ok
```

## enable_env_stream

Stream Env. Streams temperature, humidity and ambient light to the host. This stream is change-driven: the rate is a heartbeat floor, so samples can arrive faster when readings move. 0 stops the stream.

Requires power zone 1 (Sensors). See [Errors](errors.md).

Wire command: `i\s\e`

| Arg | Wire type |
|---|---|
| stream_rate_ms | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.sensors.enable_env_stream(stream_rate_ms: int) -> Result
```
```c
ow_status ow_io_sensors_enable_env_stream(ow_device* dev, int32_t stream_rate_ms);
```
```rust
dev.io().sensors().enable_env_stream(stream_rate_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.sensors.enable_env_stream(stream_rate_ms)   # check dev.ok
```

## enable_orientation_stream

Stream Orientation. Streams fused roll, pitch, yaw and heading to the host at the given rate. 0 stops the stream.

Requires power zone 1 (Sensors). See [Errors](errors.md).

Wire command: `i\s\r`

| Arg | Wire type |
|---|---|
| stream_rate_ms | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.sensors.enable_orientation_stream(stream_rate_ms: int) -> Result
```
```c
ow_status ow_io_sensors_enable_orientation_stream(ow_device* dev, int32_t stream_rate_ms);
```
```rust
dev.io().sensors().enable_orientation_stream(stream_rate_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.sensors.enable_orientation_stream(stream_rate_ms)   # check dev.ok
```

## get_sensors

Get Sensors. Prints the most recent sample from each of the four sensor groups.

Requires power zone 1 (Sensors). See [Errors](errors.md).

Wire command: `i\s\g`

Returns: none (Ok/Err only)

```python
dev.io.sensors.get_sensors() -> Result
```
```c
ow_status ow_io_sensors_get_sensors(ow_device* dev);
```
```rust
dev.io().sensors().get_sensors() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.sensors.get_sensors()   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `motion` (text)

Accelerometer and gyroscope data

| Payload field | Wire type |
|---|---|
| ax_mg | decS32 |
| ay_mg | decS32 |
| az_mg | decS32 |
| gx_ddps | decS32 |
| gy_ddps | decS32 |
| gz_ddps | decS32 |

### `field` (text)

Magnetometer data

| Payload field | Wire type |
|---|---|
| mx_dut | decS32 |
| my_dut | decS32 |
| mz_dut | decS32 |
| magnitude_dut | decS32 |
| heading_cdeg | decS32 |

### `env` (text)

Temperature, humidity and ambient light

| Payload field | Wire type |
|---|---|
| temp_cc | decS32 |
| rh_cpct | decS32 |
| lux_clux | decU32 |

### `orientation` (text)

Fused roll, pitch, yaw and heading

| Payload field | Wire type |
|---|---|
| roll_cdeg | decS32 |
| pitch_cdeg | decS32 |
| yaw_cdeg | decS32 |
| heading_cdeg | decS32 |
| flags | decS32 |
