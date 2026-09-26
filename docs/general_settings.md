# General Settings

`dev.hardware.settings_home.general_settings` - wire path `h\s\e` - generated from `fwMenuGeneralSettings`.

## startup_wasm_script

Startup Wasm Script. Path to wasm or RTHON script.

Wire command: `h\s\e\a`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.general_settings.startup_wasm_script(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_general_settings_startup_wasm_script(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().general_settings().startup_wasm_script(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.general_settings.startup_wasm_script(value)   # check dev.ok
```

## startup_zoom_script

Startup Zoom Script. Path to zoom script.

Wire command: `h\s\e\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.general_settings.startup_zoom_script(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_general_settings_startup_zoom_script(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().general_settings().startup_zoom_script(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.general_settings.startup_zoom_script(value)   # check dev.ok
```

## default_fpga_script

Default FPGA Script. Path to FPGA bit file

Wire command: `h\s\e\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.general_settings.default_fpga_script(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_general_settings_default_fpga_script(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().general_settings().default_fpga_script(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.general_settings.default_fpga_script(value)   # check dev.ok
```

## wasm_debug_level

Wasm debug level. Debug messaging from WiliWasm

Wire command: `h\s\e\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.general_settings.wasm_debug_level(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_general_settings_wasm_debug_level(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().general_settings().wasm_debug_level(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.general_settings.wasm_debug_level(value)   # check dev.ok
```
