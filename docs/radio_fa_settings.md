# RF Analyzer Settings

`dev.hardware.settings_home.radio_fa_settings` - wire path `h\s\a` - generated from `fwMenuRadioFASettings`.

## default_view

Default View. Default view for the RF Analyzer

Wire command: `h\s\a\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.radio_fa_settings.default_view(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_radio_fa_settings_default_view(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().radio_fa_settings().default_view(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.radio_fa_settings.default_view(value)   # check dev.ok
```
