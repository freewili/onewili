# Ethernet Test

`dev.io.t1s.eth_test` - wire path `i\r\t` - generated from `fwMenuEthTest`.

## eth_test_start_periodic

Start Periodic. Starts the test-frame generator sending one frame every Period us (see setting u). Frames use the current Frame Size/Type/CRC settings and the destination MAC from command m. Refused while Loopback is on or on a build without the NCM stack

Wire command: `i\r\t\p`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.eth_test_start_periodic() -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_start_periodic(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().eth_test_start_periodic() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_start_periodic()   # check dev.ok
```

## eth_test_start_flood

Start Flood. Starts the test-frame generator sending as fast as the USB link accepts (natural NTB backpressure paces it; submit failures are counted, not lost sequence numbers). Refused while Loopback is on

Wire command: `i\r\t\f`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.eth_test_start_flood() -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_start_flood(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().eth_test_start_flood() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_start_flood()   # check dev.ok
```

## eth_test_start_line_rate

Start Line Rate. Starts the test-frame generator at Line Rate % (setting e) of a 10 Mbit/s reference wire, using a token bucket that charges each frame its size plus 24 bytes of preamble/FCS/gap overhead. Refused while Loopback is on

Wire command: `i\r\t\r`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.eth_test_start_line_rate() -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_start_line_rate(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().eth_test_start_line_rate() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_start_line_rate()   # check dev.ok
```

## eth_test_start_burst

Start Burst. Starts the test-frame generator releasing Burst Count frames (setting n) every second, the first burst immediately. Refused while Loopback is on

Wire command: `i\r\t\b`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.eth_test_start_burst() -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_start_burst(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().eth_test_start_burst() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_start_burst()   # check dev.ok
```

## eth_test_send_count

Send N Frames. Sends exactly Count test frames as fast as the link accepts, then stops by itself (Count 1 = one transmit). Counters keep running so the result can be read with Show Stats afterwards. Refused while Loopback is on

Wire command: `i\r\t\o`

| Arg | Wire type |
|---|---|
| count | dec |

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.eth_test_send_count(count: int) -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_send_count(ow_device* dev, int32_t count);
```
```rust
dev.io().t1s().eth_test().eth_test_send_count(count: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_send_count(count)   # check dev.ok
```

## eth_test_stop

Stop. Stops the test-frame generator. Counters are kept (use Clear Stats to zero them); the responder and loopback settings are unaffected

Wire command: `i\r\t\x`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.eth_test_stop() -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_stop(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().eth_test_stop() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_stop()   # check dev.ok
```

## eth_test_show_stats

Show Stats. Prints one line of key=value counters: mode link TXf TXb TXfail TXfps TXkbps RXf RXb RXfps RXkbps gap lost crc under over other echoq echos echod. The fps/kbps values are 1 Hz rates; RXf counts received FWET test frames, other counts everything else (host OS chatter)

Wire command: `i\r\t\s`

Returns: stats (string)

```python
dev.io.t1s.eth_test.eth_test_show_stats() -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_show_stats(ow_device* dev, char* stats, size_t stats_cap);
```
```rust
dev.io().t1s().eth_test().eth_test_show_stats() -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_show_stats()   # returns value; check dev.ok
```

## eth_test_clear_stats

Clear Stats. Zeros every TX/RX/echo counter and restarts sequence-gap tracking. The generator, responder and link state are unaffected

Wire command: `i\r\t\c`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.eth_test_clear_stats() -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_clear_stats(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().eth_test_clear_stats() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_clear_stats()   # check dev.ok
```

## eth_test_set_dest_mac

Set Dest MAC. Sets the destination MAC for generated test frames (default FF FF FF FF FF FF broadcast). Takes effect at the next generator start. Not persisted across reboot

Wire command: `i\r\t\m`

| Arg | Wire type |
|---|---|
| dest_mac | hexbytes |

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.eth_test_set_dest_mac(dest_mac: bytes | bytearray) -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_set_dest_mac(ow_device* dev, const uint8_t* dest_mac, size_t dest_mac_len);
```
```rust
dev.io().t1s().eth_test().eth_test_set_dest_mac(dest_mac: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_set_dest_mac(dest_mac)   # check dev.ok
```

## eth_test_link_status

Link Status. Reports whether the USB network adapter is up (host selected the NCM data interface) plus the host-side MAC, device-side MAC and the device's static IP 10.55.0.2

Wire command: `i\r\t\k`

Returns: info (string)

```python
dev.io.t1s.eth_test.eth_test_link_status() -> Result
```
```c
ow_status ow_io_t1s_eth_test_eth_test_link_status(ow_device* dev, char* info, size_t info_cap);
```
```rust
dev.io().t1s().eth_test().eth_test_link_status() -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.eth_test_link_status()   # returns value; check dev.ok
```

## frame_size

Frame Size. Total Ethernet frame size in bytes for generated test frames (headers included, FCS excluded). The udp frame type needs at least 66 bytes for its headers and is raised to that silently

Wire command: `i\r\t\i`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.frame_size(value: int) -> Result
```
```c
ow_status ow_io_t1s_eth_test_frame_size(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().eth_test().frame_size(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.frame_size(value)   # check dev.ok
```

## period_us

Period us. Microseconds between frames in Periodic mode (10000 = 100 frames per second)

Wire command: `i\r\t\u`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.period_us(value: int) -> Result
```
```c
ow_status ow_io_t1s_eth_test_period_us(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().eth_test().period_us(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.period_us(value)   # check dev.ok
```

## burst_count

Burst Count. Frames released in each one-second burst in Burst mode

Wire command: `i\r\t\n`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.burst_count(value: int) -> Result
```
```c
ow_status ow_io_t1s_eth_test_burst_count(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().eth_test().burst_count(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.burst_count(value)   # check dev.ok
```

## line_rate_percent

Line Rate Percent. Percentage of the 10 Mbit/s reference wire rate for Line Rate mode

Wire command: `i\r\t\e`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.line_rate_percent(value: int) -> Result
```
```c
ow_status ow_io_t1s_eth_test_line_rate_percent(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().eth_test().line_rate_percent(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.line_rate_percent(value)   # check dev.ok
```

## payload_crc

Payload CRC. When on, each generated frame carries a CRC32 over its sequence/timestamp/fill so the host can prove payload integrity; costs a CRC pass per frame at high rates

Wire command: `i\r\t\v`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.payload_crc() -> Result
```
```c
ow_status ow_io_t1s_eth_test_payload_crc(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().payload_crc() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.payload_crc()   # check dev.ok
```

## responder

Responder. When on, the device answers as 10.55.0.2: ARP requests, ICMP echo (ping) and UDP echo on port 5556. Turn off to measure pure generator/counter behavior. (Served by lwIP when compiled in -- FW2MAIN_LWIP builds answer through the Network (TCP/IP) menu's stack and this toggle only drives the legacy mini-responder on non-lwIP builds)

Wire command: `i\r\t\a`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.responder() -> Result
```
```c
ow_status ow_io_t1s_eth_test_responder(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().responder() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.responder()   # check dev.ok
```

## loopback

Loopback. When on, EVERY received frame is echoed back with its MAC addresses swapped and the generator/responder are disabled (mutually exclusive). Always off after a reboot

Wire command: `i\r\t\l`

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.loopback() -> Result
```
```c
ow_status ow_io_t1s_eth_test_loopback(ow_device* dev);
```
```rust
dev.io().t1s().eth_test().loopback() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.loopback()   # check dev.ok
```

## frame_type

Frame Type. Carrier for generated test frames: raw = ethertype 0x88B5 (needs npcap/scapy on the host), udp = IPv4 broadcast 10.55.0.255 port 5555 (a plain host socket receives it)

Wire command: `i\r\t\t`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.eth_test.frame_type(value: int) -> Result
```
```c
ow_status ow_io_t1s_eth_test_frame_type(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().eth_test().frame_type(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.eth_test.frame_type(value)   # check dev.ok
```
