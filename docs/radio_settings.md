# radio1

`dev.hardware.settings_home.radio_settings` - wire path `h\s\r` - generated from `fwMenuRadioSettings`.

## frequency_mhz

FrequencyMhz. basic frequency calculated automatically (default = 433.92). The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ

Wire command: `h\s\r\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.frequency_mhz(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_frequency_mhz(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().frequency_mhz(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.frequency_mhz(value)   # check dev.ok
```

## modulation

Modulation. set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK

Wire command: `h\s\r\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.modulation(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_modulation(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().modulation(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.modulation(value)   # check dev.ok
```

## devation

Devation. Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.

Wire command: `h\s\r\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.devation(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_devation(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().devation(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.devation(value)   # check dev.ok
```

## channel

Channel. Channelnumber from 0 to 255. Default is channel 0

Wire command: `h\s\r\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.channel(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_channel(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().channel(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.channel(value)   # check dev.ok
```

## channel_spacing

ChannelSpacing. channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.

Wire command: `h\s\r\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.channel_spacing(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_channel_spacing(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().channel_spacing(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.channel_spacing(value)   # check dev.ok
```

## rx_bandwidth

RxBandwidth. Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.

Wire command: `h\s\r\y`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.rx_bandwidth(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_rx_bandwidth(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().rx_bandwidth(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.rx_bandwidth(value)   # check dev.ok
```

## data_rate

DataRate. Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud

Wire command: `h\s\r\e`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.data_rate(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_data_rate(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().data_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.data_rate(value)   # check dev.ok
```

## power_amp

PowerAmp. TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max

Wire command: `h\s\r\g`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.power_amp(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_power_amp(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().power_amp(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.power_amp(value)   # check dev.ok
```

## sync_mode

SyncMode. Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync- carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.

Wire command: `h\s\r\1`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.sync_mode(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_sync_mode(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().sync_mode(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.sync_mode(value)   # check dev.ok
```

## sync_word

SyncWord. sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low)

Wire command: `h\s\r\i`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.sync_word(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_sync_word(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().sync_word(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.sync_word(value)   # check dev.ok
```

## addr_check

AddrCheck. Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.

Wire command: `h\s\r\j`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.addr_check(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_addr_check(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().addr_check(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.addr_check(value)   # check dev.ok
```

## address

Address. Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).

Wire command: `h\s\r\k`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.address(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_address(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().address(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.address(value)   # check dev.ok
```

## white_data

WhiteData. Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.

Wire command: `h\s\r\l`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.white_data() -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_white_data(ow_device* dev);
```
```rust
dev.hardware().settings_home().radio_settings().white_data() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.white_data()   # check dev.ok
```

## packet_format

PacketFormat. Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.

Wire command: `h\s\r\n`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.packet_format(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_packet_format(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().packet_format(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.packet_format(value)   # check dev.ok
```

## length_config

LengthConfig. 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved

Wire command: `h\s\r\o`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.length_config(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_length_config(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().length_config(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.length_config(value)   # check dev.ok
```

## packet_length

PacketLength. Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.

Wire command: `h\s\r\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.packet_length(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_packet_length(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().packet_length(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.packet_length(value)   # check dev.ok
```

## c_rc_enabled

CRCEnabled. 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.

Wire command: `h\s\r\x`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.c_rc_enabled() -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_c_rc_enabled(ow_device* dev);
```
```rust
dev.hardware().settings_home().radio_settings().c_rc_enabled() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.c_rc_enabled()   # check dev.ok
```

## c_rc_auto_flush

CRCAutoFlush. Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.

Wire command: `h\s\r\0`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.c_rc_auto_flush() -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_c_rc_auto_flush(ow_device* dev);
```
```rust
dev.hardware().settings_home().radio_settings().c_rc_auto_flush() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.c_rc_auto_flush()   # check dev.ok
```

## d_c_blocking_filter

DCBlockingFilter. Disable digital DC blocking filter before demodulator. Only for data rates <= 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).

Wire command: `h\s\r\r`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.d_c_blocking_filter() -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_d_c_blocking_filter(ow_device* dev);
```
```rust
dev.hardware().settings_home().radio_settings().d_c_blocking_filter() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.d_c_blocking_filter()   # check dev.ok
```

## manchester

Manchester. Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.

Wire command: `h\s\r\s`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.manchester() -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_manchester(ow_device* dev);
```
```rust
dev.hardware().settings_home().radio_settings().manchester() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.manchester()   # check dev.ok
```

## forword_error_correction

ForwordErrorCorrection. Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.

Wire command: `h\s\r\t`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.forword_error_correction() -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_forword_error_correction(ow_device* dev);
```
```rust
dev.hardware().settings_home().radio_settings().forword_error_correction() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.forword_error_correction()   # check dev.ok
```

## preamble_bytes

PreambleBytes. Sets the minimum number of preamble bytes to be transmitted. Values: 0 : 2, 1 : 3, 2 : 4, 3 : 6, 4 : 8, 5 : 12, 6 : 16, 7 : 24

Wire command: `h\s\r\u`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.preamble_bytes(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_preamble_bytes(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().preamble_bytes(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.preamble_bytes(value)   # check dev.ok
```

## p_qt

PQT. Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4-PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.

Wire command: `h\s\r\v`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.p_qt(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_p_qt(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_settings().p_qt(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.p_qt(value)   # check dev.ok
```

## append_status

AppendStatus. When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.

Wire command: `h\s\r\w`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_settings.append_status() -> Result
```
```c
ow_status ow_hardware_settings_home_radio_settings_append_status(ow_device* dev);
```
```rust
dev.hardware().settings_home().radio_settings().append_status() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_settings.append_status()   # check dev.ok
```
