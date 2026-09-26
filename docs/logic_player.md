# Logic Player Functions

`dev.io.logic_player` - wire path `i\p` - generated from `fwMenuLogicPlayer`.

## setup_player

configure. Configures digital playback

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\p\c`

| Arg | Wire type |
|---|---|
| sample_rate_ns | decU32 |
| sample_count | decS32 |
| pin_start | decS32 |
| pin_stop | decS32 |
| start_mode | decS32 |
| trigger_pin | decS32 |
| loop | bool |

Returns: none (Ok/Err only)

```python
dev.io.logic_player.setup_player(sample_rate_ns: int, sample_count: int, pin_start: int, pin_stop: int, start_mode: int, trigger_pin: int, loop: bool) -> Result
```
```c
ow_status ow_io_logic_player_setup_player(ow_device* dev, int32_t sample_rate_ns, int32_t sample_count, int32_t pin_start, int32_t pin_stop, int32_t start_mode, int32_t trigger_pin, bool loop);
```
```rust
dev.io().logic_player().setup_player(sample_rate_ns: i32, sample_count: i32, pin_start: i32, pin_stop: i32, start_mode: i32, trigger_pin: i32, loop_: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_player.setup_player(sample_rate_ns, sample_count, pin_start, pin_stop, start_mode, trigger_pin, loop)   # check dev.ok
```

## setup_analog

configure analog. Configures DAC playback

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\p\a`

| Arg | Wire type |
|---|---|
| mask | decU32 |
| analog_rate_ns | decS32 |
| analog_resolution | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.logic_player.setup_analog(mask: int, analog_rate_ns: int, analog_resolution: int) -> Result
```
```c
ow_status ow_io_logic_player_setup_analog(ow_device* dev, int32_t mask, int32_t analog_rate_ns, int32_t analog_resolution);
```
```rust
dev.io().logic_player().setup_analog(mask: i32, analog_rate_ns: i32, analog_resolution: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_player.setup_analog(mask, analog_rate_ns, analog_resolution)   # check dev.ok
```

## load_file

load. Loads a raw buffer from the filesystem

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\p\l`

| Arg | Wire type |
|---|---|
| file_path | string |

Returns: none (Ok/Err only)

```python
dev.io.logic_player.load_file(file_path: str) -> Result
```
```c
ow_status ow_io_logic_player_load_file(ow_device* dev, const char* file_path);
```
```rust
dev.io().logic_player().load_file(file_path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_player.load_file(file_path)   # check dev.ok
```

## start

start. Starts playback

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\p\s`

Returns: none (Ok/Err only)

```python
dev.io.logic_player.start() -> Result
```
```c
ow_status ow_io_logic_player_start(ow_device* dev);
```
```rust
dev.io().logic_player().start() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_player.start()   # check dev.ok
```

## stop

stop. Stops playback

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\p\e`

Returns: none (Ok/Err only)

```python
dev.io.logic_player.stop() -> Result
```
```c
ow_status ow_io_logic_player_stop(ow_device* dev);
```
```rust
dev.io().logic_player().stop() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_player.stop()   # check dev.ok
```
