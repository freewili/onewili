# Sensor Settings

`dev.hardware.settings_home.sensor_settings` - wire path `h\s\v` - generated from `fwMenuSensorSettings`.

## accel_range

Accel Range. Accelerometer full-scale range index

Wire command: `h\s\v\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sensor_settings.accel_range(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sensor_settings_accel_range(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sensor_settings().accel_range(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sensor_settings.accel_range(value)   # check dev.ok
```

## gyro_range

Gyro Range. Gyroscope full-scale range index

Wire command: `h\s\v\g`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sensor_settings.gyro_range(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sensor_settings_gyro_range(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sensor_settings().gyro_range(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sensor_settings.gyro_range(value)   # check dev.ok
```

## move_threshold

Move Threshold. The amount accel must change to signal movement

Wire command: `h\s\v\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sensor_settings.move_threshold(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sensor_settings_move_threshold(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sensor_settings().move_threshold(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sensor_settings.move_threshold(value)   # check dev.ok
```

## t_cal_scale

TCal Scale. Temperature calibration, the m of mX+b

Wire command: `h\s\v\s`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sensor_settings.t_cal_scale(value: float) -> Result
```
```c
ow_status ow_hardware_settings_home_sensor_settings_t_cal_scale(ow_device* dev, double value);
```
```rust
dev.hardware().settings_home().sensor_settings().t_cal_scale(value: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sensor_settings.t_cal_scale(value)   # check dev.ok
```

## t_cal_offset

TCal Offset. Temperature calibration, the b of mX+b

Wire command: `h\s\v\o`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sensor_settings.t_cal_offset(value: float) -> Result
```
```c
ow_status ow_hardware_settings_home_sensor_settings_t_cal_offset(ow_device* dev, double value);
```
```rust
dev.hardware().settings_home().sensor_settings().t_cal_offset(value: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sensor_settings.t_cal_offset(value)   # check dev.ok
```

## stream_defaults

Stream Defaults. Bitmask of sensor streams enabled at boot: 1 accel-legacy, 2 temp, 4 motion, 8 field, 16 env, 32 orientation

Wire command: `h\s\v\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sensor_settings.stream_defaults(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sensor_settings_stream_defaults(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sensor_settings().stream_defaults(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sensor_settings.stream_defaults(value)   # check dev.ok
```
