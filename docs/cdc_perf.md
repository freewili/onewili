# CDC Serial Performance

`dev.io.cdc_perf` - wire path `i\y` - generated from `fwMenuCdcPerf`.

## cdc_perf_blast

Blast. Streams Bytes of deterministic XORshift32 pattern data device-to-host as fast as possible in Chunk-sized writes. Prints the line <<<BLAST>>> before the raw binary begins. Returns bytes,elapsed_us,crc32 (CRC-32 of the payload)

Wire command: `i\y\b`

| Arg | Wire type |
|---|---|
| bytes | dec |
| chunk | dec |

Returns: bytes (dec), elapsed_us (dec), crc32 (hex)

```python
dev.io.cdc_perf.cdc_perf_blast(bytes: int, chunk: int) -> Result
```
```c
ow_status ow_io_cdc_perf_cdc_perf_blast(ow_device* dev, int32_t bytes, int32_t chunk, int32_t* bytes_out, int32_t* elapsed_us, uint32_t* crc32);
```
```rust
dev.io().cdc_perf().cdc_perf_blast(bytes: i32, chunk: i32) -> Result<(i32, i32, u32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.cdc_perf.cdc_perf_blast(bytes, chunk)   # returns value; check dev.ok
```

## cdc_perf_sink

Sink. Receives exactly Bytes of raw binary host-to-device and CRC-32-accumulates them. Prints the line <<<SINK>>> when ready to receive. A 10 second inactivity timeout aborts with failure. Returns bytes,elapsed_us,crc32

Wire command: `i\y\s`

| Arg | Wire type |
|---|---|
| bytes | dec |

Returns: bytes (dec), elapsed_us (dec), crc32 (hex)

```python
dev.io.cdc_perf.cdc_perf_sink(bytes: int) -> Result
```
```c
ow_status ow_io_cdc_perf_cdc_perf_sink(ow_device* dev, int32_t bytes, int32_t* bytes_out, int32_t* elapsed_us, uint32_t* crc32);
```
```rust
dev.io().cdc_perf().cdc_perf_sink(bytes: i32) -> Result<(i32, i32, u32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.cdc_perf.cdc_perf_sink(bytes)   # returns value; check dev.ok
```

## cdc_perf_echo

Echo. Per round reads exactly Chunk raw bytes from the host then writes them back verbatim, Rounds times. Prints the line <<<ECHO>>> when ready for round 1. A 10 second inactivity timeout aborts with failure. Returns rounds,elapsed_us

Wire command: `i\y\e`

| Arg | Wire type |
|---|---|
| rounds | dec |
| chunk | dec |

Returns: rounds (dec), elapsed_us (dec)

```python
dev.io.cdc_perf.cdc_perf_echo(rounds: int, chunk: int) -> Result
```
```c
ow_status ow_io_cdc_perf_cdc_perf_echo(ow_device* dev, int32_t rounds, int32_t chunk, int32_t* rounds_out, int32_t* elapsed_us);
```
```rust
dev.io().cdc_perf().cdc_perf_echo(rounds: i32, chunk: i32) -> Result<(i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.cdc_perf.cdc_perf_echo(rounds, chunk)   # returns value; check dev.ok
```
