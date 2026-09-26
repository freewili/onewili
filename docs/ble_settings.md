# Bluetooth Settings

`dev.hardware.settings_home.ble_settings` - wire path `h\s\b` - generated from `fwMenuBLESettings`.

## enable_bt

Enable BT. Turn Bluetooth LE on or off

Wire command: `h\s\b\s`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.ble_settings.enable_bt() -> Result
```
```c
ow_status ow_hardware_settings_home_ble_settings_enable_bt(ow_device* dev);
```
```rust
dev.hardware().settings_home().ble_settings().enable_bt() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.ble_settings.enable_bt()   # check dev.ok
```

## b_t_terminal

BT <-> Terminal. Shown in Bluetooth LE status, but not currently used: the firmware always follows the Enable BT setting instead

Wire command: `h\s\b\t`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.ble_settings.b_t_terminal() -> Result
```
```c
ow_status ow_hardware_settings_home_ble_settings_b_t_terminal(ow_device* dev);
```
```rust
dev.hardware().settings_home().ble_settings().b_t_terminal() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.ble_settings.b_t_terminal()   # check dev.ok
```

## b_t_advert_name

BT Advert Name. Set the name the device advertises over Bluetooth LE

Wire command: `h\s\b\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.ble_settings.b_t_advert_name(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_ble_settings_b_t_advert_name(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().ble_settings().b_t_advert_name(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.ble_settings.b_t_advert_name(value)   # check dev.ok
```
