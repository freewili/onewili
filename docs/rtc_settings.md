# RTC Settings

`dev.hardware.settings_home.rtc_settings` - wire path `h\s\c` - generated from `fwMenuRTCSettings`.

## year

Year. Set the year on the real-time clock

Wire command: `h\s\c\y`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.rtc_settings.year(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_rtc_settings_year(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().rtc_settings().year(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.rtc_settings.year(value)   # check dev.ok
```

## month

Month. Set the month on the real-time clock

Wire command: `h\s\c\n`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.rtc_settings.month(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_rtc_settings_month(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().rtc_settings().month(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.rtc_settings.month(value)   # check dev.ok
```

## day

Day. Set the day of the month on the real-time clock

Wire command: `h\s\c\e`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.rtc_settings.day(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_rtc_settings_day(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().rtc_settings().day(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.rtc_settings.day(value)   # check dev.ok
```

## day_of_week

Day Of Week. Set the day of the week on the real-time clock

Wire command: `h\s\c\w`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.rtc_settings.day_of_week(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_rtc_settings_day_of_week(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().rtc_settings().day_of_week(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.rtc_settings.day_of_week(value)   # check dev.ok
```

## hours

Hours. Set the hour on the real-time clock (24-hour format)

Wire command: `h\s\c\o`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.rtc_settings.hours(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_rtc_settings_hours(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().rtc_settings().hours(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.rtc_settings.hours(value)   # check dev.ok
```

## minutes

Minutes. Set the minutes on the real-time clock

Wire command: `h\s\c\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.rtc_settings.minutes(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_rtc_settings_minutes(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().rtc_settings().minutes(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.rtc_settings.minutes(value)   # check dev.ok
```

## seconds

Seconds. Set the seconds on the real-time clock

Wire command: `h\s\c\s`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.rtc_settings.seconds(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_rtc_settings_seconds(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().rtc_settings().seconds(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.rtc_settings.seconds(value)   # check dev.ok
```

## trim

Trim. Add or subtract n*2 clock cycles every minute

Wire command: `h\s\c\t`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.rtc_settings.trim(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_rtc_settings_trim(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().rtc_settings().trim(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.rtc_settings.trim(value)   # check dev.ok
```
