# Raw Transceiver

`dev.wireless.nfc.raw` - wire path `w\n\k` - generated from `fwMenuNFCRaw`.

## begin

Begin. Initialize the ST25R3916 and take ownership of the NFC front-end

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\k\b`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.raw.begin() -> Result
```
```c
ow_status ow_wireless_nfc_raw_begin(ow_device* dev);
```
```rust
dev.wireless().nfc().raw().begin() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.raw.begin()   # check dev.ok
```

## end

End. Release the ST25R3916 back to the normal reader/writer state machine

Wire command: `w\n\k\e`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.raw.end() -> Result
```
```c
ow_status ow_wireless_nfc_raw_end(ow_device* dev);
```
```rust
dev.wireless().nfc().raw().end() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.raw.end()   # check dev.ok
```

## field

Field. Turn the RF field on or off

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\k\f`

| Arg | Wire type |
|---|---|
| on | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.raw.field(on: int) -> Result
```
```c
ow_status ow_wireless_nfc_raw_field(ow_device* dev, int32_t on);
```
```rust
dev.wireless().nfc().raw().field(on: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.raw.field(on)   # check dev.ok
```

## reg_write

Write Register. Write a single ST25R3916 register

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\k\w`

| Arg | Wire type |
|---|---|
| addr | hex |
| value | hex |

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.raw.reg_write(addr: int, value: int) -> Result
```
```c
ow_status ow_wireless_nfc_raw_reg_write(ow_device* dev, uint32_t addr, uint32_t value);
```
```rust
dev.wireless().nfc().raw().reg_write(addr: u32, value: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.raw.reg_write(addr, value)   # check dev.ok
```

## reg_read

Read Register. Read a single ST25R3916 register

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\k\r`

| Arg | Wire type |
|---|---|
| addr | hex |

Returns: value (hex)

```python
dev.wireless.nfc.raw.reg_read(addr: int) -> Result
```
```c
ow_status ow_wireless_nfc_raw_reg_read(ow_device* dev, uint32_t addr, uint32_t* value);
```
```rust
dev.wireless().nfc().raw().reg_read(addr: u32) -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.raw.reg_read(addr)   # returns value; check dev.ok
```

## cmd

Send Command. Send a direct command to the ST25R3916

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\k\c`

| Arg | Wire type |
|---|---|
| command | hex |

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.raw.cmd(command: int) -> Result
```
```c
ow_status ow_wireless_nfc_raw_cmd(ow_device* dev, uint32_t command);
```
```rust
dev.wireless().nfc().raw().cmd(command: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.raw.cmd(command)   # check dev.ok
```

## transceive

Transceive. Transmit bytes and receive the response over the RF field

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\k\t`

| Arg | Wire type |
|---|---|
| flags | hex |
| timeout_ms | dec |
| tx | hexbytes |

Returns: status (hex), rx (hexbytes)

```python
dev.wireless.nfc.raw.transceive(flags: int, timeout_ms: int, tx: bytes | bytearray) -> Result
```
```c
ow_status ow_wireless_nfc_raw_transceive(ow_device* dev, uint32_t flags, int32_t timeout_ms, const uint8_t* tx, size_t tx_len, uint32_t* status, uint8_t* rx, size_t rx_cap, size_t* rx_len);
```
```rust
dev.wireless().nfc().raw().transceive(flags: u32, timeout_ms: i32, tx: &[u8]) -> Result<(u32, Vec<u8>), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.raw.transceive(flags, timeout_ms, tx)   # returns value; check dev.ok
```
