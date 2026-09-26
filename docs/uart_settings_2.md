# UART Settings

`dev.hardware.settings_home.uart_settings` - wire path `h\s\u` - generated from `fwMenuUARTSettings`.

## baud_rate

Baud Rate. UART baud rate in bits per second

Wire command: `h\s\u\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.uart_settings.baud_rate(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_uart_settings_baud_rate(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().uart_settings().baud_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.uart_settings.baud_rate(value)   # check dev.ok
```

## r_ts_hand_shaking

RTS Hand Shaking. Enable RTS hardware handshaking

Wire command: `h\s\u\r`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.uart_settings.r_ts_hand_shaking() -> Result
```
```c
ow_status ow_hardware_settings_home_uart_settings_r_ts_hand_shaking(ow_device* dev);
```
```rust
dev.hardware().settings_home().uart_settings().r_ts_hand_shaking() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.uart_settings.r_ts_hand_shaking()   # check dev.ok
```

## c_ts_hand_shaking

CTS Hand Shaking. Enable CTS hardware handshaking

Wire command: `h\s\u\c`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.uart_settings.c_ts_hand_shaking() -> Result
```
```c
ow_status ow_hardware_settings_home_uart_settings_c_ts_hand_shaking(ow_device* dev);
```
```rust
dev.hardware().settings_home().uart_settings().c_ts_hand_shaking() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.uart_settings.c_ts_hand_shaking()   # check dev.ok
```

## data_bits

Data Bits. UART data bits

Wire command: `h\s\u\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.uart_settings.data_bits(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_uart_settings_data_bits(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().uart_settings().data_bits(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.uart_settings.data_bits(value)   # check dev.ok
```

## parity

Parity. UART parity mode

Wire command: `h\s\u\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.uart_settings.parity(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_uart_settings_parity(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().uart_settings().parity(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.uart_settings.parity(value)   # check dev.ok
```

## stop_bits

Stop Bits. UART stop bits

Wire command: `h\s\u\s`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.uart_settings.stop_bits(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_uart_settings_stop_bits(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().uart_settings().stop_bits(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.uart_settings.stop_bits(value)   # check dev.ok
```

## module

Module. Which UART module handles the port

Wire command: `h\s\u\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.uart_settings.module(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_uart_settings_module(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().uart_settings().module(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.uart_settings.module(value)   # check dev.ok
```
