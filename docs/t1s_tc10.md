# TC10 Wake/Sleep

`dev.io.t1s.tc10` - wire path `i\r\w` - generated from `fwMenuT1STc10`.

## t1s_tc10_generate_wake

Generate Wake. Emits a TC10 wake-up from the running LAN865x: a 1 ms DME wake burst onto the MDI (Forward to MDI) and/or a 90 us pulse on the WAKE_OUT pin (Forward to WAKE_OUT), per the settings below. The engine polls the PHY until the request completes (see Wake Status gen/done/timeout). Fails when the PHY is not in run, neither forward target is enabled, or a previous wake is still busy

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\g`

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.t1s_tc10_generate_wake() -> Result
```
```c
ow_status ow_io_t1s_tc10_t1s_tc10_generate_wake(ow_device* dev);
```
```rust
dev.io().t1s().tc10().t1s_tc10_generate_wake() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.t1s_tc10_generate_wake()   # check dev.ok
```

## t1s_tc10_wake_status

Wake Status. Prints one line of key=value TC10 status: state (engine state, sleep while asleep) gen done busy timeout (wake generations requested/completed/in flight/expired) sleeps woke pulses (sleep entries, wake detections, local WAKE_IN pulses) src (last wake source: none/mdi/wakein/mdi+wakein) sts2 (raw PHY STS2) fwd wake (forward targets and wake sources as configured) inhdly (INH release delay code) sleepms (ms asleep, 0 when awake). Wire-parseable, append-only

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\s`

Returns: status (string)

```python
dev.io.t1s.tc10.t1s_tc10_wake_status() -> Result
```
```c
ow_status ow_io_t1s_tc10_t1s_tc10_wake_status(ow_device* dev, char* status, size_t status_cap);
```
```rust
dev.io().t1s().tc10().t1s_tc10_wake_status() -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.t1s_tc10_wake_status()   # returns value; check dev.ok
```

## t1s_tc10_enter_sleep

Enter Sleep. Puts the LAN865x into TC10 sleep with the configured wake sources (Wake on MDI / Wake on WAKE_IN) and forward targets. On Orca the PHY's INH output then cuts its own SPI/IRQ path, so the engine tears the link down after a short grace and parks in the sleep state until the chip wakes (MDI energy, a WAKE_IN pulse, Local Wake Pulse) or Cancel Sleep reinits it. Fails when the PHY is not in run or a sleep is already pending

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\e`

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.t1s_tc10_enter_sleep() -> Result
```
```c
ow_status ow_io_t1s_tc10_t1s_tc10_enter_sleep(ow_device* dev);
```
```rust
dev.io().t1s().tc10().t1s_tc10_enter_sleep() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.t1s_tc10_enter_sleep()   # check dev.ok
```

## t1s_tc10_local_wake_pulse

Local Wake Pulse. Drives a 200 us HIGH pulse on the PHY's WAKE_IN pin from the board's IO expander (which stays powered while the PHY sleeps). Only a sleeping PHY reacts (it wakes and the engine reinitializes it); harmless when awake. Fails if the expander is not configured or the I2C write fails

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\w`

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.t1s_tc10_local_wake_pulse() -> Result
```
```c
ow_status ow_io_t1s_tc10_t1s_tc10_local_wake_pulse(ow_device* dev);
```
```rust
dev.io().t1s().tc10().t1s_tc10_local_wake_pulse() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.t1s_tc10_local_wake_pulse()   # check dev.ok
```

## t1s_tc10_cancel_sleep

Cancel Sleep. Abandons a pending sleep or leaves the sleep state by requesting a full PHY reinit (RST pulse + fresh init). Reports 'not sleeping' when no sleep is in progress

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\c`

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.t1s_tc10_cancel_sleep() -> Result
```
```c
ow_status ow_io_t1s_tc10_t1s_tc10_cancel_sleep(ow_device* dev);
```
```rust
dev.io().t1s().tc10().t1s_tc10_cancel_sleep() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.t1s_tc10_cancel_sleep()   # check dev.ok
```

## forward_to_mdi

Forward to MDI. When on, Generate Wake (and a wake forwarded during sleep) puts a 1 ms wake burst onto the MDI so the far end of the T1S segment wakes. At least one of Forward to MDI / Forward to WAKE_OUT must be on for Generate Wake to do anything

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\m`

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.forward_to_mdi() -> Result
```
```c
ow_status ow_io_t1s_tc10_forward_to_mdi(ow_device* dev);
```
```rust
dev.io().t1s().tc10().forward_to_mdi() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.forward_to_mdi()   # check dev.ok
```

## forward_to_wakeout

Forward to WAKE_OUT. When on, Generate Wake (and a wake forwarded during sleep) emits a 90 us pulse on the PHY's WAKE_OUT pin (routed to the header on Orca; the host cannot observe it). At least one of Forward to MDI / Forward to WAKE_OUT must be on for Generate Wake to do anything

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\o`

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.forward_to_wakeout() -> Result
```
```c
ow_status ow_io_t1s_tc10_forward_to_wakeout(ow_device* dev);
```
```rust
dev.io().t1s().tc10().forward_to_wakeout() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.forward_to_wakeout()   # check dev.ok
```

## wake_on_mdi

Wake on MDI. When on, a sleeping PHY wakes on energy detected on the MDI (any activity, not only a TC10 wake burst). Applied at the next Enter Sleep

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\a`

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.wake_on_mdi() -> Result
```
```c
ow_status ow_io_t1s_tc10_wake_on_mdi(ow_device* dev);
```
```rust
dev.io().t1s().tc10().wake_on_mdi() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.wake_on_mdi()   # check dev.ok
```

## wake_on_wakein

Wake on WAKE_IN. When on, a sleeping PHY wakes on a HIGH pulse longer than 40 us on its WAKE_IN pin (Local Wake Pulse drives that pin from the IO expander). Applied at the next Enter Sleep

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\n`

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.wake_on_wakein() -> Result
```
```c
ow_status ow_io_t1s_tc10_wake_on_wakein(ow_device* dev);
```
```rust
dev.io().t1s().tc10().wake_on_wakein() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.wake_on_wakein()   # check dev.ok
```

## sleep_inhibit_delay

Sleep Inhibit Delay. Delay before the PHY releases its INH output after entering sleep: 0 = 0 ms, 1 = 50 ms, 2 = 100 ms, 3 = 200 ms. On Orca INH powers the PHY's SPI/IRQ path, so this is how long the link stays reachable after Enter Sleep. Applied at the next Enter Sleep

Requires power zone 6 (FPGA). See [Errors](errors.md).

Wire command: `i\r\w\y`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.tc10.sleep_inhibit_delay(value: int) -> Result
```
```c
ow_status ow_io_t1s_tc10_sleep_inhibit_delay(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().tc10().sleep_inhibit_delay(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.tc10.sleep_inhibit_delay(value)   # check dev.ok
```
