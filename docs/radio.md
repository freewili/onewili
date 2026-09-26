# Radio

`dev.wireless.radio` - wire path `w\r` - generated from `fwMenuRadio`.

## select_circuit

Select Circuit. Claims the sub-GHz front end for this client and holds it until Release.
The CC1101 and the WIO-E5 LoRa module are mutually exclusive in hardware:
one net drives both the antenna switches and the CC1101's chip-select
demux, so claiming the circuit disconnects the LoRa bridge until released.
band: 0 keep whatever is selected (defaults high), 1 low, 2 mid, 3 high.
A hold taken here never expires, which is what makes a multi-step script
safe; a command issued without one takes an implicit hold that DOES expire
after 20 s idle. Refused while the on-screen SubGHz app holds the circuit.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\s`

| Arg | Wire type |
|---|---|
| band | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.select_circuit(band: int) -> Result
```
```c
ow_status ow_wireless_radio_select_circuit(ow_device* dev, int32_t band);
```
```rust
dev.wireless().radio().select_circuit(band: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.select_circuit(band)   # check dev.ok
```

## release_circuit

Release Circuit. Ends this client's circuit hold and hands the antenna back to LoRa.
Fails when no explicit hold is in force, so 'released' and 'was not mine'
stay distinguishable. Needs no hold of its own, so a session left behind by
a script that died is always recoverable from any client.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\e`

Returns: none (Ok/Err only)

```python
dev.wireless.radio.release_circuit() -> Result
```
```c
ow_status ow_wireless_radio_release_circuit(ow_device* dev);
```
```rust
dev.wireless().radio().release_circuit() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.release_circuit()   # check dev.ok
```

## read_state

State. Reads the mux and radio state back, in this order:
owner 0=LoRa 1=CC1101; holder 0=none 1=panel 2=command 3=session;
band 0=none 1=low 2=mid 3=high; wantV1/wantV2 the expander bits asked for;
haveValid/haveV1/haveV2 the same two bits read back out of the expander's
input port, so they are evidence about the pads rather than an echo;
loraPaused 1 = the LoRa PIO UART is stopped; freqHz the frequency the chip is
currently tuned to, which follows a .sub transmit as well as an explicit tune;
active 1 = something is running that outlived its request;
status 0=idle 2=ok 3=fail for the last operation; version the last
comm-check register read (0x14 is a live CC1101, FF means no comm-check has
run yet, and 00 means one ran and the part answered nothing).

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\t`

Returns: owner (dec), holder (dec), band (dec), want_v1 (dec), want_v2 (dec), have_valid (dec), have_v1 (dec), have_v2 (dec), lora_paused (dec), freq_hz (decU32), active (dec), status (dec), version (hex8)

```python
dev.wireless.radio.read_state() -> Result
```
```c
ow_status ow_wireless_radio_read_state(ow_device* dev, int32_t* owner, int32_t* holder, int32_t* band, int32_t* want_v1, int32_t* want_v2, int32_t* have_valid, int32_t* have_v1, int32_t* have_v2, int32_t* lora_paused, int32_t* freq_hz, int32_t* active, int32_t* status, uint8_t* version);
```
```rust
dev.wireless().radio().read_state() -> Result<(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, u8), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.read_state()   # returns value; check dev.ok
```

## select_band

Band. Forces the matched antenna path now: 1 low, 2 mid, 3 high.
The next tune re-derives the path from the frequency, so this is an
override for measurement rather than a persistent setting. Read it back
with State (band, and haveV1/haveV2 for the pads).

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\b`

| Arg | Wire type |
|---|---|
| band | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.select_band(band: int) -> Result
```
```c
ow_status ow_wireless_radio_select_band(ow_device* dev, int32_t band);
```
```rust
dev.wireless().radio().select_band(band: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.select_band(band)   # check dev.ok
```

## comm_check

Comm Check. Reads the CC1101 version register and returns it.
0x14 is a healthy part; anything else means the SPI answered with the wrong
value, and no answer at all fails the command. Not a link check despite the
name -- it is an identity probe on the chip itself.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\c`

Returns: version (hex8)

```python
dev.wireless.radio.comm_check() -> Result
```
```c
ow_status ow_wireless_radio_comm_check(ow_device* dev, uint8_t* version);
```
```rust
dev.wireless().radio().comm_check() -> Result<u8, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.comm_check()   # returns value; check dev.ok
```

## set_frequency

Frequency. Tunes the CC1101 and selects the matched antenna path for that band.
Returns the path the frequency selected (0 none, 1 low, 2 mid, 3 high), not
the frequency. Matched bands are 300-348, 387-464 and 779-928 MHz; outside
them the tune fails rather than transmitting into a mismatch.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\f`

| Arg | Wire type |
|---|---|
| freq_hz | decU32 |

Returns: band (dec)

```python
dev.wireless.radio.set_frequency(freq_hz: int) -> Result
```
```c
ow_status ow_wireless_radio_set_frequency(ow_device* dev, int32_t freq_hz, int32_t* band);
```
```rust
dev.wireless().radio().set_frequency(freq_hz: i32) -> Result<i32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.set_frequency(freq_hz)   # returns value; check dev.ok
```

## read_rssi

RSSI. Samples received signal strength once, in dBm.
A one-shot reading, not a subscription: the radio re-applies the current
frequency, switches to receive and reads once. Poll it for a trace, or use
Monitor to have the radio keep the receiver open between reads.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\i`

Returns: rssi (decS32)

```python
dev.wireless.radio.read_rssi() -> Result
```
```c
ow_status ow_wireless_radio_read_rssi(ow_device* dev, int32_t* rssi);
```
```rust
dev.wireless().radio().read_rssi() -> Result<i32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.read_rssi()   # returns value; check dev.ok
```

## carrier

Carrier. Keys or unkeys an unmodulated carrier at the current frequency.
Keying re-applies the frequency as a side effect. A keyed carrier counts as
the radio being active, so an implicit circuit hold will not expire
underneath it while you read it on an SDR.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\o`

| Arg | Wire type |
|---|---|
| on | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.carrier(on: int) -> Result
```
```c
ow_status ow_wireless_radio_carrier(ow_device* dev, int32_t on);
```
```rust
dev.wireless().radio().carrier(on: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.carrier(on)   # check dev.ok
```

## rx_enable

RX Enable. Puts the CC1101 into continuous receive, or back to idle.
This only opens the receiver; nothing is streamed back by it. For packet
traffic use Packet RX, for signal strength use RSSI or Monitor, and for raw
pulse timing use Capture Start.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\r`

| Arg | Wire type |
|---|---|
| on | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.rx_enable(on: int) -> Result
```
```c
ow_status ow_wireless_radio_rx_enable(ow_device* dev, int32_t on);
```
```rust
dev.wireless().radio().rx_enable(on: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.rx_enable(on)   # check dev.ok
```

## idle

Idle. Returns the CC1101 to idle from receive, transmit or carrier.
Does not hand the circuit back to LoRa; Release does that.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\w`

Returns: none (Ok/Err only)

```python
dev.wireless.radio.idle() -> Result
```
```c
ow_status ow_wireless_radio_idle(ow_device* dev);
```
```rust
dev.wireless().radio().idle() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.idle()   # check dev.ok
```

## packet_send

Packet Send. Transmits one GFSK packet through the CC1101 packet engine.
The modem settings match a Flipper Zero in 'subghz chat' (GFSK 9.99 kb/s,
sync 0x464C, CRC on), so a Flipper in that mode receives it. Payload is
capped at 60 bytes by the chip's FIFO and a longer one is refused rather
than sent short. A chat peer expects a trailing 0x0A.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\x`

| Arg | Wire type |
|---|---|
| freq_hz | decU32 |
| data | hexbytes |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.packet_send(freq_hz: int, data: bytes | bytearray) -> Result
```
```c
ow_status ow_wireless_radio_packet_send(ow_device* dev, int32_t freq_hz, const uint8_t* data, size_t data_len);
```
```rust
dev.wireless().radio().packet_send(freq_hz: i32, data: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.packet_send(freq_hz, data)   # check dev.ok
```

## packet_rx

Packet RX. Opens or closes the GFSK packet receiver at the given frequency.
While it is open each received packet is reported as a 'radio' event (RSSI,
length and payload bytes) without being polled for, and the last one is
also readable with Packet Read. freqHz is ignored when turning it off.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\y`

| Arg | Wire type |
|---|---|
| on | dec |
| freq_hz | decU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.packet_rx(on: int, freq_hz: int) -> Result
```
```c
ow_status ow_wireless_radio_packet_rx(ow_device* dev, int32_t on, int32_t freq_hz);
```
```rust
dev.wireless().radio().packet_rx(on: i32, freq_hz: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.packet_rx(on, freq_hz)   # check dev.ok
```

## packet_read

Packet Read. Reads the last received GFSK packet: RSSI in dBm, a sequence counter that
bumps once per packet, then the payload bytes. Compare the sequence against
the previous read to tell a new packet from a repeat. The payload is
reported up to 60 bytes, so a longer packet is truncated here but its real
length is still reported by the radio.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\k`

Returns: rssi (decS32), seq (decU32), data (hexbytes)

```python
dev.wireless.radio.packet_read() -> Result
```
```c
ow_status ow_wireless_radio_packet_read(ow_device* dev, int32_t* rssi, int32_t* seq, uint8_t* data, size_t data_cap, size_t* data_len);
```
```rust
dev.wireless().radio().packet_read() -> Result<(i32, i32, Vec<u8>), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.packet_read()   # returns value; check dev.ok
```

## capture_start

Capture Start. Arms a raw pulse-duration capture at the given frequency.
Squelch-triggered: it waits for signal strength to open, records edge
durations until it closes or the buffer fills, then holds the result for
Replay. Set the trigger level with Squelch. A frequency outside the matched
bands (300-348, 387-464, 779-928 MHz) is rejected here rather than captured
as noise. Watch progress in State.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\g`

| Arg | Wire type |
|---|---|
| freq_hz | decU32 |
| preset | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.capture_start(freq_hz: int, preset: int) -> Result
```
```c
ow_status ow_wireless_radio_capture_start(ow_device* dev, int32_t freq_hz, int32_t preset);
```
```rust
dev.wireless().radio().capture_start(freq_hz: i32, preset: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.capture_start(freq_hz, preset)   # check dev.ok
```

## capture_stop

Capture Stop. Ends a capture and returns how many pulse durations it recorded.
Zero means the squelch never opened, which usually means the level is set
below the receiver's noise floor or there was nothing to hear.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\j`

Returns: durations (decU32)

```python
dev.wireless.radio.capture_stop() -> Result
```
```c
ow_status ow_wireless_radio_capture_stop(ow_device* dev, int32_t* durations);
```
```rust
dev.wireless().radio().capture_stop() -> Result<i32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.capture_stop()   # returns value; check dev.ok
```

## replay

Replay. Re-transmits the last capture out the transmit path.
Fails when nothing has been captured. Acks as soon as the burst is armed
because the transmit itself runs on the display's PIO and DMA; watch the
active flag in State to see it finish. Flip the pulse phase first with
Replay Invert if the recovered signal is inverted.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\p`

Returns: none (Ok/Err only)

```python
dev.wireless.radio.replay() -> Result
```
```c
ow_status ow_wireless_radio_replay(ow_device* dev);
```
```rust
dev.wireless().radio().replay() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.replay()   # check dev.ok
```

## analyzer

Analyzer. Starts or stops the background frequency-analyzer sweep.
One frequency is measured per service tick so nothing else stalls, and the
bins plus the running peak are readable with Spectrum. The sweep counts as
the radio being active, so an implicit circuit hold will not expire while it
runs.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\a`

| Arg | Wire type |
|---|---|
| on | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.analyzer(on: int) -> Result
```
```c
ow_status ow_wireless_radio_analyzer(ow_device* dev, int32_t on);
```
```rust
dev.wireless().radio().analyzer(on: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.analyzer(on)   # check dev.ok
```

## spectrum

Spectrum. Reads the analyzer's results: the peak frequency in Hz, its level in dBm,
then one signed byte of dBm per sweep bin, lowest frequency first. Values
are two's-complement, so 0xC0 is -64 dBm. Empty until the sweep has been
started with Analyzer.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\n`

Returns: peak_freq_hz (decU32), peak_rssi (decS32), bins (hexbytes)

```python
dev.wireless.radio.spectrum() -> Result
```
```c
ow_status ow_wireless_radio_spectrum(ow_device* dev, int32_t* peak_freq_hz, int32_t* peak_rssi, uint8_t* bins, size_t bins_cap, size_t* bins_len);
```
```rust
dev.wireless().radio().spectrum() -> Result<(i32, i32, Vec<u8>), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.spectrum()   # returns value; check dev.ok
```

## squelch

Squelch. Sets the level a capture must see before it starts recording, and below
which it stops. Default -65. Set it between the signal you want (a nearby
remote reads around -10) and the receiver's noise floor, which rides higher
than -75 on repeat captures because the gain control has wound up.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\u`

| Arg | Wire type |
|---|---|
| dbm | decS32 |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.squelch(dbm: int) -> Result
```
```c
ow_status ow_wireless_radio_squelch(ow_device* dev, int32_t dbm);
```
```rust
dev.wireless().radio().squelch(dbm: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.squelch(dbm)   # check dev.ok
```

## replay_invert

Replay Invert. Flips the captured low/high phase before re-keying it on Replay.
Use it when a replay does not reproduce the original and the capture looks
phase-inverted. Default off.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\v`

| Arg | Wire type |
|---|---|
| on | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.replay_invert(on: int) -> Result
```
```c
ow_status ow_wireless_radio_replay_invert(ow_device* dev, int32_t on);
```
```rust
dev.wireless().radio().replay_invert(on: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.replay_invert(on)   # check dev.ok
```

## transmit_sub_file

Transmit Sub. Transmits a Flipper .sub file from the card.
A bare name is resolved under the radio directory and .sub is appended when
the name has no extension. This processor parses the file and streams the
decoded durations to the display, so the ack means the burst was armed, not
that it finished; a 'radioasync' event reports completion. The display holds
4096 durations and a longer file is refused rather than sent truncated.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\m`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.transmit_sub_file(path: str) -> Result
```
```c
ow_status ow_wireless_radio_transmit_sub_file(ow_device* dev, const char* path);
```
```rust
dev.wireless().radio().transmit_sub_file(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.transmit_sub_file(path)   # check dev.ok
```

## monitor

Monitor. Keeps the receiver open and samples signal strength continuously, so
repeated RSSI reads reflect a live channel instead of re-entering receive
each time. Read the value with RSSI or in State. Counts as the radio being
active, so an implicit circuit hold will not expire while it runs.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

Wire command: `w\r\l`

| Arg | Wire type |
|---|---|
| on | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.radio.monitor(on: int) -> Result
```
```c
ow_status ow_wireless_radio_monitor(ow_device* dev, int32_t on);
```
```rust
dev.wireless().radio().monitor(on: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.radio.monitor(on)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `radio1` (text)

Radio 1 received data (hex bytes)

| Payload field | Wire type |
|---|---|
| data_bytes | hexbytes |

### `radio2` (text)

Radio 2 received data (hex bytes)

| Payload field | Wire type |
|---|---|
| data_bytes | hexbytes |

### `radioasync` (text)

Async sub-file transmit/capture status (free-form text)

| Payload field | Wire type |
|---|---|
| data | string |
