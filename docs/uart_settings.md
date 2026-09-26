# UART Settings

`dev.io.uart.settings` - wire path `i\u\s` - generated from `fwMenuUARTSettings`.

## baud_rate

Baud Rate. UART baud rate in bits per second

Wire command: `i\u\s\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.uart.settings.baud_rate(value: int) -> Result
```
```c
ow_status ow_io_uart_settings_baud_rate(ow_device* dev, int32_t value);
```
```rust
dev.io().uart().settings().baud_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.settings.baud_rate(value)   # check dev.ok
```

## r_ts_hand_shaking

RTS Hand Shaking. Enable RTS hardware handshaking

Wire command: `i\u\s\r`

Returns: none (Ok/Err only)

```python
dev.io.uart.settings.r_ts_hand_shaking() -> Result
```
```c
ow_status ow_io_uart_settings_r_ts_hand_shaking(ow_device* dev);
```
```rust
dev.io().uart().settings().r_ts_hand_shaking() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.settings.r_ts_hand_shaking()   # check dev.ok
```

## c_ts_hand_shaking

CTS Hand Shaking. Enable CTS hardware handshaking

Wire command: `i\u\s\c`

Returns: none (Ok/Err only)

```python
dev.io.uart.settings.c_ts_hand_shaking() -> Result
```
```c
ow_status ow_io_uart_settings_c_ts_hand_shaking(ow_device* dev);
```
```rust
dev.io().uart().settings().c_ts_hand_shaking() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.settings.c_ts_hand_shaking()   # check dev.ok
```

## data_bits

Data Bits. UART data bits

Wire command: `i\u\s\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.uart.settings.data_bits(value: int) -> Result
```
```c
ow_status ow_io_uart_settings_data_bits(ow_device* dev, int32_t value);
```
```rust
dev.io().uart().settings().data_bits(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.settings.data_bits(value)   # check dev.ok
```

## parity

Parity. UART parity mode

Wire command: `i\u\s\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.uart.settings.parity(value: int) -> Result
```
```c
ow_status ow_io_uart_settings_parity(ow_device* dev, int32_t value);
```
```rust
dev.io().uart().settings().parity(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.settings.parity(value)   # check dev.ok
```

## stop_bits

Stop Bits. UART stop bits

Wire command: `i\u\s\s`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.uart.settings.stop_bits(value: int) -> Result
```
```c
ow_status ow_io_uart_settings_stop_bits(ow_device* dev, int32_t value);
```
```rust
dev.io().uart().settings().stop_bits(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.settings.stop_bits(value)   # check dev.ok
```

## module

Module. Which UART module handles the port

Wire command: `i\u\s\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.uart.settings.module(value: int) -> Result
```
```c
ow_status ow_io_uart_settings_module(ow_device* dev, int32_t value);
```
```rust
dev.io().uart().settings().module(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.uart.settings.module(value)   # check dev.ok
```
