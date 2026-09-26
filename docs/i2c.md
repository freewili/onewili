# I2C Functions

`dev.io.i2c` - wire path `i\i` - generated from `fwMenuI2C`.

## i2c_write

Write. Writes data to a specific I2C Address

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\i\w`

| Arg | Wire type |
|---|---|
| address | hex8 |
| register | hex8 |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.i2c.i2c_write(address: int, register: int, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_i2c_i2c_write(ow_device* dev, uint8_t address, uint8_t register_, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().i2c().i2c_write(address: u8, register: u8, data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.i2c.i2c_write(address, register, data_bytes)   # check dev.ok
```

## i2c_read

Read. Reads the number from the address

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\i\r`

Returns: i2crepsone (hexbytes)

```python
dev.io.i2c.i2c_read() -> Result
```
```c
ow_status ow_io_i2c_i2c_read(ow_device* dev, uint8_t* i2crepsone, size_t i2crepsone_cap, size_t* i2crepsone_len);
```
```rust
dev.io().i2c().i2c_read() -> Result<Vec<u8>, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.i2c.i2c_read()   # returns value; check dev.ok
```

## i2c_poll

Poll. Tests all addresses for I2C Response

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\i\p`

Returns: none (Ok/Err only)

```python
dev.io.i2c.i2c_poll() -> Result
```
```c
ow_status ow_io_i2c_i2c_poll(ow_device* dev);
```
```rust
dev.io().i2c().i2c_poll() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.i2c.i2c_poll()   # check dev.ok
```

## i2c_slave_enable

I2C Slave Enable. Enables I2C slave mode at the given 7-bit address; 0 disables

Requires power zone 6 (FPGA). See [Errors](errors.md).

##### I2C Slave Enable

Turns this device into an I2C slave on the breakout bus (SDA 16 / SCL 17) at the given 7-bit address, backed by a 256-byte register file with auto-incrementing pointer semantics.

###### Usage
```
e 17
e 0
```
`e 17` enables slave mode at address 0x17; `e 0` disables it and returns the bus to master mode using the current I2C settings.

###### Behavior
- While slave mode is active the master items (Write, Read, Poll) report Failed.
- Master writes received by the slave are streamed as `i2cslv` events (`register` followed by the data bytes).
- Seed the register file with **Set I2C Slave Data** (`l`).

###### Arguments
- `address` — 7-bit slave address in hex; `0` disables slave mode.

Wire command: `i\i\e`

| Arg | Wire type |
|---|---|
| address | hex8 |

Returns: none (Ok/Err only)

```python
dev.io.i2c.i2c_slave_enable(address: int) -> Result
```
```c
ow_status ow_io_i2c_i2c_slave_enable(ow_device* dev, uint8_t address);
```
```rust
dev.io().i2c().i2c_slave_enable(address: u8) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.i2c.i2c_slave_enable(address)   # check dev.ok
```

## i2c_slave_set_data

Set I2C Slave Data. Writes bytes into the I2C slave register file

Requires power zone 6 (FPGA). See [Errors](errors.md).

##### Set I2C Slave Data

Seeds the I2C slave register file: the first byte is the starting register index, the rest are data bytes stored from there.

###### Usage
```
l 00 DE AD BE EF
```
Stores DE AD BE EF at registers 0x00-0x03.

###### Behavior
- Works whether or not slave mode is currently enabled; the register file persists across enable/disable.
- Bounds-checked against the 256-byte register file.

###### Arguments
- `dataBytes` — register index followed by one or more data bytes, all hex.

Wire command: `i\i\l`

| Arg | Wire type |
|---|---|
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.i2c.i2c_slave_set_data(data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_i2c_i2c_slave_set_data(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().i2c().i2c_slave_set_data(data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.i2c.i2c_slave_set_data(data_bytes)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `i2cmon` (text)

I2C monitor captured bytes

| Payload field | Wire type |
|---|---|
| data_bytes | hexbytes |

### `i2cslv` (text)

I2C slave received master write (register, data bytes)

| Payload field | Wire type |
|---|---|
| data_bytes | hexbytes |

## Sub-menus

- [I2C Settings](i2c_settings.md) - `dev.io.i2c.settings`
