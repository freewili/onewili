# Analog Out & Trigger Functions

`dev.io.analog_out` - wire path `i\a` - generated from `fwMenuAnalogOut`.

## set_analog_output

Set Analog Output. sets the voltage of an analog output 0 or 1. ch 2 and 3 are use for window comparator

Requires power zone 11 (Analog). See [Errors](errors.md).

#### Set Analog Output

Sets the voltage on an analog output channel.

##### Usage

```
s <channel> <value>
```

##### Arguments

| Name | Type | Units | Range | Notes |
|------|------|-------|-------|-------|
| `channel` | int | - | 0 - 3 | 0, 1: analog output voltages; 2, 3: window comparator thresholds (low, high) |
| `value` | float | Volts | 0.0 - 4.84 | 4x internal-reference full scale; higher values clamp |

##### Examples

```
s 0 3.3     # Set analog output 0 to 3.3 V
s 0 0       # Stop any waveform on channel 0 and hold 0 V
s 2 1.0     # Set window comparator low threshold to 1.0 V
s 3 4.0     # Set window comparator high threshold to 4.0 V
```

Wire command: `i\a\s`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| value | float |

Returns: none (Ok/Err only)

```python
dev.io.analog_out.set_analog_output(channel: int, value: float) -> Result
```
```c
ow_status ow_io_analog_out_set_analog_output(ow_device* dev, int32_t channel, double value);
```
```rust
dev.io().analog_out().set_analog_output(channel: i32, value: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_out.set_analog_output(channel, value)   # check dev.ok
```

## set_trigger_window

Set Trigger Window. Trigger will be 1 when TrigV is between V- and V+.

Requires power zone 11 (Analog). See [Errors](errors.md).

#### Set Trigger Window

Configures a **window comparator** on the `Trig IN/VREF` input (pin 4 of the FreeWili 2 20-pin connector). The comparator drives an internal digital signal that can be used as a trigger source for the **Logic Analyzer** and **Logic Player**.

##### Behavior

The digital trigger output is:

- **High (1)** when `valueLow ≤ TrigV ≤ valueHigh`
- **Low (0)** when `TrigV` is outside the window

##### Arguments

| Name | Type | Units | Range |
|------|------|-------|-------|
| `valueLow` | float | Volts | 0.0 – 5.0 |
| `valueHigh` | float | Volts | 0.0 – 5.0 |

###### Constraints

- `valueLow` **must be less than** `valueHigh`.
- If `valueLow >= valueHigh`, the window is invalid and the trigger output will be **stuck fixed** (always high or always low) — no triggering will occur.

##### Returns

A basic status response indicating success or failure.

##### ⚠️ Pin Sharing Warning

The `Trig IN/VREF` pin is **shared** with the CANFD special-function pins:

- Software **CAN Rx**
- **CANFD Int**

Wire command: `i\a\t`

| Arg | Wire type |
|---|---|
| value_low | float |
| value_high | float |

Returns: none (Ok/Err only)

```python
dev.io.analog_out.set_trigger_window(value_low: float, value_high: float) -> Result
```
```c
ow_status ow_io_analog_out_set_trigger_window(ow_device* dev, double value_low, double value_high);
```
```rust
dev.io().analog_out().set_trigger_window(value_low: f64, value_high: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_out.set_trigger_window(value_low, value_high)   # check dev.ok
```

## set_enable_trigger

Enable Trigger. Enables the Trigger Input to CPU

Requires power zone 11 (Analog). See [Errors](errors.md).

#### Enable Trigger

Routes the **window comparator** output from the `Trig IN/VREF` pin (pin 4 of the FreeWili 2 20-pin connector) onto the CPU's internal **GPIO40** trigger input.

Once enabled, the comparator signal configured by `t` (Set Trigger Window) becomes available to the **Logic Analyzer** and **Logic Player** as a trigger source.

##### Usage

```
e
```

No arguments.

##### Behavior

- Selects `analogtrigger` as the input feature for GPIO40 via the I/O expander.
- The digital state at GPIO40 will then reflect the window comparator:
  - **1** when `valueLow ≤ TrigV ≤ valueHigh`
  - **0** when `TrigV` is outside the window

##### Prerequisites

1. Set the comparator thresholds first with `t <valueLow> <valueHigh>`.
2. Apply the analog signal to monitor on the `Trig IN/VREF` pin.

##### Returns

A basic status response indicating success or failure.

##### ⚠️ Pin Sharing Warning

GPIO40 and the `Trig IN/VREF` pin are **shared** with CANFD special-function pins (**Software CAN Rx**, **CANFD Int**). Enabling the trigger will override those features on this pin.

Wire command: `i\a\e`

Returns: none (Ok/Err only)

```python
dev.io.analog_out.set_enable_trigger() -> Result
```
```c
ow_status ow_io_analog_out_set_enable_trigger(ow_device* dev);
```
```rust
dev.io().analog_out().set_enable_trigger() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_out.set_enable_trigger()   # check dev.ok
```

## set_v_prog_vout

Set Programmable VOut. Sets the programmable VOut: enable then target voltage.

Requires power zone 11 (Analog). See [Errors](errors.md).

#### Set Programmable VOut

Controls the on-board **programmable output supply** (`VOut`) on the FreeWili 2. This rail can source up to **1.5 A** at a software-selected voltage between **1.0 V and 5.5 V**, and is also the source used by the `g` (Glitch Programmable VOut) command.

##### Usage

```
u <enable> [setVoltage]
```

##### Arguments

| Name | Type | Units | Range | Notes |
|------|------|-------|-------|-------|
| `enable` | int | — | 0 or 1 | 0 = disable VOut, 1 = enable VOut |
| `setVoltage` | float | Volts | 1.0 – 5.5 | Only required (and used) when `enable = 1` |

##### Behavior

- **`u 0`** — Disables the `VOut` rail. No voltage is provided. `setVoltage` is ignored.
- **`u 1 <setVoltage>`** — Enables the rail (if not already enabled) and programs the supply to `setVoltage` volts.
  - If the rail was previously disabled, a brief settling delay is inserted and the target voltage is written twice to ensure the regulator latches in cleanly.
  - If the rail was already enabled, the new voltage is applied immediately.

##### Examples

```
u 0           # turn VOut off
u 1 3.3       # enable VOut and set it to 3.3 V
u 1 5.0       # enable VOut and set it to 5.0 V
```

##### Returns

A basic status response indicating success or failure of the command.

##### Related Commands

- `g <nanoSeconds>` — Briefly glitches `VOut` low (for fault-injection experiments).
- `p` (menu state) — Shows the current `VOut` enable state and the most recently programmed voltage.

##### ⚠️ Notes

- Make sure the load connected to `VOut` is rated for the selected voltage **before** enabling.
- Switching `VOut` on or changing voltage can briefly perturb attached devices; power-cycle-sensitive targets should be designed accordingly.

Wire command: `i\a\u`

| Arg | Wire type |
|---|---|
| enable | decS32 |
| set_voltage | float |

Returns: none (Ok/Err only)

```python
dev.io.analog_out.set_v_prog_vout(enable: int, set_voltage: float) -> Result
```
```c
ow_status ow_io_analog_out_set_v_prog_vout(ow_device* dev, int32_t enable, double set_voltage);
```
```rust
dev.io().analog_out().set_v_prog_vout(enable: i32, set_voltage: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_out.set_v_prog_vout(enable, set_voltage)   # check dev.ok
```

## set_glitch

Glitch Programmable VOut. Briefly glitches the programmable VOut for the given nanoseconds.

Requires power zone 11 (Analog). See [Errors](errors.md).

#### Glitch Programmable VOut

Triggers a brief **voltage glitch** on the on-board programmable `VOut` rail by activating the MOSFET crowbar that pulls the rail toward ground for approximately the requested number of nanoseconds. Intended for **fault-injection / voltage-glitching** experiments on a target powered from `VOut`.

##### Usage

```
g <nanoSeconds>
```

##### Arguments

| Name | Type | Units | Range |
|------|------|-------|-------|
| `nanoSeconds` | int | nanoseconds | 10 – 2000 |

> The pulse width is **approximate**, not exact. Actual glitch duration depends on MOSFET switching time, board parasitics, and the load on `VOut`.

##### Behavior

- A single short low-side pulse is generated on the `VOut` rail.
- The rail returns to its previously programmed voltage after the pulse.
- No change is made to the `VOut` enable state or programmed voltage setting.

##### Prerequisites

1. Enable and program `VOut` first with `u 1 <setVoltage>` (1.0 – 5.5 V).
2. Connect the target device to `VOut`.

##### Examples

```
g 50      # ~50 ns glitch pulse on VOut
g 250     # ~250 ns glitch pulse on VOut
g 2000    # ~2 µs glitch pulse on VOut
```

##### Returns

A basic status response indicating whether the glitch command was accepted.

##### Related Commands

- `u <enable> [setVoltage]` — Enable / set the programmable `VOut` voltage.
- `p` (menu state) — Shows the current `VOut` enable state and programmed voltage.

##### ⚠️ Warnings

- Voltage glitching can **reset, corrupt, or permanently damage** the connected target. Only glitch devices you are willing to risk.
- The crowbar briefly shorts `VOut` low — ensure the load and any series/decoupling components can tolerate the transient.
- Avoid long or repeated pulses near the upper end of the range, especially at higher `VOut` voltages and currents.

Wire command: `i\a\g`

| Arg | Wire type |
|---|---|
| nano_seconds | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.analog_out.set_glitch(nano_seconds: int) -> Result
```
```c
ow_status ow_io_analog_out_set_glitch(ow_device* dev, int32_t nano_seconds);
```
```rust
dev.io().analog_out().set_glitch(nano_seconds: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_out.set_glitch(nano_seconds)   # check dev.ok
```

## set_waveform

Set Waveform. Configures and starts the DAC63204 function generator on analog output 0 or 1.

Requires power zone 11 (Analog). See [Errors](errors.md).

#### Set Waveform

Configures the **DAC63204 function generator** on analog output `0` or `1` and starts it. Channels 2 and 3 are the trigger-window comparator thresholds and are rejected.

##### Usage

```
w <channel> <waveform> <frequencyHz> <lowVoltage> <highVoltage> <phase>
```

##### Arguments

| Name | Type | Units | Range |
|------|------|-------|-------|
| `channel` | int | - | 0 or 1 |
| `waveform` | enum | - | 0 off, 1 triangle, 2 sawtooth, 3 inverse sawtooth, 4 sine |
| `frequencyHz` | float | Hz | see below |
| `lowVoltage` | float | Volts | 0.0 - 4.84 |
| `highVoltage` | float | Volts | 0.0 - 4.84 |
| `phase` | enum | - | 0 = 0 deg, 1 = 120 deg, 2 = 240 deg, 3 = 90 deg |

`waveform 0` stops the channel and returns it to DC mode.

##### Frequency

The hardware generates waveforms by stepping the output at one of 15 slew rates in one of 8 code-step sizes, so only a discrete set of frequencies exists. The firmware picks the closest and prints both the requested and the actual value.

- **Sine**: 8.13 Hz to 10.42 kHz, 15 evenly-spread steps. Sine plays a fixed 24-point table, so `lowVoltage` and `highVoltage` do not set its amplitude - they select the nearest output gain (1.5x, 2x, 3x or 4x of the 1.21 V internal reference).
- **Triangle**: roughly 0.024 Hz to 977 Hz at full amplitude, up to about 62 kHz on a narrow span.
- **Sawtooth / inverse sawtooth**: twice the triangle rate for the same settings.

A precise high frequency costs amplitude: near the top of the range the slew rate is already at its 4 us minimum and only the code step can move, in 2x jumps.

##### Notes

- Writing a DC voltage to the channel with `s` stops the waveform (last writer wins).
- `phase` is documented by the datasheet for the sine wave only; for the ramp shapes the value is written to the register but its effect is unspecified.
- There is no square wave - the hardware does not generate one.

##### Related Commands

- `x <mask>` - start or stop both channels in one write, edge-aligned.
- `s <channel> <value>` - set a static DC voltage (stops any waveform).
- `p` - shows the current waveform state for both channels.

Wire command: `i\a\w`

| Arg | Wire type |
|---|---|
| channel | decS32 |
| waveform | [dacWaveShapeMenu](enums.md#dacwaveshapemenu) |
| frequency_hz | float |
| low_voltage | float |
| high_voltage | float |
| phase | [dacWavePhase](enums.md#dacwavephase) |

Returns: none (Ok/Err only)

```python
dev.io.analog_out.set_waveform(channel: int, waveform: enums.dacWaveShapeMenu | int, frequency_hz: float, low_voltage: float, high_voltage: float, phase: enums.dacWavePhase | int) -> Result
```
```c
ow_status ow_io_analog_out_set_waveform(ow_device* dev, int32_t channel, ow_dac_wave_shape_menu waveform, double frequency_hz, double low_voltage, double high_voltage, ow_dac_wave_phase phase);
```
```rust
dev.io().analog_out().set_waveform(channel: i32, waveform: dacWaveShapeMenu, frequency_hz: f64, low_voltage: f64, high_voltage: f64, phase: dacWavePhase) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_out.set_waveform(channel, waveform, frequency_hz, low_voltage, high_voltage, phase)   # check dev.ok
```

## set_waveform_run

Waveform Run/Stop. Starts or stops the configured waveforms on analog outputs 0 and 1 in a single write.

Requires power zone 11 (Analog). See [Errors](errors.md).

#### Waveform Run/Stop

Sets the run state of the DAC63204 function generator on analog outputs `0` and `1` with **one** `COMMON-DAC-TRIG` register write, so channels started together start on the same edge.

##### Usage

```
x <mask>
```

##### Arguments

| Name | Type | Range | Meaning |
|------|------|-------|---------|
| `mask` | int | 0 - 3 | bit 0 = channel 0, bit 1 = channel 1 |

```
x 0     stop both channels
x 1     run channel 0 only
x 2     run channel 1 only
x 3     run both channels, edge-aligned
```

Channels with no waveform configured are masked out, so their start bit is never written.

##### Prerequisites

Configure each channel first with `w <channel> <waveform> <frequencyHz> <lowVoltage> <highVoltage> <phase>`. `w` also starts the channel it configures; use `x` when you need the two channels to start together.

##### Returns

A basic status response indicating success or failure.

Wire command: `i\a\x`

| Arg | Wire type |
|---|---|
| mask | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.analog_out.set_waveform_run(mask: int) -> Result
```
```c
ow_status ow_io_analog_out_set_waveform_run(ow_device* dev, int32_t mask);
```
```rust
dev.io().analog_out().set_waveform_run(mask: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.analog_out.set_waveform_run(mask)   # check dev.ok
```
