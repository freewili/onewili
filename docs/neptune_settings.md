# Neptune Settings

`dev.hardware.settings_home.neptune_settings` - wire path `h\s\p` - generated from `fwMenuNeptuneSettings`.

## c_an1_mode

CAN1 Mode. CAN Type or UART over CAN PHY

Wire command: `h\s\p\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1_mode(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1_mode(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1_mode(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1_mode(value)   # check dev.ok
```

## c_an1_rate

CAN1 Rate. Baudrate of CAN or UART over CAN PHY

Wire command: `h\s\p\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1_rate(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1_rate(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1_rate(value)   # check dev.ok
```

## c_an1fdd_rate

CAN1 FD D Rate. Baud Rate for CANFD Data section

Wire command: `h\s\p\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1fdd_rate(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1fdd_rate(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1fdd_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1fdd_rate(value)   # check dev.ok
```

## c_an1_listen_only

CAN1 Listen Only. Enables Listen Only mode

Wire command: `h\s\p\y`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1_listen_only() -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1_listen_only(ow_device* dev);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1_listen_only() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1_listen_only()   # check dev.ok
```

## c_an1_tx_retry

CAN1 Tx Retry. CAN Transmit retry options

Wire command: `h\s\p\e`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1_tx_retry(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1_tx_retry(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1_tx_retry(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1_tx_retry(value)   # check dev.ok
```

## c_an1_cust_baud

CAN1 Cust Baud. Hex Value String for Register C1NBTCFG. Blank to disable.

Wire command: `h\s\p\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1_cust_baud(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1_cust_baud(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1_cust_baud(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1_cust_baud(value)   # check dev.ok
```

## c_an1_cust_data_baud

CAN1 Cust Data Baud. Hex Value String for Register C1DBTCFG

Wire command: `h\s\p\g`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1_cust_data_baud(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1_cust_data_baud(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1_cust_data_baud(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1_cust_data_baud(value)   # check dev.ok
```

## c_an1_termination

CAN1 Termination. Enables termination for network.

Wire command: `h\s\p\1`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1_termination() -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1_termination(ow_device* dev);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1_termination() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1_termination()   # check dev.ok
```

## c_an1api_enabled

CAN1 API Enabled. Set Wili API Base ID

Wire command: `h\s\p\i`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an1api_enabled() -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an1api_enabled(ow_device* dev);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an1api_enabled() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an1api_enabled()   # check dev.ok
```

## c_anapiid

CAN API ID. Enables Terminal over CANFD

Wire command: `h\s\p\j`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_anapiid(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_anapiid(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_anapiid(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_anapiid(value)   # check dev.ok
```

## c_an2_mode

CAN2 Mode. CAN Type or UART over CAN PHY

Wire command: `h\s\p\k`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2_mode(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2_mode(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2_mode(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2_mode(value)   # check dev.ok
```

## c_an2_rate

CAN2 Rate. Baudrate of CAN or UART over CAN PHY

Wire command: `h\s\p\l`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2_rate(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2_rate(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2_rate(value)   # check dev.ok
```

## c_an2fdd_rate

CAN2 FD D Rate. Baud Rate for CANFD Data section

Wire command: `h\s\p\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2fdd_rate(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2fdd_rate(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2fdd_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2fdd_rate(value)   # check dev.ok
```

## c_an2_listen_only

CAN2 Listen Only. Enables Listen Only mode

Wire command: `h\s\p\n`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2_listen_only() -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2_listen_only(ow_device* dev);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2_listen_only() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2_listen_only()   # check dev.ok
```

## c_an2_tx_retry

CAN2 Tx Retry. CAN Transmit retry options

Wire command: `h\s\p\o`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2_tx_retry(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2_tx_retry(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2_tx_retry(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2_tx_retry(value)   # check dev.ok
```

## c_an2_cust_baud

CAN2 Cust Baud. Hex Value String for Register C1NBTCFG. Blank to disable.

Wire command: `h\s\p\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2_cust_baud(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2_cust_baud(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2_cust_baud(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2_cust_baud(value)   # check dev.ok
```

## c_an2_cust_data_baud

CAN2 Cust Data Baud. Hex Value String for Register C1DBTCFG

Wire command: `h\s\p\r`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2_cust_data_baud(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2_cust_data_baud(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2_cust_data_baud(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2_cust_data_baud(value)   # check dev.ok
```

## c_an2_termination

CAN2 Termination. Enables termination for network.

Wire command: `h\s\p\s`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2_termination() -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2_termination(ow_device* dev);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2_termination() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2_termination()   # check dev.ok
```

## c_an2api_enabled

CAN2 API Enabled. Enables Wili API over CANFD

Wire command: `h\s\p\t`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.c_an2api_enabled() -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_c_an2api_enabled(ow_device* dev);
```
```rust
dev.hardware().settings_home().neptune_settings().c_an2api_enabled() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.c_an2api_enabled()   # check dev.ok
```

## l_in_master_en

LIN Master En. Enables LIN Master Pull Resistor

Wire command: `h\s\p\u`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.l_in_master_en() -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_l_in_master_en(ow_device* dev);
```
```rust
dev.hardware().settings_home().neptune_settings().l_in_master_en() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.l_in_master_en()   # check dev.ok
```

## l_in_baud_rate

LIN Baud Rate. Baud Rate for LIN

Wire command: `h\s\p\v`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.l_in_baud_rate(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_l_in_baud_rate(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().neptune_settings().l_in_baud_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.l_in_baud_rate(value)   # check dev.ok
```

## analog_in_en

Analog In En. Enables analog input measurement

Wire command: `h\s\p\x`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.neptune_settings.analog_in_en() -> Result
```
```c
ow_status ow_hardware_settings_home_neptune_settings_analog_in_en(ow_device* dev);
```
```rust
dev.hardware().settings_home().neptune_settings().analog_in_en() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.neptune_settings.analog_in_en()   # check dev.ok
```
