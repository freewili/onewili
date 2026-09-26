# Orca Communication

`dev.hardware.settings_home.orca_settings` - wire path `h\s\g` - generated from `fwMenuOrcaSettings`.

## orca_com_over_uart

Orca Com over UART. Set Communication protocol for connected Orca device over UART

Wire command: `h\s\g\u`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.orca_settings.orca_com_over_uart(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_orca_settings_orca_com_over_uart(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().orca_settings().orca_com_over_uart(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.orca_settings.orca_com_over_uart(value)   # check dev.ok
```
