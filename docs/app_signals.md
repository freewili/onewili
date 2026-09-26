# App Signals

`dev.scripting.app_signals` - wire path `s\i` - generated from `fwMenuAppSignals`.

## app_signal_add

Add. Adds an app signal.

Wire command: `s\i\a`

| Arg | Wire type |
|---|---|
| name | string |

Returns: none (Ok/Err only)

```python
dev.scripting.app_signals.app_signal_add(name: str) -> Result
```
```c
ow_status ow_scripting_app_signals_app_signal_add(ow_device* dev, const char* name);
```
```rust
dev.scripting().app_signals().app_signal_add(name: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.app_signals.app_signal_add(name)   # check dev.ok
```

## app_signal_remove

Remove. Removes an app signal.

Wire command: `s\i\x`

| Arg | Wire type |
|---|---|
| name | string |

Returns: none (Ok/Err only)

```python
dev.scripting.app_signals.app_signal_remove(name: str) -> Result
```
```c
ow_status ow_scripting_app_signals_app_signal_remove(ow_device* dev, const char* name);
```
```rust
dev.scripting().app_signals().app_signal_remove(name: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.app_signals.app_signal_remove(name)   # check dev.ok
```

## app_signal_rename

Rename. Renames an app signal.

Wire command: `s\i\r`

| Arg | Wire type |
|---|---|
| name | string |
| new_name | string |

Returns: none (Ok/Err only)

```python
dev.scripting.app_signals.app_signal_rename(name: str, new_name: str) -> Result
```
```c
ow_status ow_scripting_app_signals_app_signal_rename(ow_device* dev, const char* name, const char* new_name);
```
```rust
dev.scripting().app_signals().app_signal_rename(name: &str, new_name: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.app_signals.app_signal_rename(name, new_name)   # check dev.ok
```

## app_signal_set

Set Value. Sets an app signal value.

Wire command: `s\i\s`

| Arg | Wire type |
|---|---|
| name | string |
| value | float |

Returns: none (Ok/Err only)

```python
dev.scripting.app_signals.app_signal_set(name: str, value: float) -> Result
```
```c
ow_status ow_scripting_app_signals_app_signal_set(ow_device* dev, const char* name, double value);
```
```rust
dev.scripting().app_signals().app_signal_set(name: &str, value: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.app_signals.app_signal_set(name, value)   # check dev.ok
```

## app_signal_get

Get Value. Gets an app signal value.

Wire command: `s\i\g`

| Arg | Wire type |
|---|---|
| name | string |

Returns: name (string), value (float)

```python
dev.scripting.app_signals.app_signal_get(name: str) -> Result
```
```c
ow_status ow_scripting_app_signals_app_signal_get(ow_device* dev, const char* name, char* name_out, size_t name_out_cap, double* value);
```
```rust
dev.scripting().app_signals().app_signal_get(name: &str) -> Result<(String, f64), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.app_signals.app_signal_get(name)   # returns value; check dev.ok
```

## app_signal_wave

Apply Wave. Applies wave mode 0-8 (0 off; sine, triangle, square and saw at 0.5/2 Hz).

Wire command: `s\i\w`

| Arg | Wire type |
|---|---|
| name | string |
| wave | decS32 |

Returns: none (Ok/Err only)

```python
dev.scripting.app_signals.app_signal_wave(name: str, wave: int) -> Result
```
```c
ow_status ow_scripting_app_signals_app_signal_wave(ow_device* dev, const char* name, int32_t wave);
```
```rust
dev.scripting().app_signals().app_signal_wave(name: &str, wave: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.app_signals.app_signal_wave(name, wave)   # check dev.ok
```

## app_signal_stream

Stream. Streams every defined app signal; 0 disables streaming.

Wire command: `s\i\t`

| Arg | Wire type |
|---|---|
| stream_rate_ms | decS32 |

Returns: none (Ok/Err only)

```python
dev.scripting.app_signals.app_signal_stream(stream_rate_ms: int) -> Result
```
```c
ow_status ow_scripting_app_signals_app_signal_stream(ow_device* dev, int32_t stream_rate_ms);
```
```rust
dev.scripting().app_signals().app_signal_stream(stream_rate_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.app_signals.app_signal_stream(stream_rate_ms)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `appSignal` (text)

Streamed app-signal value.

| Payload field | Wire type |
|---|---|
| name | string |
| value | float |
