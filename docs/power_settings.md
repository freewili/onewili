# Power Settings

`dev.hardware.settings_home.power_settings` - wire path `h\s\m` - generated from `fwMenuPowerSettings`.

## brightness_powered

Brightness Powered. Display backlight percent while on USB power

Wire command: `h\s\m\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.brightness_powered(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_brightness_powered(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().power_settings().brightness_powered(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.brightness_powered(value)   # check dev.ok
```

## brightness_batt_gt70

Brightness Batt Gt 70. Display backlight percent with battery above 70 percent

Wire command: `h\s\m\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.brightness_batt_gt70(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_brightness_batt_gt70(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().power_settings().brightness_batt_gt70(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.brightness_batt_gt70(value)   # check dev.ok
```

## brightness_batt_gt30

Brightness Batt Gt 30. Display backlight percent with battery between 30 and 70 percent

Wire command: `h\s\m\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.brightness_batt_gt30(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_brightness_batt_gt30(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().power_settings().brightness_batt_gt30(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.brightness_batt_gt30(value)   # check dev.ok
```

## brightness_batt_lt30

Brightness Batt Lt 30. Display backlight percent with battery below 30 percent

Wire command: `h\s\m\g`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.brightness_batt_lt30(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_brightness_batt_lt30(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().power_settings().brightness_batt_lt30(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.brightness_batt_lt30(value)   # check dev.ok
```

## battery_timeout

Battery Timeout. Seconds of inactivity before the display shuts off on battery

Wire command: `h\s\m\e`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.battery_timeout(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_battery_timeout(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().power_settings().battery_timeout(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.battery_timeout(value)   # check dev.ok
```

## powered_timeout

Powered Timeout. Seconds of inactivity before the display shuts off on USB power, 0 keeps it on

Wire command: `h\s\m\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.powered_timeout(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_powered_timeout(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().power_settings().powered_timeout(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.powered_timeout(value)   # check dev.ok
```

## wake_on_sound

Wake On Sound. Wake the display when the mic hears a sound

Wire command: `h\s\m\s`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.wake_on_sound() -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_wake_on_sound(ow_device* dev);
```
```rust
dev.hardware().settings_home().power_settings().wake_on_sound() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.wake_on_sound()   # check dev.ok
```

## wake_on_move

Wake On Move. Wake the display when the accelerometer sees movement

Wire command: `h\s\m\m`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.wake_on_move() -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_wake_on_move(ow_device* dev);
```
```rust
dev.hardware().settings_home().power_settings().wake_on_move() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.wake_on_move()   # check dev.ok
```

## auto_power_zones

Auto Power Zones. Auto-manage power zones; off keeps every live rail on, commands still raise rails on demand

Wire command: `h\s\m\o`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.power_settings.auto_power_zones() -> Result
```
```c
ow_status ow_hardware_settings_home_power_settings_auto_power_zones(ow_device* dev);
```
```rust
dev.hardware().settings_home().power_settings().auto_power_zones() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.power_settings.auto_power_zones()   # check dev.ok
```
