# Bluetooth Settings

`dev.wireless.bluetooth_le.ble_settings` - wire path `w\b\b` - generated from `fwMenuBLESettings`.

## enable_bt

Enable BT. Turn Bluetooth LE on or off

Wire command: `w\b\b\s`

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.ble_settings.enable_bt() -> Result
```
```c
ow_status ow_wireless_bluetooth_le_ble_settings_enable_bt(ow_device* dev);
```
```rust
dev.wireless().bluetooth_le().ble_settings().enable_bt() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.ble_settings.enable_bt()   # check dev.ok
```

## b_t_terminal

BT <-> Terminal. Shown in Bluetooth LE status, but not currently used: the firmware always follows the Enable BT setting instead

Wire command: `w\b\b\t`

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.ble_settings.b_t_terminal() -> Result
```
```c
ow_status ow_wireless_bluetooth_le_ble_settings_b_t_terminal(ow_device* dev);
```
```rust
dev.wireless().bluetooth_le().ble_settings().b_t_terminal() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.ble_settings.b_t_terminal()   # check dev.ok
```

## b_t_advert_name

BT Advert Name. Set the name the device advertises over Bluetooth LE

Wire command: `w\b\b\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.ble_settings.b_t_advert_name(value: str) -> Result
```
```c
ow_status ow_wireless_bluetooth_le_ble_settings_b_t_advert_name(ow_device* dev, const char* value);
```
```rust
dev.wireless().bluetooth_le().ble_settings().b_t_advert_name(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.ble_settings.b_t_advert_name(value)   # check dev.ok
```
