# FPGA Clock

`dev.hardware.settings_home.fpga_clock_settings` - wire path `h\s\f` - generated from `fwMenuFPGAClockSettings`.

## clk_source

Clk Source. Choose the clock source that drives the FPGA (CPU clock, oscillator, USB, or RTC)

Wire command: `h\s\f\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.fpga_clock_settings.clk_source(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_fpga_clock_settings_clk_source(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().fpga_clock_settings().clk_source(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.fpga_clock_settings.clk_source(value)   # check dev.ok
```

## clk_divider_int

Clk Divider (int). Set the integer part of the clock divider used to derive the FPGA clock frequency

Wire command: `h\s\f\i`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.fpga_clock_settings.clk_divider_int(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_fpga_clock_settings_clk_divider_int(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().fpga_clock_settings().clk_divider_int(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.fpga_clock_settings.clk_divider_int(value)   # check dev.ok
```

## clk_divider_frac

Clk Divider (Frac). Set the fractional part of the clock divider used to fine-tune the FPGA clock frequency

Wire command: `h\s\f\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.fpga_clock_settings.clk_divider_frac(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_fpga_clock_settings_clk_divider_frac(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().fpga_clock_settings().clk_divider_frac(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.fpga_clock_settings.clk_divider_frac(value)   # check dev.ok
```

## comms_mode

Comms Mode. Choose whether the CPU talks to the FPGA configuration registers over SPI or I2C

Wire command: `h\s\f\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.fpga_clock_settings.comms_mode(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_fpga_clock_settings_comms_mode(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().fpga_clock_settings().comms_mode(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.fpga_clock_settings.comms_mode(value)   # check dev.ok
```
