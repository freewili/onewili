# Ethernet 10BaseT1S

`dev.io.t1s` - wire path `i\r` - generated from `fwMenuT1S`.

## t1s_status

Status. Prints one line of key=value T1S engine status: state link plca plcaen id cnt to chipRev t1sTx t1sTxDrop t1sRx t1sRxDrop spiAbort errs evts faults lastErr lastEvt ... term tc10 wkgen wksrc. plca=1 means the PLCA cycle is locked; chipRev is 0 until the PHY initialized; tc10 is 0 awake / 1 sleep pending / 2 sleeping, wkgen counts TC10 wake generations requested, wksrc is the last wake source (bit1 MDI, bit0 WAKE_IN). Wire-parseable, append-only

Wire command: `i\r\s`

Returns: status (string)

```python
dev.io.t1s.t1s_status() -> Result
```
```c
ow_status ow_io_t1s_t1s_status(ow_device* dev, char* status, size_t status_cap);
```
```rust
dev.io().t1s().t1s_status() -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.t1s_status()   # returns value; check dev.ok
```

## t1s_link_status

Link Status. Reports the T1S link (up while the PHY is initialized and running), the engine state name and whether the NCM<->T1S bridge is on

Wire command: `i\r\k`

Returns: info (string)

```python
dev.io.t1s.t1s_link_status() -> Result
```
```c
ow_status ow_io_t1s_t1s_link_status(ow_device* dev, char* info, size_t info_cap);
```
```rust
dev.io().t1s().t1s_link_status() -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.t1s_link_status()   # returns value; check dev.ok
```

## t1s_reinit_phy

Reinit PHY. Requests a full PHY reinit: RST pulse plus fresh TC6 init with the current PLCA settings (this is how Burst Max/Burst Timer changes take effect). Also enables the T1S engine and clears the FAULT retry budget; bring-up itself still waits for the IO-header rail (zone 6)

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\i`

Returns: none (Ok/Err only)

```python
dev.io.t1s.t1s_reinit_phy() -> Result
```
```c
ow_status ow_io_t1s_t1s_reinit_phy(ow_device* dev);
```
```rust
dev.io().t1s().t1s_reinit_phy() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.t1s_reinit_phy()   # check dev.ok
```

## t1s_clear_counters

Clear Counters. Zeros the T1S TX/RX/drop/error counters (chip revision is kept). The engine state, link and bridge are unaffected

Wire command: `i\r\c`

Returns: none (Ok/Err only)

```python
dev.io.t1s.t1s_clear_counters() -> Result
```
```c
ow_status ow_io_t1s_t1s_clear_counters(ow_device* dev);
```
```rust
dev.io().t1s().t1s_clear_counters() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.t1s_clear_counters()   # check dev.ok
```

## t1s_register_read

Register Read. Reads one 32-bit register from the LAN865x over the TC6 SPI protocol: MMS is the memory map selector (0..15), Address the 16-bit register address within it. Requires the PHY to be initialized and running

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\g`

| Arg | Wire type |
|---|---|
| mms | dec |
| address | hex16 |

Returns: value (hex32)

```python
dev.io.t1s.t1s_register_read(mms: int, address: int) -> Result
```
```c
ow_status ow_io_t1s_t1s_register_read(ow_device* dev, int32_t mms, uint32_t address, uint32_t* value);
```
```rust
dev.io().t1s().t1s_register_read(mms: i32, address: u32) -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.t1s_register_read(mms, address)   # returns value; check dev.ok
```

## bridge

Bridge. When on, host NCM frames forward to the T1S wire and T1S frames forward to the host (the local classifier/responder/loopback step aside) and the host adapter's link mirrors the T1S link. Always off after a reboot

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\b`

Returns: none (Ok/Err only)

```python
dev.io.t1s.bridge() -> Result
```
```c
ow_status ow_io_t1s_bridge(ow_device* dev);
```
```rust
dev.io().t1s().bridge() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.bridge()   # check dev.ok
```

## Sub-menus

- [Ethernet Test](eth_test.md) - `dev.io.t1s.eth_test`
- [PLCA Settings](plca.md) - `dev.io.t1s.plca`
- [TC10 Wake/Sleep](t1s_tc10.md) - `dev.io.t1s.tc10`
