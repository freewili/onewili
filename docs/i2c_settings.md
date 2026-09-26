# I2C Settings

`dev.io.i2c.settings` - wire path `i\i\s` - generated from `fwMenuI2CSettings`.

## frequency

Frequency. I2C bus clock frequency in Hz

Wire command: `i\i\s\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.i2c.settings.frequency(value: int) -> Result
```
```c
ow_status ow_io_i2c_settings_frequency(ow_device* dev, int32_t value);
```
```rust
dev.io().i2c().settings().frequency(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.i2c.settings.frequency(value)   # check dev.ok
```

## pull_ups

PullUps. Enable I2C bus pull-up resistors

Wire command: `i\i\s\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.i2c.settings.pull_ups(value: int) -> Result
```
```c
ow_status ow_io_i2c_settings_pull_ups(ow_device* dev, int32_t value);
```
```rust
dev.io().i2c().settings().pull_ups(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.i2c.settings.pull_ups(value)   # check dev.ok
```
