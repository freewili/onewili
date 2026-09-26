# Device Settings

`dev.hardware.settings_home` - wire path `h\s` - generated from `fwMenuSettingsHome`.

## software_reset

Software Reset. Performs a software reset of the device.

Wire command: `h\s\1`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.software_reset() -> Result
```
```c
ow_status ow_hardware_settings_home_software_reset(ow_device* dev);
```
```rust
dev.hardware().settings_home().software_reset() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.software_reset()   # check dev.ok
```

## software_reset_to_bootloader

Reset To Bootloader. Resets the device into the USB bootloader.

Wire command: `h\s\2`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.software_reset_to_bootloader() -> Result
```
```c
ow_status ow_hardware_settings_home_software_reset_to_bootloader(ow_device* dev);
```
```rust
dev.hardware().settings_home().software_reset_to_bootloader() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.software_reset_to_bootloader()   # check dev.ok
```

## all_settings_to_defaults

All Settings To Defaults. Restores all settings to their default values.

Wire command: `h\s\3`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.all_settings_to_defaults() -> Result
```
```c
ow_status ow_hardware_settings_home_all_settings_to_defaults(ow_device* dev);
```
```rust
dev.hardware().settings_home().all_settings_to_defaults() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.all_settings_to_defaults()   # check dev.ok
```

## Sub-menus

- [UART Settings](uart_settings_2.md) - `dev.hardware.settings_home.uart_settings`
- [I2C Settings](i2c_settings_2.md) - `dev.hardware.settings_home.i2c_settings`
- [Sensor Settings](sensor_settings.md) - `dev.hardware.settings_home.sensor_settings`
- [SPI Settings](spi_settings_2.md) - `dev.hardware.settings_home.spi_settings`
- [IO Directions](io_direction_settings_2.md) - `dev.hardware.settings_home.io_direction_settings`
- [FPGA Clock](fpga_clock_settings.md) - `dev.hardware.settings_home.fpga_clock_settings`
- [radio1](radio_settings.md) - `dev.hardware.settings_home.radio_settings`
- [radio1](radio_settings_2.md) - `dev.hardware.settings_home.radio_settings_2`
- [RF Analyzer Settings](radio_fa_settings.md) - `dev.hardware.settings_home.radio_fa_settings`
- [RTC Settings](rtc_settings.md) - `dev.hardware.settings_home.rtc_settings`
- [Wifi Settings](wifi_settings.md) - `dev.hardware.settings_home.wifi_settings`
- [Bluetooth Settings](ble_settings.md) - `dev.hardware.settings_home.ble_settings`
- [Orca Communication](orca_settings.md) - `dev.hardware.settings_home.orca_settings`
- [Websocket Server](websocket_settings.md) - `dev.hardware.settings_home.websocket_settings`
- [Neptune Settings](neptune_settings.md) - `dev.hardware.settings_home.neptune_settings`
- [General Settings](general_settings.md) - `dev.hardware.settings_home.general_settings`
- [Analog In (TLA2024) Settings](analog_in_settings.md) - `dev.hardware.settings_home.analog_in_settings`
- [Sound Settings](sound_settings.md) - `dev.hardware.settings_home.sound_settings`
- [Power Settings](power_settings.md) - `dev.hardware.settings_home.power_settings`
- [LED Show Settings](light_show_settings.md) - `dev.hardware.settings_home.light_show_settings`
- [Interface Settings](interface_settings.md) - `dev.hardware.settings_home.interface_settings`
