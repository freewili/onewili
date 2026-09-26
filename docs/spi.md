# SPI Functions

`dev.io.spi` - wire path `i\e` - generated from `fwMenuSPI`.

## s_pi_write

Write and Read. Writes data to SPI and returns response data

Requires power zone 6 (FPGA). See [Errors](errors.md).

##### Write and Read (SPI)

Performs a full-duplex SPI transaction: writes the supplied bytes on MOSI while simultaneously capturing the bytes returned on MISO, then prints the received bytes.

###### Usage
Enter one or more data bytes as hexadecimal values separated by spaces.

```
w 9F
w 03 00 00 00
w AB CD EF
```

###### Behavior
- **Chip Select** is asserted automatically (driven low before the transfer, high after).
- The transfer length equals the number of input bytes; MISO is captured for every clocked byte.
- Uses the currently configured SPI **baud rate**, **mode**, and **CS pin** from the SPI settings.

###### Arguments
- `dataBytes` — one or more hex bytes to clock out (e.g. `9F`, `03 00 00 00`).

###### Returns
- `success` — `true` if the transfer completed, `false` on invalid input or bus error.
- Response bytes are printed as space-separated hex.

###### Example
Reading a SPI flash JEDEC ID:
```
w 9F 00 00 00
```
Response bytes 2–4 contain the manufacturer / device ID.

Wire command: `i\e\w`

| Arg | Wire type |
|---|---|
| data_bytes | hexbytes |

Returns: spi_response (hexbytes)

```python
dev.io.spi.s_pi_write(data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_spi_s_pi_write(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len, uint8_t* spi_response, size_t spi_response_cap, size_t* spi_response_len);
```
```rust
dev.io().spi().s_pi_write(data_bytes: &[u8]) -> Result<Vec<u8>, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.spi.s_pi_write(data_bytes)   # returns value; check dev.ok
```

## s_pi_slave_enable

SPI Slave Enable. Toggles SPI slave mode using the configured response data

Requires power zone 6 (FPGA). See [Errors](errors.md).

##### SPI Slave Enable

Toggles this device into SPI slave mode on the breakout bus (MISO 12 / CS 13 / SCLK 14 / MOSI 15).

###### Behavior
- Enabling reconfigures the breakout directions (CS and SCLK become inputs) and arms the response data set with **Set SPI Slave Response Data** (`l`).
- Bytes clocked in by the master are streamed as `spislv` events.
- The response (up to 8 bytes, the hardware FIFO depth) is re-armed after every chip-select release.
- While slave mode is active, **Write and Read** reports Failed.
- Disabling restores the default breakout directions and master mode per the SPI settings.

###### CPHA requirement for multi-byte transfers
The master here holds Chip Select low for the whole duration of a **Write and Read** (`w`), not just one byte. The PL022 SPI peripheral behind this slave mode only supports that when **CPHA is 1**: at the default CPHA=0, the hardware requires Chip Select to pulse between every single byte, so only the first byte of a multi-byte transfer is valid and every byte after it reads back as idle (0xFF) on both sides. Set CPHA (SPI Settings `a`) to 1 on BOTH the master and the slave board before exchanging more than one byte; a single-byte transfer works at either CPHA setting.

###### Direction apply while slave is active
An FPGA/direction operation while slave mode is active may steal the SPI pins; re-enable slave mode if that happens.

Wire command: `i\e\e`

Returns: none (Ok/Err only)

```python
dev.io.spi.s_pi_slave_enable() -> Result
```
```c
ow_status ow_io_spi_s_pi_slave_enable(ow_device* dev);
```
```rust
dev.io().spi().s_pi_slave_enable() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.spi.s_pi_slave_enable()   # check dev.ok
```

## s_pi_slave_set_data

Set SPI Slave Response Data. Sets the bytes the SPI slave clocks out (max 8, FIFO depth)

Requires power zone 6 (FPGA). See [Errors](errors.md).

##### Set SPI Slave Response Data

Stores the bytes the SPI slave will clock out on MISO when a master transfers. Limited to 8 bytes -- the PL022 transmit FIFO depth.

###### Usage
```
l A1 B2 C3 D4
```

###### Behavior
- Takes effect immediately if slave mode is active (the FIFO is flushed and re-armed).
- A master clocking more bytes than the response length reads undefined data for the excess bytes; its extra written bytes are still captured and streamed.

Wire command: `i\e\l`

| Arg | Wire type |
|---|---|
| data_bytes | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.spi.s_pi_slave_set_data(data_bytes: bytes | bytearray) -> Result
```
```c
ow_status ow_io_spi_s_pi_slave_set_data(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len);
```
```rust
dev.io().spi().s_pi_slave_set_data(data_bytes: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.spi.s_pi_slave_set_data(data_bytes)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `spislv` (text)

SPI slave received bytes from the master

| Payload field | Wire type |
|---|---|
| data_bytes | hexbytes |

## Sub-menus

- [SPI Settings](spi_settings.md) - `dev.io.spi.settings`
