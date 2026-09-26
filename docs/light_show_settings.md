# LED Show Settings

`dev.hardware.settings_home.light_show_settings` - wire path `h\s\l` - generated from `fwMenuLightShowSettings`.

## default_show

Default Show. The LED show the display boots into

Wire command: `h\s\l\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.light_show_settings.default_show(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_light_show_settings_default_show(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().light_show_settings().default_show(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.light_show_settings.default_show(value)   # check dev.ok
```

## l_ed_strips_enabled

LED Strips Enabled. How many external LED strips the built-in show drives

Wire command: `h\s\l\s`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.light_show_settings.l_ed_strips_enabled(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_light_show_settings_l_ed_strips_enabled(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().light_show_settings().l_ed_strips_enabled(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.light_show_settings.l_ed_strips_enabled(value)   # check dev.ok
```

## roku_led_control

Roku LED Control. Allow a Roku remote to cycle LED show patterns

Wire command: `h\s\l\i`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.light_show_settings.roku_led_control() -> Result
```
```c
ow_status ow_hardware_settings_home_light_show_settings_roku_led_control(ow_device* dev);
```
```rust
dev.hardware().settings_home().light_show_settings().roku_led_control() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.light_show_settings.roku_led_control()   # check dev.ok
```

## brightness

Brightness. Onboard LED strip brightness divisor, 1 (brightest) to 16 (dimmest)

Wire command: `h\s\l\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.light_show_settings.brightness(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_light_show_settings_brightness(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().light_show_settings().brightness(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.light_show_settings.brightness(value)   # check dev.ok
```
