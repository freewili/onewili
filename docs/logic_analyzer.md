# Logic Analyzer Functions

`dev.io.logic_analyzer` - wire path `i\b` - generated from `fwMenuLogicAnalyzer`.

## setup_logic_analyzer

configure. Configures the logic analyzer capture.

Requires power zone 6 (FPGA). See [Errors](errors.md).

#### Configure Logic Analyzer

Configures a digital logic capture on a contiguous range of GPIO pins, with an optional edge trigger and auto-rearm.

##### Arguments

| # | Name | Description |
|---|------|-------------|
| 1 | `sampleRateNs` | Sample period in **nanoseconds** (time between samples). Smaller = faster. |
| 2 | `sampleCount` | Total number of samples to capture per run. |
| 3 | `pinStart` | First GPIO pin in the capture range (inclusive). |
| 4 | `pinStop` | Last GPIO pin in the capture range (inclusive). Bits per sample = `pinStop - pinStart + 1`. |
| 5 | `triggerPin` | GPIO pin used as the trigger source. |
| 6 | `triggerType` | Trigger mode (see below). |
| 7 | `rearm` | `0` = single-shot, `1` = automatically rearm after each capture. |

##### Trigger Types

- `0` — Falling edge on `triggerPin`
- `1` — Rising edge on `triggerPin`
- `2` — None (one-shot, fires immediately on `start`)
- `3` — Continuous (free-running capture)

##### Returns

- `success` — `true` if the configuration was accepted, `false` otherwise.

##### Example

Capture 4096 samples on GPIO 0–7 at 1 µs/sample, triggering on a rising edge of GPIO 2, with auto-rearm enabled:

```
c 1000 4096 0 7 2 1 1
```

##### Notes

- Issue `s` (start) after configuring to begin capture, and `e` (stop) to halt.
- For analog capture, configure separately with `a`.

Wire command: `i\b\c`

| Arg | Wire type |
|---|---|
| sample_rate_ns | decS32 |
| sample_count | decS32 |
| pin_start | decS32 |
| pin_stop | decS32 |
| trigger_pin | decS32 |
| trigger_type | decS32 |
| rearm | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.logic_analyzer.setup_logic_analyzer(sample_rate_ns: int, sample_count: int, pin_start: int, pin_stop: int, trigger_pin: int, trigger_type: int, rearm: int) -> Result
```
```c
ow_status ow_io_logic_analyzer_setup_logic_analyzer(ow_device* dev, int32_t sample_rate_ns, int32_t sample_count, int32_t pin_start, int32_t pin_stop, int32_t trigger_pin, int32_t trigger_type, int32_t rearm);
```
```rust
dev.io().logic_analyzer().setup_logic_analyzer(sample_rate_ns: i32, sample_count: i32, pin_start: i32, pin_stop: i32, trigger_pin: i32, trigger_type: i32, rearm: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_analyzer.setup_logic_analyzer(sample_rate_ns, sample_count, pin_start, pin_stop, trigger_pin, trigger_type, rearm)   # check dev.ok
```

## setup_analog

configure analog. Configures the analog capture inputs.

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\b\a`

| Arg | Wire type |
|---|---|
| analog_mask | decS32 |
| analog_rate_ns | decS32 |
| analog_res | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.logic_analyzer.setup_analog(analog_mask: int, analog_rate_ns: int, analog_res: int) -> Result
```
```c
ow_status ow_io_logic_analyzer_setup_analog(ow_device* dev, int32_t analog_mask, int32_t analog_rate_ns, int32_t analog_res);
```
```rust
dev.io().logic_analyzer().setup_analog(analog_mask: i32, analog_rate_ns: i32, analog_res: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_analyzer.setup_analog(analog_mask, analog_rate_ns, analog_res)   # check dev.ok
```

## start

start. Starts logic analyzer capture.

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\b\s`

Returns: none (Ok/Err only)

```python
dev.io.logic_analyzer.start() -> Result
```
```c
ow_status ow_io_logic_analyzer_start(ow_device* dev);
```
```rust
dev.io().logic_analyzer().start() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_analyzer.start()   # check dev.ok
```

## stop

stop. Stops logic analyzer capture.

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\b\e`

Returns: none (Ok/Err only)

```python
dev.io.logic_analyzer.stop() -> Result
```
```c
ow_status ow_io_logic_analyzer_stop(ow_device* dev);
```
```rust
dev.io().logic_analyzer().stop() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_analyzer.stop()   # check dev.ok
```

## trigger

trigger. Manually triggers the logic analyzer.

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\b\t`

| Arg | Wire type |
|---|---|
| trigger_type | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.logic_analyzer.trigger(trigger_type: int) -> Result
```
```c
ow_status ow_io_logic_analyzer_trigger(ow_device* dev, int32_t trigger_type);
```
```rust
dev.io().logic_analyzer().trigger(trigger_type: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.logic_analyzer.trigger(trigger_type)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `logicAnalyzerReport` (binary)

Logic analyzer capture report (binary API; header then digital + analog samples)

Header type: 2 - payload struct `apiFrame_LogicAnalyzerReport` (44 bytes):

| Field | C type | Offset |
|---|---|---|
| ui64TriggerTimeStampNs | uint64_t | 0 |
| uiSampleRateNs | uint32_t | 8 |
| uiGPIOStartPin | uint8_t | 12 |
| uiBitsPerSample | uint8_t | 13 |
| uiTriggerType | uint8_t | 14 |
| uiDummy | uint8_t | 15 |
| uiTriggerLocation | uint32_t | 16 |
| uiBufferHead | uint32_t | 20 |
| uiAnalogChannelMask | uint8_t | 24 |
| uiAnalogResolution | uint8_t | 25 |
| uiAnalogChannelCount | uint8_t | 26 |
| uiAnalogDummy | uint8_t | 27 |
| uiAnalogSampleRateNs | uint32_t | 28 |
| uiAnalogSampleCount | uint32_t | 32 |
| uiAnalogBufferHead | uint32_t | 36 |
| uiAnalogTriggerLocation | uint32_t | 40 |

Receive it:

```python
evt = dev.binary_events.get()   # LogicAnalyzerReportEvent
```
```c
ow_event ev;
if (ow_binary_poll(&bdev, &ev) == 1 && ev.kind == OW_EV_LOGIC_ANALYZER_REPORT)
    { /* ev.u.logic_analyzer_report.<field> */ }
```
```rust
if let Some(onewili::Event::LogicAnalyzerReport(e)) = dev.poll_event()? { /* e.<field> */ }
```
