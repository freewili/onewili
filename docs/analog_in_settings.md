# Analog In (TLA2024) Settings

`dev.hardware.settings_home.analog_in_settings` - wire path `h\s\j` - generated from `fwMenuAnalogInSettings`.

## ch0_input

Ch0 Input. TLA2024 channel 0 input mux

Wire command: `h\s\j\0`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.ch0_input(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_ch0_input(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().ch0_input(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.ch0_input(value)   # check dev.ok
```

## ch1_input

Ch1 Input. TLA2024 channel 1 input mux

Wire command: `h\s\j\1`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.ch1_input(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_ch1_input(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().ch1_input(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.ch1_input(value)   # check dev.ok
```

## ch2_input

Ch2 Input. TLA2024 channel 2 input mux

Wire command: `h\s\j\2`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.ch2_input(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_ch2_input(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().ch2_input(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.ch2_input(value)   # check dev.ok
```

## ch3_input

Ch3 Input. TLA2024 channel 3 input mux

Wire command: `h\s\j\3`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.ch3_input(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_ch3_input(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().ch3_input(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.ch3_input(value)   # check dev.ok
```

## ch0_range

Ch0 Range. TLA2024 channel 0 full-scale range

Wire command: `h\s\j\4`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.ch0_range(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_ch0_range(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().ch0_range(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.ch0_range(value)   # check dev.ok
```

## ch1_range

Ch1 Range. TLA2024 channel 1 full-scale range

Wire command: `h\s\j\5`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.ch1_range(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_ch1_range(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().ch1_range(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.ch1_range(value)   # check dev.ok
```

## ch2_range

Ch2 Range. TLA2024 channel 2 full-scale range

Wire command: `h\s\j\6`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.ch2_range(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_ch2_range(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().ch2_range(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.ch2_range(value)   # check dev.ok
```

## ch3_range

Ch3 Range. TLA2024 channel 3 full-scale range

Wire command: `h\s\j\7`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.ch3_range(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_ch3_range(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().ch3_range(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.ch3_range(value)   # check dev.ok
```

## data_rate

Data Rate. TLA2024 conversion data rate

Wire command: `h\s\j\8`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.analog_in_settings.data_rate(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_analog_in_settings_data_rate(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().analog_in_settings().data_rate(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.analog_in_settings.data_rate(value)   # check dev.ok
```
