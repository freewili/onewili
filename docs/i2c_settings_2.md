# I2C Settings

`dev.hardware.settings_home.i2c_settings` - wire path `h\s\i` - generated from `fwMenuI2CSettings`.

## frequency

Frequency. I2C bus clock frequency in Hz

Wire command: `h\s\i\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.i2c_settings.frequency(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_i2c_settings_frequency(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().i2c_settings().frequency(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.i2c_settings.frequency(value)   # check dev.ok
```

## pull_ups

PullUps. Enable I2C bus pull-up resistors

Wire command: `h\s\i\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.i2c_settings.pull_ups(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_i2c_settings_pull_ups(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().i2c_settings().pull_ups(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.i2c_settings.pull_ups(value)   # check dev.ok
```
