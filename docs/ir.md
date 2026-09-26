# IR Functions

`dev.wireless.ir` - wire path `w\i` - generated from `fwMenuIR`.

## enable_ir_stream

Stream IR. Enables or disables streaming of received IR codes to the host.

Wire command: `w\i\o`

| Arg | Wire type |
|---|---|
| enable | decS32 |

Returns: none (Ok/Err only)

```python
dev.wireless.ir.enable_ir_stream(enable: int) -> Result
```
```c
ow_status ow_wireless_ir_enable_ir_stream(ow_device* dev, int32_t enable);
```
```rust
dev.wireless().ir().enable_ir_stream(enable: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.enable_ir_stream(enable)   # check dev.ok
```

## send_ir_data

Send IR. Transmits a 4-byte IR code.

Wire command: `w\i\a`

| Arg | Wire type |
|---|---|
| ir_code | decU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.ir.send_ir_data(ir_code: int) -> Result
```
```c
ow_status ow_wireless_ir_send_ir_data(ow_device* dev, int32_t ir_code);
```
```rust
dev.wireless().ir().send_ir_data(ir_code: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.send_ir_data(ir_code)   # check dev.ok
```

## ir_self_test

IR Self Test. Transmits one frame per supported protocol and checks that the on-board receiver decodes each one back. Takes a few seconds and emits infrared.

Wire command: `w\i\t`

Returns: none (Ok/Err only)

```python
dev.wireless.ir.ir_self_test() -> Result
```
```c
ow_status ow_wireless_ir_ir_self_test(ow_device* dev);
```
```rust
dev.wireless().ir().ir_self_test() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.ir_self_test()   # check dev.ok
```

## ir_list_dir

List IR Dir. Lists the directories and .ir files on the SD card, directories first. Empty path lists \ir\.

Wire command: `w\i\l`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.ir.ir_list_dir(path: str) -> Result
```
```c
ow_status ow_wireless_ir_ir_list_dir(ow_device* dev, const char* path);
```
```rust
dev.wireless().ir().ir_list_dir(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.ir_list_dir(path)   # check dev.ok
```

## ir_list_buttons

List IR Buttons. Lists the buttons in one Flipper .ir file with the index each one is sent by. Malformed entries are counted as skipped, not listed.

Wire command: `w\i\b`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.ir.ir_list_buttons(path: str) -> Result
```
```c
ow_status ow_wireless_ir_ir_list_buttons(ow_device* dev, const char* path);
```
```rust
dev.wireless().ir().ir_list_buttons(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.ir_list_buttons(path)   # check dev.ok
```

## ir_send_button

Send IR Button. Transmits one button from a .ir file, repeated by the IR Repeat setting. Emits infrared.

Wire command: `w\i\s`

| Arg | Wire type |
|---|---|
| index | decU32 |
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.ir.ir_send_button(index: int, path: str) -> Result
```
```c
ow_status ow_wireless_ir_ir_send_button(ow_device* dev, int32_t index, const char* path);
```
```rust
dev.wireless().ir().ir_send_button(index: i32, path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.ir_send_button(index, path)   # check dev.ok
```

## ir_save_capture

Save IR Capture. Appends the last received signal to \ir\learned.ir under this name, decoded when the protocol was recognised and as raw timings when it was not.

Wire command: `w\i\c`

| Arg | Wire type |
|---|---|
| name | string |

Returns: none (Ok/Err only)

```python
dev.wireless.ir.ir_save_capture(name: str) -> Result
```
```c
ow_status ow_wireless_ir_ir_save_capture(ow_device* dev, const char* name);
```
```rust
dev.wireless().ir().ir_save_capture(name: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.ir_save_capture(name)   # check dev.ok
```

## ir_status

IR Status. Reports the IR engine's carrier, repeat count, capture overruns and whether the \ir\ tree exists on the card.

Wire command: `w\i\i`

Returns: none (Ok/Err only)

```python
dev.wireless.ir.ir_status() -> Result
```
```c
ow_status ow_wireless_ir_ir_status(ow_device* dev);
```
```rust
dev.wireless().ir().ir_status() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.ir_status()   # check dev.ok
```

## i_r_carrier

IR Carrier. Default transmit carrier frequency. Only these four are legal; a .ir raw entry with its own frequency line overrides this for that entry.

Wire command: `w\i\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.ir.i_r_carrier(value: int) -> Result
```
```c
ow_status ow_wireless_ir_i_r_carrier(ow_device* dev, int32_t value);
```
```rust
dev.wireless().ir().i_r_carrier(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.i_r_carrier(value)   # check dev.ok
```

## i_r_repeat

IR Repeat. How many times Send IR Button transmits each frame, 1 to 5, with a 40 ms gap between repeats.

Wire command: `w\i\r`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.ir.i_r_repeat(value: int) -> Result
```
```c
ow_status ow_wireless_ir_i_r_repeat(ow_device* dev, int32_t value);
```
```rust
dev.wireless().ir().i_r_repeat(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.ir.i_r_repeat(value)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `irrx` (text)

Received IR code

| Payload field | Wire type |
|---|---|
| code | hexU32 |
