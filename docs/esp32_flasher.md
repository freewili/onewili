# ESP32 Flasher Functions

`dev.wireless.esp32_flasher` - wire path `w\a` - generated from `fwMenuESP32Flasher`.

## enter_bootloader

Connect To Bootloader. Opens a ROM-loader session: resets the ESP32 into its bootloader and loads the flasher stub

Requires power zone 5 (ESP32). See [Errors](errors.md).

#### Connect To Bootloader

Drives the ESP32's `BOOT` and `EN` pins to put the target into ROM bootloader (download) mode and establishes a serial-loader sync over the UART. Once synced, the session stays open for the other flash/memory/register operations in this menu.

##### Session

- While a session is open the chip sits in its ROM loader: the FREE-WILi's Wi-Fi/BLE link to it is parked, and Flash From Folder reports `Busy`.
- `r` (Reset) closes the session and restarts the ESP32 application; `p 1` and a non-zero `t` entry point close it too.
- A session with no command for 60 s closes itself and restarts the application.
- Write, erase and memory commands need an open session and answer `Not connected` without one. The read-only queries (`i`, `k`, `m`, `j`, `c`) open a session for themselves when none is open and restart the application afterwards.
- Calling `b` again restarts the session, so a new baud rate takes effect.

##### Argument

- `upgrade_transmission_rate` (`decU32`, baud)
  - Baud rate to switch to **after** a successful sync.
  - Initial sync always occurs at the default `115200` baud.
  - Pass `0` to keep the link at `115200`.
  - Typical values: `230400`, `460800`, `921600`.
  - Ignored on ESP8266 targets (not supported by ROM).

##### Returns

- `success` — `true` if the bootloader handshake (and optional rate change) completed.

##### Behavior

1. Toggle `BOOT`/`EN` to enter ROM download mode.
2. Sync with the ESP loader at `115200`.
3. If `upgrade_transmission_rate != 0`, request the target to switch baud and reconfigure the host UART to match.

##### Typical Workflow

```text
b <baud>     # Connect To Bootloader
i            # Read Chip ID / security info
k            # Read flash size
f ...        # Start flash operations
o ...        # Write flash data
p 1          # Finish flash, reboot
```

##### Troubleshooting

- **Timeout** — check wiring of `EN`, `BOOT`, `TX`, `RX`, `GND`.
- **Invalid target** — chip or revision not supported by the loader build.
- **Invalid response at high baud** — retry with `0` (stay at 115200) or shorter / better-quality wires.

Wire command: `w\a\b`

| Arg | Wire type |
|---|---|
| upgrade_transmission_rate | decU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.enter_bootloader(upgrade_transmission_rate: int) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_enter_bootloader(ow_device* dev, int32_t upgrade_transmission_rate);
```
```rust
dev.wireless().esp32_flasher().enter_bootloader(upgrade_transmission_rate: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.enter_bootloader(upgrade_transmission_rate)   # check dev.ok
```

## enter_application

Reset. Closes any loader session and resets the ESP32 into its application

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\r`

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.enter_application() -> Result
```
```c
ow_status ow_wireless_esp32_flasher_enter_application(ow_device* dev);
```
```rust
dev.wireless().esp32_flasher().enter_application() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.enter_application()   # check dev.ok
```

## get_i_dand_security

Read Chip ID And Security Info. Reads the ESP32's chip ID, ECO version and security flags

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\i`

Returns: esp_chip_id (decU32), version (decU32), sb_en (bool), sbar_en (bool), sdm_en (bool), sbrk_1 (bool), sbrk_2 (bool), sbrk_3 (bool), jtag_sw_dis (bool), jtag_hw_dis (bool), usb_dis (bool), flash_enc_en (bool), dcache_dis (bool), icache_dis (bool)

```python
dev.wireless.esp32_flasher.get_i_dand_security() -> Result
```
```c
ow_status ow_wireless_esp32_flasher_get_i_dand_security(ow_device* dev, int32_t* esp_chip_id, int32_t* version, bool* sb_en, bool* sbar_en, bool* sdm_en, bool* sbrk_1, bool* sbrk_2, bool* sbrk_3, bool* jtag_sw_dis, bool* jtag_hw_dis, bool* usb_dis, bool* flash_enc_en, bool* dcache_dis, bool* icache_dis);
```
```rust
dev.wireless().esp32_flasher().get_i_dand_security() -> Result<(i32, i32, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.get_i_dand_security()   # returns value; check dev.ok
```

## read_flash_size

Read Flash Size. Detects the ESP32's flash size in bytes

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\k`

Returns: flash_size_bytes (decU32)

```python
dev.wireless.esp32_flasher.read_flash_size() -> Result
```
```c
ow_status ow_wireless_esp32_flasher_read_flash_size(ow_device* dev, int32_t* flash_size_bytes);
```
```rust
dev.wireless().esp32_flasher().read_flash_size() -> Result<i32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.read_flash_size()   # returns value; check dev.ok
```

## read_esp32mac

Read MAC. Reads the ESP32's factory MAC address

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\m`

Returns: esp32_mac (string)

```python
dev.wireless.esp32_flasher.read_esp32mac() -> Result
```
```c
ow_status ow_wireless_esp32_flasher_read_esp32mac(ow_device* dev, char* esp32_mac, size_t esp32_mac_cap);
```
```rust
dev.wireless().esp32_flasher().read_esp32mac() -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.read_esp32mac()   # returns value; check dev.ok
```

## erase_all_flash

Erase All Flash. Erases the ESP32's entire flash. Needs an open loader session

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\e`

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.erase_all_flash() -> Result
```
```c
ow_status ow_wireless_esp32_flasher_erase_all_flash(ow_device* dev);
```
```rust
dev.wireless().esp32_flasher().erase_all_flash() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.erase_all_flash()   # check dev.ok
```

## start_flash_operations

Start Writing Flash Operations. Prepares ESP32 to write flash at offset and expected size. Block size can be up to 128 bytes; each Write Flash sends one block

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\f`

| Arg | Wire type |
|---|---|
| offset | hexU32 |
| size | decU32 |
| block_size | decU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.start_flash_operations(offset: int, size: int, block_size: int) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_start_flash_operations(ow_device* dev, uint32_t offset, int32_t size, int32_t block_size);
```
```rust
dev.wireless().esp32_flasher().start_flash_operations(offset: u32, size: i32, block_size: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.start_flash_operations(offset, size, block_size)   # check dev.ok
```

## stop_flash_operation

Finish Flash Writing Operations. Ends ESP32 flashing; reboot=1 also closes the session and starts the new image

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\p`

| Arg | Wire type |
|---|---|
| reboot | bool |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.stop_flash_operation(reboot: bool) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_stop_flash_operation(ow_device* dev, bool reboot);
```
```rust
dev.wireless().esp32_flasher().stop_flash_operation(reboot: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.stop_flash_operation(reboot)   # check dev.ok
```

## flash_write

Write Flash. Writes one block (up to the block size given to f) into flash

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\o`

| Arg | Wire type |
|---|---|
| flash_data | bytearray |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.flash_write(flash_data: bytes | bytearray) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_flash_write(ow_device* dev, const uint8_t* flash_data, size_t flash_data_len);
```
```rust
dev.wireless().esp32_flasher().flash_write(flash_data: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.flash_write(flash_data)   # check dev.ok
```

## flash_read

Read Flash. Reads up to 128 bytes of ESP32 flash at the given address

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\j`

| Arg | Wire type |
|---|---|
| offset | hexU32 |
| size | decU32 |

Returns: data (hexbytes)

```python
dev.wireless.esp32_flasher.flash_read(offset: int, size: int) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_flash_read(ow_device* dev, uint32_t offset, int32_t size, uint8_t* data, size_t data_cap, size_t* data_len);
```
```rust
dev.wireless().esp32_flasher().flash_read(offset: u32, size: i32) -> Result<Vec<u8>, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.flash_read(offset, size)   # returns value; check dev.ok
```

## start_write_memory_operations

Start Memory Write Operations. Prepares a RAM load on the ESP32. Block size can be up to 128 bytes

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\y`

| Arg | Wire type |
|---|---|
| offset | hexU32 |
| size | decU32 |
| block_size | decU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.start_write_memory_operations(offset: int, size: int, block_size: int) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_start_write_memory_operations(ow_device* dev, uint32_t offset, int32_t size, int32_t block_size);
```
```rust
dev.wireless().esp32_flasher().start_write_memory_operations(offset: u32, size: i32, block_size: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.start_write_memory_operations(offset, size, block_size)   # check dev.ok
```

## memory_write

Write Memory. Writes one block (up to the block size given to y) into ESP32 RAM

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\0`

| Arg | Wire type |
|---|---|
| data | bytearray |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.memory_write(data: bytes | bytearray) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_memory_write(ow_device* dev, const uint8_t* data, size_t data_len);
```
```rust
dev.wireless().esp32_flasher().memory_write(data: &[u8]) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.memory_write(data)   # check dev.ok
```

## stop_memory_operation

Stop Memory Write Operations. Ends a RAM load; a non-zero entry point starts the loaded code and closes the session

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\t`

| Arg | Wire type |
|---|---|
| entry_address | hexU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.stop_memory_operation(entry_address: int) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_stop_memory_operation(ow_device* dev, uint32_t entry_address);
```
```rust
dev.wireless().esp32_flasher().stop_memory_operation(entry_address: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.stop_memory_operation(entry_address)   # check dev.ok
```

## register_write

Write Register. Writes a 4 byte value onto a register in the esp32

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\g`

| Arg | Wire type |
|---|---|
| offset | hexU32 |
| value | hexU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.register_write(offset: int, value: int) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_register_write(ow_device* dev, uint32_t offset, uint32_t value);
```
```rust
dev.wireless().esp32_flasher().register_write(offset: u32, value: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.register_write(offset, value)   # check dev.ok
```

## register_read

Read Register. Reads a 4 byte value from a register in the esp32

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\c`

| Arg | Wire type |
|---|---|
| offset | hexU32 |

Returns: memory_block (hexU32)

```python
dev.wireless.esp32_flasher.register_read(offset: int) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_register_read(ow_device* dev, uint32_t offset, uint32_t* memory_block);
```
```rust
dev.wireless().esp32_flasher().register_read(offset: u32) -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.register_read(offset)   # returns value; check dev.ok
```

## flash_default

Flash Default App. Not available on FW2: there is no built-in image. Use Flash From Folder

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\n`

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.flash_default() -> Result
```
```c
ow_status ow_wireless_esp32_flasher_flash_default(ow_device* dev);
```
```rust
dev.wireless().esp32_flasher().flash_default() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.flash_default()   # check dev.ok
```

## flash_from_folder

Flash From Folder. Flashes the ESP32 from an idf.py build folder on the SD card

Requires power zone 5 (ESP32). See [Errors](errors.md).

#### Flash From Folder

Starts flashing the ESP32 from a folder on the SD card. The folder must contain a `flasher_args.json` manifest as produced by an `idf.py build` (copy the whole build output folder - the manifest plus the `.bin` files it references - onto the SD card).

##### Argument

- `folder` (`string`) - SD card path of the build folder, e.g. `1:/bottlenose/`. A trailing `/` is optional.

##### Returns

- `success` - `true` if the flash was STARTED. Flashing itself runs in the background and takes tens of seconds.

The command fails immediately (with a reason in the payload) when:

- `Busy` - a flash is already running
- `No flasher_args.json` - the folder has no manifest

##### Behavior

1. The ESP32 is put into its ROM bootloader (BOOT/EN via the IO expander).
2. The loader syncs at 115200 baud, then upgrades to 460800.
3. Each partition listed in `flash_files` is written in turn.
4. The ESP32 is reset back into its application.

Progress is streamed to the console as `[*espflasher <message> <0|1>]` events and shown on the display as a progress dialog. Poll `Flash Status` (`s`) for machine-readable progress.

##### Typical Workflow

```text
w            # wireless menu
a            # ESP32 Flasher Functions
w 1:/bottlenose/   # start flashing
s            # poll: flashing progress partition_index partition_count
```

##### Troubleshooting

- **Timeout events** - ESP32 not entering the bootloader; check power and the BOOT/EN lines.
- **Manifest parse errors** - `flasher_args.json` larger than 4 KB or more than 6 partitions is rejected.

Wire command: `w\a\w`

| Arg | Wire type |
|---|---|
| folder | string |

Returns: none (Ok/Err only)

```python
dev.wireless.esp32_flasher.flash_from_folder(folder: str) -> Result
```
```c
ow_status ow_wireless_esp32_flasher_flash_from_folder(ow_device* dev, const char* folder);
```
```rust
dev.wireless().esp32_flasher().flash_from_folder(folder: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.flash_from_folder(folder)   # check dev.ok
```

## flash_status

Flash Status. Reports ESP32 flashing state and progress percentage

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\a\s`

Returns: flashing (bool), progress (decU32), partition_index (decU32), partition_count (decU32)

```python
dev.wireless.esp32_flasher.flash_status() -> Result
```
```c
ow_status ow_wireless_esp32_flasher_flash_status(ow_device* dev, bool* flashing, int32_t* progress, int32_t* partition_index, int32_t* partition_count);
```
```rust
dev.wireless().esp32_flasher().flash_status() -> Result<(bool, i32, i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.esp32_flasher.flash_status()   # returns value; check dev.ok
```
