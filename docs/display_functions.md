# Display Functions

`dev.hardware.display_functions` - wire path `h\v` - generated from `fwMenuDisplayFunctions`.

## list_display_apps

List Display Apps. Lists the firmware images available in the SD card /apps/ directory.

Wire command: `h\v\l`

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.list_display_apps() -> Result
```
```c
ow_status ow_hardware_display_functions_list_display_apps(ow_device* dev);
```
```rust
dev.hardware().display_functions().list_display_apps() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.list_display_apps()   # check dev.ok
```

## restore_display_firmware

Restore Display Firmware. Reflashes /firmware/FW2Display.uf2 to restore the standard display GUI.

Wire command: `h\v\r`

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.restore_display_firmware() -> Result
```
```c
ow_status ow_hardware_display_functions_restore_display_firmware(ow_device* dev);
```
```rust
dev.hardware().display_functions().restore_display_firmware() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.restore_display_firmware()   # check dev.ok
```

## display_bl_version

Display Bootloader Version. Enters the display bootloader, reads its version, and releases the link without transferring anything.

Wire command: `h\v\v`

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.display_bl_version() -> Result
```
```c
ow_status ow_hardware_display_functions_display_bl_version(ow_device* dev);
```
```rust
dev.hardware().display_functions().display_bl_version() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.display_bl_version()   # check dev.ok
```

## reset_display_cpu

Reset Display CPU. Pulses the display processor reset so it cold-boots its flash image.

Wire command: `h\v\x`

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.reset_display_cpu() -> Result
```
```c
ow_status ow_hardware_display_functions_reset_display_cpu(ow_device* dev);
```
```rust
dev.hardware().display_functions().reset_display_cpu() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.reset_display_cpu()   # check dev.ok
```

## power_cycle_display_cpu

Power Cycle Display. Cuts the display processor's power rail and restores it, giving a true power-on reset. Heavier than Reset Display CPU, which only pulses RUN. Bootloader entry uses RUN/BOOT on its own; use this when a warm reset is not enough.

Wire command: `h\v\c`

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.power_cycle_display_cpu() -> Result
```
```c
ow_status ow_hardware_display_functions_power_cycle_display_cpu(ow_device* dev);
```
```rust
dev.hardware().display_functions().power_cycle_display_cpu() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.power_cycle_display_cpu()   # check dev.ok
```

## set_ram_app_arg

Set RAM App Argument. Arms up to 128 bytes for the NEXT Run RAM App, placed at a fixed address near the top of the display's RAM window. Blank clears it. An armed argument makes the launch noticeably slower: the fused bootloader cannot seek, so the loader must pad the wire up to that address.

Wire command: `h\v\g`

| Arg | Wire type |
|---|---|
| text | str |

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.set_ram_app_arg(text: str) -> Result
```
```c
ow_status ow_hardware_display_functions_set_ram_app_arg(ow_device* dev, const char* text);
```
```rust
dev.hardware().display_functions().set_ram_app_arg(text: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.set_ram_app_arg(text)   # check dev.ok
```

## run_app_on_display

Run App On Display. Asks the display processor to load and run /apps/<filename> itself: it reads the UF2 over the SD link, shows a progress bar on its own screen, and jumps to the image. Works for UF2s targeting the PSRAM window (0x11000000, up to ~4 MB) or the RAM window (0x20000000, up to 448 KB) -- the display copies the image to its run address at the moment of launch. Flash is untouched; Reset Display CPU restores the stock firmware. Progress and errors appear on the display, not here.

Wire command: `h\v\a`

| Arg | Wire type |
|---|---|
| filename | str |

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.run_app_on_display(filename: str) -> Result
```
```c
ow_status ow_hardware_display_functions_run_app_on_display(ow_device* dev, const char* filename);
```
```rust
dev.hardware().display_functions().run_app_on_display(filename: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.run_app_on_display(filename)   # check dev.ok
```

## run_psram_app

Run PSRAM App. Runs /apps/<filename> on the display processor from PSRAM (0x11000000 window, up to 8 MB). Two-hop launch: a small SRAM stub is staged through the fused bootloader, then the stub receives the image into PSRAM and jumps to it. Flash is untouched; Reset Display CPU restores the stock firmware. The image must be a UF2 whose blocks target the PSRAM window.

Wire command: `h\v\p`

| Arg | Wire type |
|---|---|
| filename | str |

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.run_psram_app(filename: str) -> Result
```
```c
ow_status ow_hardware_display_functions_run_psram_app(ow_device* dev, const char* filename);
```
```rust
dev.hardware().display_functions().run_psram_app(filename: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.run_psram_app(filename)   # check dev.ok
```

## load_psram_data

Load PSRAM Data. Stages /apps/<filename> verbatim into the display's PSRAM at <offset> bytes from 0x11000000, and leaves the loader stub running instead of launching anything. For bulk assets that would otherwise have to travel inside the app's own UF2. The file is taken as raw bytes: no UF2 decode. Repeat for as many blobs as needed, then Run PSRAM App -- the stub stays resident between calls, so only the first pays the two-hop entry, and the launch overwrites only what the app image itself covers. Staged data does NOT survive a display reset.

Wire command: `h\v\s`

| Arg | Wire type |
|---|---|
| filename | str |
| offset | hexU32 |

Returns: none (Ok/Err only)

```python
dev.hardware.display_functions.load_psram_data(filename: str, offset: int) -> Result
```
```c
ow_status ow_hardware_display_functions_load_psram_data(ow_device* dev, const char* filename, uint32_t offset);
```
```rust
dev.hardware().display_functions().load_psram_data(filename: &str, offset: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.display_functions.load_psram_data(filename, offset)   # check dev.ok
```
