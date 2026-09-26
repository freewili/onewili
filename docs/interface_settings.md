# Interface Settings

`dev.hardware.settings_home.interface_settings` - wire path `h\s\x` - generated from `fwMenuInterfaceSettings`.

## double_click_ms

Double Click Ms. Button double-click window in milliseconds (currently inert; the display uses a compile-time window)

Wire command: `h\s\x\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.interface_settings.double_click_ms(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_interface_settings_double_click_ms(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().interface_settings().double_click_ms(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.interface_settings.double_click_ms(value)   # check dev.ok
```

## roku_gui_control

Roku Gui Control. Allow a Roku remote to drive the display GUI buttons

Wire command: `h\s\x\r`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.interface_settings.roku_gui_control() -> Result
```
```c
ow_status ow_hardware_settings_home_interface_settings_roku_gui_control(ow_device* dev);
```
```rust
dev.hardware().settings_home().interface_settings().roku_gui_control() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.interface_settings.roku_gui_control()   # check dev.ok
```
