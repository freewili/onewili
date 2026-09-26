# Analog In Functions

`dev.io.analog_in` - wire path `i\j` - generated from `fwMenuAnalogIn`.

## enable_analog_in_stream

Stream Analog In. Streams analog input values to the host at the given rate.

Requires power zone 11 (Analog). See [Errors](errors.md).

Wire command: `i\j\s`

| Arg | Wire type |
|---|---|
| stream_rate_ms | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.analog_in.enable_analog_in_stream(stream_rate_ms: int) -> Result
```
```c
ow_status ow_io_analog_in_enable_analog_in_stream(ow_device* dev, int32_t stream_rate_ms);
```
```rust
dev.io().analog_in().enable_analog_in_stream(stream_rate_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_in.enable_analog_in_stream(stream_rate_ms)   # check dev.ok
```

## read_analog_in2024

Read TLA2024. Reads the latest TLA2024 voltages for all 4 channels.

Requires power zone 11 (Analog). See [Errors](errors.md).

Wire command: `i\j\r`

Returns: v0 (float), v1 (float), v2 (float), v3 (float)

```python
dev.io.analog_in.read_analog_in2024() -> Result
```
```c
ow_status ow_io_analog_in_read_analog_in2024(ow_device* dev, double* v0, double* v1, double* v2, double* v3);
```
```rust
dev.io().analog_in().read_analog_in2024() -> Result<(f64, f64, f64, f64), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_in.read_analog_in2024()   # returns value; check dev.ok
```

## config_analog_in2024

Config TLA2024 Channel. Configures a TLA2024 channel: mux 0-7 = A0-A1,A0-A3,A1-A3,A2-A3,A0-GND,A1-GND,A2-GND,A3-GND; range 0-5 = 6.144V,4.096V,2.048V,1.024V,0.512V,0.256V.

Requires power zone 11 (Analog). See [Errors](errors.md).

Wire command: `i\j\c`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| mux | decS32 |
| range | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.analog_in.config_analog_in2024(channel: int, mux: int, range: int) -> Result
```
```c
ow_status ow_io_analog_in_config_analog_in2024(ow_device* dev, int32_t channel, int32_t mux, int32_t range);
```
```rust
dev.io().analog_in().config_analog_in2024(channel: i32, mux: i32, range: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_in.config_analog_in2024(channel, mux, range)   # check dev.ok
```

## set_data_rate2024

TLA2024 Data Rate. Sets the TLA2024 data rate: 0-6 = 128,250,490,920,1600,2400,3300 SPS.

Requires power zone 11 (Analog). See [Errors](errors.md).

Wire command: `i\j\f`

| Arg | Wire type |
|---|---|
| rate | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.analog_in.set_data_rate2024(rate: int) -> Result
```
```c
ow_status ow_io_analog_in_set_data_rate2024(ow_device* dev, int32_t rate);
```
```rust
dev.io().analog_in().set_data_rate2024(rate: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_in.set_data_rate2024(rate)   # check dev.ok
```

## enable_analog_in2024_stream

Stream TLA2024. Streams TLA2024 voltages to the host at the given rate (0 stops).

Requires power zone 11 (Analog). See [Errors](errors.md).

Wire command: `i\j\t`

| Arg | Wire type |
|---|---|
| stream_rate_ms | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.analog_in.enable_analog_in2024_stream(stream_rate_ms: int) -> Result
```
```c
ow_status ow_io_analog_in_enable_analog_in2024_stream(ow_device* dev, int32_t stream_rate_ms);
```
```rust
dev.io().analog_in().enable_analog_in2024_stream(stream_rate_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_in.enable_analog_in2024_stream(stream_rate_ms)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `ainIn` (text)

Internal ADC voltages (connector channels 0-3)

| Payload field | Wire type |
|---|---|
| v0 | float |
| v1 | float |
| v2 | float |
| v3 | float |

### `adcIn` (text)

TLA2024 voltages (connector channels 0-3)

| Payload field | Wire type |
|---|---|
| v0 | float |
| v1 | float |
| v2 | float |
| v3 | float |
