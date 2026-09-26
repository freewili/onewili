# File System

`dev.hardware.file_system` - wire path `h\x` - generated from `fwMenuFileSystem`.

## change_directory

Change Directory. Changes current directory

Wire command: `h\x\a`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.change_directory(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_change_directory(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().change_directory(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.change_directory(path)   # check dev.ok
```

## create_directory

Create Directory. Creates a new directory

Wire command: `h\x\c`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.create_directory(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_create_directory(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().create_directory(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.create_directory(path)   # check dev.ok
```

## remove_file_or_directory

Remove File or Directory. Removes a file or directory

Wire command: `h\x\r`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.remove_file_or_directory(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_remove_file_or_directory(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().remove_file_or_directory(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.remove_file_or_directory(path)   # check dev.ok
```

## get_file_from_pc

Get File From PC. Downloads file to Free Wili

Wire command: `h\x\f`

| Arg | Wire type |
|---|---|
| path | string |
| size | decS32 |
| crc32 | decU32 |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.get_file_from_pc(path: str, size: int, crc32: int) -> Result
```
```c
ow_status ow_hardware_file_system_get_file_from_pc(ow_device* dev, const char* path, int32_t size, int32_t crc32);
```
```rust
dev.hardware().file_system().get_file_from_pc(path: &str, size: i32, crc32: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.get_file_from_pc(path, size, crc32)   # check dev.ok
```

## send_file_to_pc

Send File To PC. Sends file to PC

Wire command: `h\x\u`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.send_file_to_pc(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_send_file_to_pc(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().send_file_to_pc(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.send_file_to_pc(path)   # check dev.ok
```

## print_file

Print File. Prints the File Content

Wire command: `h\x\p`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.print_file(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_print_file(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().print_file(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.print_file(path)   # check dev.ok
```

## create_blank_file

Create Blank File. Creates a blank file

Wire command: `h\x\b`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.create_blank_file(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_create_blank_file(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().create_blank_file(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.create_blank_file(path)   # check dev.ok
```

## edit_file

Edit File. Edits a text file

Wire command: `h\x\e`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.edit_file(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_edit_file(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().edit_file(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.edit_file(path)   # check dev.ok
```

## rename_or_move_file_directory

Rename or Move File Or Directory. Renames or Moves a File or Directory

Wire command: `h\x\n`

| Arg | Wire type |
|---|---|
| path | string |
| new_path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.rename_or_move_file_directory(path: str, new_path: str) -> Result
```
```c
ow_status ow_hardware_file_system_rename_or_move_file_directory(ow_device* dev, const char* path, const char* new_path);
```
```rust
dev.hardware().file_system().rename_or_move_file_directory(path: &str, new_path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.rename_or_move_file_directory(path, new_path)   # check dev.ok
```

## list_directory

List Directory. lists the contents of a directory. Blank for current directory.

Wire command: `h\x\l`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.list_directory(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_list_directory(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().list_directory(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.list_directory(path)   # check dev.ok
```

## format_file_system

Format File System. reformats the internal flash

Wire command: `h\x\t`

| Arg | Wire type |
|---|---|
| confirm | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.format_file_system(confirm: str) -> Result
```
```c
ow_status ow_hardware_file_system_format_file_system(ow_device* dev, const char* confirm);
```
```rust
dev.hardware().file_system().format_file_system(confirm: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.format_file_system(confirm)   # check dev.ok
```

## toggle_sd_card_host_select

Toggle SDCard Host. Toggles which host controls the SD card.

Wire command: `h\x\s`

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.toggle_sd_card_host_select() -> Result
```
```c
ow_status ow_hardware_file_system_toggle_sd_card_host_select(ow_device* dev);
```
```rust
dev.hardware().file_system().toggle_sd_card_host_select() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.toggle_sd_card_host_select()   # check dev.ok
```

## load_wili_project

Load Wili Project. Loads a fwcom .wili project (panels, blocks, app signals) and shows the Panels app.

Wire command: `h\x\w`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.load_wili_project(path: str) -> Result
```
```c
ow_status ow_hardware_file_system_load_wili_project(ow_device* dev, const char* path);
```
```rust
dev.hardware().file_system().load_wili_project(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.load_wili_project(path)   # check dev.ok
```

## set_sd_card_host

SDCard Host Select. Connects the SD card to the main CPU (0) or the USB reader / PC (1).

Wire command: `h\x\k`

| Arg | Wire type |
|---|---|
| host | decS32 |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.set_sd_card_host(host: int) -> Result
```
```c
ow_status ow_hardware_file_system_set_sd_card_host(ow_device* dev, int32_t host);
```
```rust
dev.hardware().file_system().set_sd_card_host(host: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.set_sd_card_host(host)   # check dev.ok
```

## begin_file_read

Begin File Read. Open an SD file for bounded framed reads. Paths are UTF-8 encoded as compact hex and must be absolute; the session expires after 30 seconds of inactivity.

Wire command: `h\x\0`

| Arg | Wire type |
|---|---|
| session | hex32 |
| path_hex | string |

Returns: size (decU32)

```python
dev.hardware.file_system.begin_file_read(session: int, path_hex: str) -> Result
```
```c
ow_status ow_hardware_file_system_begin_file_read(ow_device* dev, uint32_t session, const char* path_hex, int32_t* size);
```
```rust
dev.hardware().file_system().begin_file_read(session: u32, path_hex: &str) -> Result<i32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.begin_file_read(session, path_hex)   # returns value; check dev.ok
```

## begin_file_write

Begin File Write. Stage an upload beside its destination. Existing files are preserved until size and CRC32 validation succeeds. No raw USB mode is entered.

Wire command: `h\x\1`

| Arg | Wire type |
|---|---|
| session | hex32 |
| path_hex | string |
| size | decU32 |
| crc32 | hex32 |
| overwrite | bool |

Returns: size (decU32)

```python
dev.hardware.file_system.begin_file_write(session: int, path_hex: str, size: int, crc32: int, overwrite: bool) -> Result
```
```c
ow_status ow_hardware_file_system_begin_file_write(ow_device* dev, uint32_t session, const char* path_hex, int32_t size, uint32_t crc32, bool overwrite, int32_t* size_out);
```
```rust
dev.hardware().file_system().begin_file_write(session: u32, path_hex: &str, size: i32, crc32: u32, overwrite: bool) -> Result<i32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.begin_file_write(session, path_hex, size, crc32, overwrite)   # returns value; check dev.ok
```

## read_file_chunk

Read File Chunk. Read the next 1 to 192 bytes as compact hex. Use the exact sequential offset; a dash means an empty file or EOF.

Wire command: `h\x\2`

| Arg | Wire type |
|---|---|
| session | hex32 |
| offset | decU32 |
| maximum | decU32 |

Returns: count (decU32), data (string)

```python
dev.hardware.file_system.read_file_chunk(session: int, offset: int, maximum: int) -> Result
```
```c
ow_status ow_hardware_file_system_read_file_chunk(ow_device* dev, uint32_t session, int32_t offset, int32_t maximum, int32_t* count, char* data, size_t data_cap);
```
```rust
dev.hardware().file_system().read_file_chunk(session: u32, offset: i32, maximum: i32) -> Result<(i32, String), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.read_file_chunk(session, offset, maximum)   # returns value; check dev.ok
```

## write_file_chunk

Write File Chunk. Write the next 1 to 192 hex-encoded bytes. Duplicate or out-of-order chunks are rejected; never replay an ambiguous timeout.

Wire command: `h\x\3`

| Arg | Wire type |
|---|---|
| session | hex32 |
| offset | decU32 |
| data | string |

Returns: position (decU32)

```python
dev.hardware.file_system.write_file_chunk(session: int, offset: int, data: str) -> Result
```
```c
ow_status ow_hardware_file_system_write_file_chunk(ow_device* dev, uint32_t session, int32_t offset, const char* data, int32_t* position);
```
```rust
dev.hardware().file_system().write_file_chunk(session: u32, offset: i32, data: &str) -> Result<i32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.write_file_chunk(session, offset, data)   # returns value; check dev.ok
```

## finish_file_transfer

Finish File Transfer. Verify the byte count, close the file and return CRC32. A complete verified upload is published; an incomplete or corrupt upload never replaces the destination.

Wire command: `h\x\4`

| Arg | Wire type |
|---|---|
| session | hex32 |

Returns: size (decU32), crc32 (hex32)

```python
dev.hardware.file_system.finish_file_transfer(session: int) -> Result
```
```c
ow_status ow_hardware_file_system_finish_file_transfer(ow_device* dev, uint32_t session, int32_t* size, uint32_t* crc32);
```
```rust
dev.hardware().file_system().finish_file_transfer(session: u32) -> Result<(i32, u32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.finish_file_transfer(session)   # returns value; check dev.ok
```

## cancel_file_transfer

Cancel File Transfer. Close the matching transfer and remove its incomplete staging file. Other shell and menu sessions remain available.

Wire command: `h\x\5`

| Arg | Wire type |
|---|---|
| session | hex32 |

Returns: none (Ok/Err only)

```python
dev.hardware.file_system.cancel_file_transfer(session: int) -> Result
```
```c
ow_status ow_hardware_file_system_cancel_file_transfer(ow_device* dev, uint32_t session);
```
```rust
dev.hardware().file_system().cancel_file_transfer(session: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.file_system.cancel_file_transfer(session)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `fdir` (text)

directory listing entry; kind is dir/fil, or end with size as the entry count

| Payload field | Wire type |
|---|---|
| kind | string |
| name | string |
| size | decU32 |

### `filedl` (text)

File download progress ('complete N bytes')

| Payload field | Wire type |
|---|---|
| data | string |

### `fpgadl` (text)

FPGA bitstream download progress ('complete N bytes')

| Payload field | Wire type |
|---|---|
| data | string |
