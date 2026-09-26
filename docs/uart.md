# UART Functions

`dev.io.uart` - wire path `i\u` - generated from `fwMenuUART`.

## u_art_write

Write. Writes data to a specific I2C Address

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\u\w`

| Arg | Wire type |
|---|---|
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.uart.u_art_write(data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_uart_u_art_write(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().uart().u_art_write(data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.u_art_write(data_bytes)   # check dev.ok
```

## toggle_stream

Enable UART Read Events. Reads the number from the address

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\u\r`

Returns: data_bytes (hexbytes)

```python
dev.io.uart.toggle_stream() -> Result
```
```c
ow_status ow_io_uart_toggle_stream(ow_device* dev, uint8_t* data_bytes, size_t data_bytes_cap, size_t* data_bytes_len);
```
```rust
dev.io().uart().toggle_stream() -> Result<Vec<u8>, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.toggle_stream()   # returns value; check dev.ok
```

## uart_enable_api_mode

Enable UART API mode. Tests all addresses for I2C Response

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\u\t`

Returns: none (Ok/Err only)

```python
dev.io.uart.uart_enable_api_mode() -> Result
```
```c
ow_status ow_io_uart_uart_enable_api_mode(ow_device* dev);
```
```rust
dev.io().uart().uart_enable_api_mode() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.uart_enable_api_mode()   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `uart1` (text)

uart receive frame

| Payload field | Wire type |
|---|---|
| data_bytes | hexbytes |

## Sub-menus

- [UART Settings](uart_settings.md) - `dev.io.uart.settings`
