# LoRa

`dev.wireless.lo_ra` - wire path `w\l` - generated from `fwMenuLoRa`.

## configure

Configure. LoRa modem params:
freqHz: carrier Hz, US 902-928M (def 906875000)
sf: spreading factor 6-12 (def 11)
bwEnc: 0=125 1=250 2=500 kHz (def 1)
cr: coding rate 5-8 = 4/5..4/8 (def 5)
power: TX dBm, -9..22 (def 22)
preamble: symbols (def 8)
sync: 1-byte LoRa sync word, hex (def 12; 2b = Meshtastic)
preamble and sync may be omitted; they then take those defaults
only power is range-checked

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

https://meshtastic.org/docs/configuration/radio/lora/

Wire command: `w\l\c`

| Arg | Wire type |
|---|---|
| freq_hz | decS32 |
| sf | dec |
| bw_enc | dec |
| cr | dec |
| power | decS32 |
| preamble | dec |
| sync | hex8 |

Returns: none (Ok/Err only)

```python
dev.wireless.lo_ra.configure(freq_hz: int, sf: int, bw_enc: int, cr: int, power: int, preamble: int, sync: int) -> Result
```
```c
ow_status ow_wireless_lo_ra_configure(ow_device* dev, int32_t freq_hz, int32_t sf, int32_t bw_enc, int32_t cr, int32_t power, int32_t preamble, uint8_t sync);
```
```rust
dev.wireless().lo_ra().configure(freq_hz: i32, sf: i32, bw_enc: i32, cr: i32, power: i32, preamble: i32, sync: u8) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.lo_ra.configure(freq_hz, sf, bw_enc, cr, power, preamble, sync)   # check dev.ok
```

## send_payload

Send. Transmits a LoRa packet

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\l\s`

| Arg | Wire type |
|---|---|
| data | hexbytes |

Returns: none (Ok/Err only)

```python
dev.wireless.lo_ra.send_payload(data: bytes | bytearray) -> Result
```
```c
ow_status ow_wireless_lo_ra_send_payload(ow_device* dev, const uint8_t* data, size_t data_len);
```
```rust
dev.wireless().lo_ra().send_payload(data: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.lo_ra.send_payload(data)   # check dev.ok
```

## rx_enable

RX Enable. RX control:
0 = standby (radio idle, no RX, low power)
1 = receive (RX armed; packets print as 'lora' events)
default 1 (RX on); not persisted across reboot

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\l\r`

| Arg | Wire type |
|---|---|
| mode | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.lo_ra.rx_enable(mode: int) -> Result
```
```c
ow_status ow_wireless_lo_ra_rx_enable(ow_device* dev, int32_t mode);
```
```rust
dev.wireless().lo_ra().rx_enable(mode: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.lo_ra.rx_enable(mode)   # check dev.ok
```

## status

Status. WIO-E5 bridge status (a 'lora' STATUS event):
state: IDLE/RX/TX/SLEEP
chip: raw SX126x status byte
rssi: live channel, dBm
rxPkts/polls/rxBytes/cmds: counters
rxBytes+cmds rising = DISPLAY<->WIO link alive

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\l\t`

Returns: none (Ok/Err only)

```python
dev.wireless.lo_ra.status() -> Result
```
```c
ow_status ow_wireless_lo_ra_status(ow_device* dev);
```
```rust
dev.wireless().lo_ra().status() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.lo_ra.status()   # check dev.ok
```

## raw_frame

Raw Frame. Sends a raw framed command to the bridge (advanced)

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\l\f`

| Arg | Wire type |
|---|---|
| cmd | hex8 |
| payload | hexbytes |

Returns: none (Ok/Err only)

```python
dev.wireless.lo_ra.raw_frame(cmd: int, payload: bytes | bytearray) -> Result
```
```c
ow_status ow_wireless_lo_ra_raw_frame(ow_device* dev, uint8_t cmd_, const uint8_t* payload, size_t payload_len);
```
```rust
dev.wireless().lo_ra().raw_frame(cmd_: u8, payload: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.lo_ra.raw_frame(cmd, payload)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `lora` (text)

LoRa RX / status / event line (free-form text)

| Payload field | Wire type |
|---|---|
| data | string |
