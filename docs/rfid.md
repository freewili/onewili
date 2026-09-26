# RFID Functions

`dev.wireless.rfid` - wire path `w\p` - generated from `fwMenuRFID`.

## enable_reader

Enable Reader. Start or stop the 125 kHz carrier and tag reader

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Enable Reader

Starts or stops the 125 kHz carrier and demodulator.

```
r 1
```

Enabling claims GPIO46 (envelope) and GPIO34 (carrier). GPIO34 shares PWM slice 9 with the haptic, and clkdiv is per-slice, so a buzz while the reader runs disturbs the carrier.


Wire command: `w\p\r`

| Arg | Wire type |
|---|---|
| enable | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.rfid.enable_reader(enable: int) -> Result
```
```c
ow_status ow_wireless_rfid_enable_reader(ow_device* dev, int32_t enable);
```
```rust
dev.wireless().rfid().enable_reader(enable: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.enable_reader(enable)   # check dev.ok
```

## get_status

Get Status. Reader state, carrier frequency and live envelope

#### Get Status

Reader state, measured carrier, live envelope window and the frame/tag counters.

A large `frames` with a near-zero `tags` means the front end produces edges no decoder accepts, which points at bit timing rather than coupling.


Wire command: `w\p\g`

Returns: state (dec), flags (hex8), carrier_hz (decU32), env_min (decU32), env_max (decU32), threshold (decU32), frames (decU32), tags (decU32)

```python
dev.wireless.rfid.get_status() -> Result
```
```c
ow_status ow_wireless_rfid_get_status(ow_device* dev, int32_t* state, uint8_t* flags, int32_t* carrier_hz, int32_t* env_min, int32_t* env_max, int32_t* threshold, int32_t* frames, int32_t* tags);
```
```rust
dev.wireless().rfid().get_status() -> Result<(i32, u8, i32, i32, i32, i32, i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.get_status()   # returns value; check dev.ok
```

## read_tag

Read Tag. Block until one tag is decoded or the timeout expires

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Read Tag

Waits for one tag and returns its format, modulation and id.

```
t 2000
```

An unknown format still returns what was assembled, so an unrecognised tag is visible rather than dropped. Use `b` for its raw bits.


Wire command: `w\p\t`

| Arg | Wire type |
|---|---|
| timeout_ms | dec |

Returns: format (dec), modulation (dec), id (hexbytes)

```python
dev.wireless.rfid.read_tag(timeout_ms: int) -> Result
```
```c
ow_status ow_wireless_rfid_read_tag(ow_device* dev, int32_t timeout_ms, int32_t* format, int32_t* modulation, uint8_t* id, size_t id_cap, size_t* id_len);
```
```rust
dev.wireless().rfid().read_tag(timeout_ms: i32) -> Result<(i32, i32, Vec<u8>), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.read_tag(timeout_ms)   # returns value; check dev.ok
```

## stream_tags

Stream Tags. Push each decoded tag to the host as an event

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Stream Tags

Emits each decoded tag as an event instead of requiring a poll.

Tags drain a few per pass rather than in a burst: the event FIFO is 24 slots and a busy field produces tags faster than the host drains them.


Wire command: `w\p\s`

| Arg | Wire type |
|---|---|
| enable | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.rfid.stream_tags(enable: int) -> Result
```
```c
ow_status ow_wireless_rfid_stream_tags(ow_device* dev, int32_t enable);
```
```rust
dev.wireless().rfid().stream_tags(enable: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.stream_tags(enable)   # check dev.ok
```

## clear_stats

Clear Stats. Zero the frame and tag counters

Wire command: `w\p\c`

Returns: none (Ok/Err only)

```python
dev.wireless.rfid.clear_stats() -> Result
```
```c
ow_status ow_wireless_rfid_clear_stats(ow_device* dev);
```
```rust
dev.wireless().rfid().clear_stats() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.clear_stats()   # check dev.ok
```

## tune

Tune Constant. Set a demodulator constant live, without reflashing

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Tune Constant

Sets one demodulator constant at runtime, without a reflash.

```
u <param> <value>
```

- `0` ASK bit period us (512 = RF/64)
- `1` ASK minimum pulse us
- `2` ASK swing divisor
- `3` ASK minimum hysteresis
- `4` PSK tolerance percent
- `5` PSK carrier cycles per bit

Values survive `r 0`/`r 1` and reset on power cycle.


Wire command: `w\p\u`

| Arg | Wire type |
|---|---|
| param | dec |
| value | dec |

Returns: param (decU32), value (decU32)

```python
dev.wireless.rfid.tune(param: int, value: int) -> Result
```
```c
ow_status ow_wireless_rfid_tune(ow_device* dev, int32_t param, int32_t value, int32_t* param_out, int32_t* value_out);
```
```rust
dev.wireless().rfid().tune(param: i32, value: i32) -> Result<(i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.tune(param, value)   # returns value; check dev.ok
```

## raw_bits

Raw Bits. Raw bits of the last assembled frame

#### Raw Bits

Returns the last 64-bit frame as assembled, before any decoder ran.

Useful when a tag is present and framing succeeds but nothing claims it: the raw bits separate a wrong format from a bit-timing error.


Wire command: `w\p\b`

Returns: modulation (dec), length (decU32), bits (hexbytes)

```python
dev.wireless.rfid.raw_bits() -> Result
```
```c
ow_status ow_wireless_rfid_raw_bits(ow_device* dev, int32_t* modulation, int32_t* length, uint8_t* bits, size_t bits_cap, size_t* bits_len);
```
```rust
dev.wireless().rfid().raw_bits() -> Result<(i32, i32, Vec<u8>), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.raw_bits()   # returns value; check dev.ok
```

## write_tag

Write Tag. Write one 32-bit block to a T5577/T5557 tag

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Write Tag

Writes one 32-bit block to a T5577/T5557 held in the field.

```
w <block 1-7> <hex>
```

The reader must be running. The write is blind: the tag never acknowledges, so success means the frame was transmitted, not accepted. Verify by reading it back.

Block 0 is the configuration block and is refused: a wrong value there is not recoverable by writing again.


Wire command: `w\p\w`

| Arg | Wire type |
|---|---|
| block | dec |
| value | hex |

Returns: result (dec), block (dec)

```python
dev.wireless.rfid.write_tag(block: int, value: int) -> Result
```
```c
ow_status ow_wireless_rfid_write_tag(ow_device* dev, int32_t block, uint32_t value, int32_t* result, int32_t* block_out);
```
```rust
dev.wireless().rfid().write_tag(block: i32, value: u32) -> Result<(i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.write_tag(block, value)   # returns value; check dev.ok
```

## carrier_info

Carrier Info. Measured carrier and PSK front-end telemetry

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Carrier Info

Measured carrier, clk_sys, envelope sample count and capture overruns.

Needs no tag. A climbing `env_samples` proves the ADC was claimed and the front end is being sampled. `frames` staying at zero against a bare carrier is correct.


Wire command: `w\p\i`

Returns: carrier_hz (decU32), psk_active (bool), psk_events (decU32), poll_count (decU32), clk_hz (decU32), clock_ok (bool), env_samples (decU32), overruns (decU32), restarts (decU32), psk_period (decU32)

```python
dev.wireless.rfid.carrier_info() -> Result
```
```c
ow_status ow_wireless_rfid_carrier_info(ow_device* dev, int32_t* carrier_hz, bool* psk_active, int32_t* psk_events, int32_t* poll_count, int32_t* clk_hz, bool* clock_ok, int32_t* env_samples, int32_t* overruns, int32_t* restarts, int32_t* psk_period);
```
```rust
dev.wireless().rfid().carrier_info() -> Result<(i32, bool, i32, i32, i32, bool, i32, i32, i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.carrier_info()   # returns value; check dev.ok
```

## enroll_id

Enroll ID. Write a caller-supplied EM4100 ID onto the card in the field

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Enroll ID

Encodes a 40-bit EM4100 id and writes it into blocks 1 and 2.

```
n A1 B2 C3 D4 E5
```

Five space-separated bytes, most significant first. The card must already be EM4100-configured (block 0 = 00148040); this firmware cannot write block 0, so it cannot convert a blank card.

Blind write. Verify with `t`.


Wire command: `w\p\n`

| Arg | Wire type |
|---|---|
| id | hexbytes |

Returns: none (Ok/Err only)

```python
dev.wireless.rfid.enroll_id(id: bytes | bytearray) -> Result
```
```c
ow_status ow_wireless_rfid_enroll_id(ow_device* dev, const uint8_t* id, size_t id_len);
```
```rust
dev.wireless().rfid().enroll_id(id: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.enroll_id(id)   # check dev.ok
```

## clone_capture

Clone Capture. Read a card and hold its ID for a later clone write

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Clone Capture

Reads the card on the coil and holds its id for `j`.

Held separately from the last-tag latch, so the reads `j` performs on the target card cannot overwrite it.


Wire command: `w\p\k`

| Arg | Wire type |
|---|---|
| timeout_ms | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.rfid.clone_capture(timeout_ms: int) -> Result
```
```c
ow_status ow_wireless_rfid_clone_capture(ow_device* dev, int32_t timeout_ms);
```
```rust
dev.wireless().rfid().clone_capture(timeout_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.clone_capture(timeout_ms)   # check dev.ok
```

## clone_write

Clone Write. Write the captured ID onto the card now on the coil

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

#### Clone Write

Writes the id captured by `k` onto the card now on the coil.

Refuses unless the target is currently reading as EM4100, since block 0 cannot be written and a non-EM4100 card would never broadcast the frame. Nothing is transmitted when refused.

Blind write. Verify with `t`.


Wire command: `w\p\j`

Returns: none (Ok/Err only)

```python
dev.wireless.rfid.clone_write() -> Result
```
```c
ow_status ow_wireless_rfid_clone_write(ow_device* dev);
```
```rust
dev.wireless().rfid().clone_write() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.rfid.clone_write()   # check dev.ok
```
