# Logger

`dev.logger` - wire path `r` - generated from `fwMenuLogger`.

## start

Start. Arms the logger with the current settings; Immediate trigger mode starts capturing at once. Emits logger events (armed/triggered/complete/error) as it runs.

Allocates the PSRAM capture arena and arms the trigger state machine.
Immediate mode begins logging right away; Button/Expression modes wait for their trigger.
Files are written under /logs on the SD card as logNNNN.csv / logNNNN.rtix.

Wire command: `r\s`

Returns: none (Ok/Err only)

```python
dev.logger.start() -> Result
```
```c
ow_status ow_logger_start(ow_device* dev);
```
```rust
dev.logger().start() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.start()   # check dev.ok
```

## stop

Stop. Stops the logger: an armed capture is discarded, a running capture drains its remaining events to the files and closes them.

Wire command: `r\e`

Returns: none (Ok/Err only)

```python
dev.logger.stop() -> Result
```
```c
ow_status ow_logger_stop(ow_device* dev);
```
```rust
dev.logger().stop() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.stop()   # check dev.ok
```

## trigger

Trigger. Software trigger: fires an armed capture regardless of the configured trigger mode.

Wire command: `r\t`

Returns: none (Ok/Err only)

```python
dev.logger.trigger() -> Result
```
```c
ow_status ow_logger_trigger(ow_device* dev);
```
```rust
dev.logger().trigger() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.trigger()   # check dev.ok
```

## status

Status. Prints the logger state, file format, trigger mode, output file names and event counters.

Wire command: `r\i`

Returns: none (Ok/Err only)

```python
dev.logger.status() -> Result
```
```c
ow_status ow_logger_status(ow_device* dev);
```
```rust
dev.logger().status() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.status()   # check dev.ok
```

## file_format

File Format. Output file format for the next capture: CSV text, RTIX binary, or both

Wire command: `r\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.file_format(value: int) -> Result
```
```c
ow_status ow_logger_file_format(ow_device* dev, int32_t value);
```
```rust
dev.logger().file_format(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.file_format(value)   # check dev.ok
```

## trigger_mode

Trigger Mode. How an armed capture is triggered: Immediate (on start), Button (a device button press), or Expression (a device expression becoming nonzero)

Wire command: `r\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.trigger_mode(value: int) -> Result
```
```c
ow_status ow_logger_trigger_mode(ow_device* dev, int32_t value);
```
```rust
dev.logger().trigger_mode(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.trigger_mode(value)   # check dev.ok
```

## trigger_button

Trigger Button. Device button that fires the trigger in Button mode

Wire command: `r\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.trigger_button(value: int) -> Result
```
```c
ow_status ow_logger_trigger_button(ow_device* dev, int32_t value);
```
```rust
dev.logger().trigger_button(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.trigger_button(value)   # check dev.ok
```

## trigger_expression

Trigger Expression. Expression evaluated every 50 ms in Expression mode; the trigger fires when it evaluates nonzero

Wire command: `r\x`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.trigger_expression(value: str) -> Result
```
```c
ow_status ow_logger_trigger_expression(ow_device* dev, const char* value);
```
```rust
dev.logger().trigger_expression(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.trigger_expression(value)   # check dev.ok
```

## pre_trigger_ms

Pre Trigger Ms. Milliseconds of events kept from before the trigger (0-60000)

Wire command: `r\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.pre_trigger_ms(value: int) -> Result
```
```c
ow_status ow_logger_pre_trigger_ms(ow_device* dev, int32_t value);
```
```rust
dev.logger().pre_trigger_ms(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.pre_trigger_ms(value)   # check dev.ok
```

## post_trigger_ms

Post Trigger Ms. Milliseconds captured after the trigger before the files close (0 = until stop, max 600000)

Wire command: `r\o`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.post_trigger_ms(value: int) -> Result
```
```c
ow_status ow_logger_post_trigger_ms(ow_device* dev, int32_t value);
```
```rust
dev.logger().post_trigger_ms(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.post_trigger_ms(value)   # check dev.ok
```

## events

Events. Selects which events this instance captures: "all", "none", a comma-separated event-name list, or +name/-name to add/remove one event from the current selection

Wire command: `r\v`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.events(value: str) -> Result
```
```c
ow_status ow_logger_events(ow_device* dev, const char* value);
```
```rust
dev.logger().events(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.events(value)   # check dev.ok
```

## active_instance

Active Instance. Selects which of the four logger instances (0-3) the settings rows show and the start, stop and trigger commands act on; every instance keeps its own saved configuration

Wire command: `r\n`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.active_instance(value: int) -> Result
```
```c
ow_status ow_logger_active_instance(ow_device* dev, int32_t value);
```
```rust
dev.logger().active_instance(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.active_instance(value)   # check dev.ok
```

## name

Name. Optional name for this instance; captures are written to /logs/<name>/<name>_NNNN.* instead of /logs/logI_NNNN.*

Wire command: `r\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.logger.name(value: str) -> Result
```
```c
ow_status ow_logger_name(ow_device* dev, const char* value);
```
```rust
dev.logger().name(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.logger.name(value)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `logger` (text)

Logger state change, prefixed with the instance number 0-3: <inst> armed, <inst> triggered, <inst> complete <csv> <rtix> <n> records, or <inst> error <reason>

| Payload field | Wire type |
|---|---|
| info | string |
