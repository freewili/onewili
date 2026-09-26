# MDIO Functions

`dev.io.mdio` - wire path `i\m` - generated from `fwMenuMDIO`.

## mdio_poll_sfp

SFP Poll. Polls for SFP Modules on the I2C bus. If any are found, return the PHY's temperature in Celsius and Signal Quality Indicator (SQI)

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\a`

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdio_poll_sfp() -> Result
```
```c
ow_status ow_io_mdio_mdio_poll_sfp(ow_device* dev);
```
```rust
dev.io().mdio().mdio_poll_sfp() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_poll_sfp()   # check dev.ok
```

## mdio_read_sfp

SFP Read. Reads a value from a register on the specified device address

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\b`

| Arg | Wire type |
|---|---|
| device_address | hex8 |
| register_address | hexbytes |

Returns: sfp_response (hex16)

```python
dev.io.mdio.mdio_read_sfp(device_address: int, register_address: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdio_read_sfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, uint32_t* sfp_response);
```
```rust
dev.io().mdio().mdio_read_sfp(device_address: u8, register_address: &[u8]) -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_read_sfp(device_address, register_address)   # returns value; check dev.ok
```

## mdio_write_sfp

SFP Write. Writes a value to a register on the specified device address

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\c`

| Arg | Wire type |
|---|---|
| device_address | hex8 |
| register_address | hexbytes |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdio_write_sfp(device_address: int, register_address: bytes | bytearray, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdio_write_sfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().mdio().mdio_write_sfp(device_address: u8, register_address: &[u8], data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_write_sfp(device_address, register_address, data_bytes)   # check dev.ok
```

## mdiormwsfp

SFP Read-Modify-Write. Read-Modify-Writes a value to a register on the specified device address. '1' bits in the mask indicate an overwrite

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\e`

| Arg | Wire type |
|---|---|
| device_address | hex8 |
| register_address | hexbytes |
| mask_bytes | hexbytes |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdiormwsfp(device_address: int, register_address: bytes | bytearray, mask_bytes: bytes | bytearray, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdiormwsfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().mdio().mdiormwsfp(device_address: u8, register_address: &[u8], mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdiormwsfp(device_address, register_address, mask_bytes, data_bytes)   # check dev.ok
```

## mdio_poll

PHY Address Poll. Polls all 32 possible PHY addresses. Test for a response from status register. Returns PHY addresses and clause compatibility

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\y`

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdio_poll() -> Result
```
```c
ow_status ow_io_mdio_mdio_poll(ow_device* dev);
```
```rust
dev.io().mdio().mdio_poll() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_poll()   # check dev.ok
```

## mdio_read22

Clause 22 Read. Reads a value from a register belonging to a Clause-22-Compatible-PHY

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\g`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| register_address | hex8 |

Returns: mdio_response (hex16)

```python
dev.io.mdio.mdio_read22(phy_address: int, register_address: int) -> Result
```
```c
ow_status ow_io_mdio_mdio_read22(ow_device* dev, uint8_t phy_address, uint8_t register_address, uint32_t* mdio_response);
```
```rust
dev.io().mdio().mdio_read22(phy_address: u8, register_address: u8) -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_read22(phy_address, register_address)   # returns value; check dev.ok
```

## mdio_write22

Clause 22 Write. Writes a value to a register belonging to a Clause-22-Compatible-PHY

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\i`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| register_address | hex8 |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdio_write22(phy_address: int, register_address: int, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdio_write22(ow_device* dev, uint8_t phy_address, uint8_t register_address, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().mdio().mdio_write22(phy_address: u8, register_address: u8, data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_write22(phy_address, register_address, data_bytes)   # check dev.ok
```

## mdiormw22

Clause 22 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\j`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| register_address | hex8 |
| mask_bytes | hexbytes |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdiormw22(phy_address: int, register_address: int, mask_bytes: bytes | bytearray, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdiormw22(ow_device* dev, uint8_t phy_address, uint8_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().mdio().mdiormw22(phy_address: u8, register_address: u8, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdiormw22(phy_address, register_address, mask_bytes, data_bytes)   # check dev.ok
```

## mdio_read45

Clause 45 Read. Reads a value from a register belonging to a Clause-45-Compatible-PHY

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\k`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| mmd_address | hex8 |
| register_address | hex16 |

Returns: mdio_response (hex16)

```python
dev.io.mdio.mdio_read45(phy_address: int, mmd_address: int, register_address: int) -> Result
```
```c
ow_status ow_io_mdio_mdio_read45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, uint32_t* mdio_response);
```
```rust
dev.io().mdio().mdio_read45(phy_address: u8, mmd_address: u8, register_address: u32) -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_read45(phy_address, mmd_address, register_address)   # returns value; check dev.ok
```

## mdio_write45

Clause 45 Write. Writes a value to a register belonging to a Clause-45-Compatible-PHY

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\l`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| mmd_address | hex8 |
| register_address | hex16 |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdio_write45(phy_address: int, mmd_address: int, register_address: int, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdio_write45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().mdio().mdio_write45(phy_address: u8, mmd_address: u8, register_address: u32, data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_write45(phy_address, mmd_address, register_address, data_bytes)   # check dev.ok
```

## mdiormw45

Clause 45 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\m`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| mmd_address | hex8 |
| register_address | hex16 |
| mask_bytes | hexbytes |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdiormw45(phy_address: int, mmd_address: int, register_address: int, mask_bytes: bytes | bytearray, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdiormw45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().mdio().mdiormw45(phy_address: u8, mmd_address: u8, register_address: u32, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdiormw45(phy_address, mmd_address, register_address, mask_bytes, data_bytes)   # check dev.ok
```

## mdio_read_emu

Clause 22 Access to Clause 45 Read. Reads a value from a register belonging to a Clause-45-Emulation-Compatible-PHY

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\n`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| mmd_address | hex8 |
| register_address | hex16 |

Returns: mdio_response (hex16)

```python
dev.io.mdio.mdio_read_emu(phy_address: int, mmd_address: int, register_address: int) -> Result
```
```c
ow_status ow_io_mdio_mdio_read_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, uint32_t* mdio_response);
```
```rust
dev.io().mdio().mdio_read_emu(phy_address: u8, mmd_address: u8, register_address: u32) -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_read_emu(phy_address, mmd_address, register_address)   # returns value; check dev.ok
```

## mdio_write_emu

Clause 22 Access to Clause 45 Write. Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\o`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| mmd_address | hex8 |
| register_address | hex16 |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdio_write_emu(phy_address: int, mmd_address: int, register_address: int, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdio_write_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().mdio().mdio_write_emu(phy_address: u8, mmd_address: u8, register_address: u32, data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdio_write_emu(phy_address, mmd_address, register_address, data_bytes)   # check dev.ok
```

## mdiormw_emu

Clause 22 Access to Clause 45 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY. '1' bits in the mask indicate an overwrite

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\m\p`

| Arg | Wire type |
|---|---|
| phy_address | hex8 |
| mmd_address | hex8 |
| register_address | hex16 |
| mask_bytes | hexbytes |
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.mdio.mdiormw_emu(phy_address: int, mmd_address: int, register_address: int, mask_bytes: bytes | bytearray, data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_mdio_mdiormw_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().mdio().mdiormw_emu(phy_address: u8, mmd_address: u8, register_address: u32, mask_bytes: &[u8], data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.mdio.mdiormw_emu(phy_address, mmd_address, register_address, mask_bytes, data_bytes)   # check dev.ok
```
