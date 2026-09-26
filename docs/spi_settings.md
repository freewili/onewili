# SPI Settings

`dev.io.spi.settings` - wire path `i\e\s` - generated from `fwMenuSPISettings`.

## frequency

Frequency. SPI clock frequency in Hz

Wire command: `i\e\s\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.spi.settings.frequency(value: int) -> Result
```
```c
ow_status ow_io_spi_settings_frequency(ow_device* dev, int32_t value);
```
```rust
dev.io().spi().settings().frequency(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.spi.settings.frequency(value)   # check dev.ok
```

## chip_select_pin

Chip Select Pin. GPIO pin used as SPI chip select

Wire command: `i\e\s\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.spi.settings.chip_select_pin(value: int) -> Result
```
```c
ow_status ow_io_spi_settings_chip_select_pin(ow_device* dev, int32_t value);
```
```rust
dev.io().spi().settings().chip_select_pin(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.spi.settings.chip_select_pin(value)   # check dev.ok
```

## data_bits

Data Bits. SPI data bits per transfer

Wire command: `i\e\s\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.spi.settings.data_bits(value: int) -> Result
```
```c
ow_status ow_io_spi_settings_data_bits(ow_device* dev, int32_t value);
```
```rust
dev.io().spi().settings().data_bits(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.spi.settings.data_bits(value)   # check dev.ok
```

## c_pol

CPOL. SPI clock polarity

Wire command: `i\e\s\p`

Returns: none (Ok/Err only)

```python
dev.io.spi.settings.c_pol() -> Result
```
```c
ow_status ow_io_spi_settings_c_pol(ow_device* dev);
```
```rust
dev.io().spi().settings().c_pol() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.spi.settings.c_pol()   # check dev.ok
```

## c_pha

CPHA. SPI clock phase

Wire command: `i\e\s\a`

Returns: none (Ok/Err only)

```python
dev.io.spi.settings.c_pha() -> Result
```
```c
ow_status ow_io_spi_settings_c_pha(ow_device* dev);
```
```rust
dev.io().spi().settings().c_pha() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.spi.settings.c_pha()   # check dev.ok
```
