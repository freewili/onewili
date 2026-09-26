# SPI Settings

`dev.hardware.settings_home.spi_settings` - wire path `h\s\s` - generated from `fwMenuSPISettings`.

## frequency

Frequency. SPI clock frequency in Hz

Wire command: `h\s\s\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.spi_settings.frequency(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_spi_settings_frequency(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().spi_settings().frequency(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.spi_settings.frequency(value)   # check dev.ok
```

## chip_select_pin

Chip Select Pin. GPIO pin used as SPI chip select

Wire command: `h\s\s\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.spi_settings.chip_select_pin(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_spi_settings_chip_select_pin(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().spi_settings().chip_select_pin(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.spi_settings.chip_select_pin(value)   # check dev.ok
```

## data_bits

Data Bits. SPI data bits per transfer

Wire command: `h\s\s\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.spi_settings.data_bits(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_spi_settings_data_bits(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().spi_settings().data_bits(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.spi_settings.data_bits(value)   # check dev.ok
```

## c_pol

CPOL. SPI clock polarity

Wire command: `h\s\s\p`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.spi_settings.c_pol() -> Result
```
```c
ow_status ow_hardware_settings_home_spi_settings_c_pol(ow_device* dev);
```
```rust
dev.hardware().settings_home().spi_settings().c_pol() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.spi_settings.c_pol()   # check dev.ok
```

## c_pha

CPHA. SPI clock phase

Wire command: `h\s\s\a`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.spi_settings.c_pha() -> Result
```
```c
ow_status ow_hardware_settings_home_spi_settings_c_pha(ow_device* dev);
```
```rust
dev.hardware().settings_home().spi_settings().c_pha() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.spi_settings.c_pha()   # check dev.ok
```
