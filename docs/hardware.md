# Hardware Functions

`dev.hardware` - wire path `h` - generated from `fwMenuHardware`.

## get_time

Get Time. Read the current date and time from the board RTC (weekday 0=Sun..6=Sat)

Reads the real-time clock kept by the board-manager PIC. The reply is one line of decimal tokens: year month day weekday hour min sec, with weekday 0=Sun..6=Sat and a 24-hour clock. Fails when the display link is down or the PIC has not seeded the clock yet.

Wire command: `h\t`

Returns: year (dec), month (dec), day (dec), weekday (dec), hour (dec), min (dec), sec (dec)

```python
dev.hardware.get_time() -> Result
```
```c
ow_status ow_hardware_get_time(ow_device* dev, int32_t* year, int32_t* month, int32_t* day, int32_t* weekday, int32_t* hour, int32_t* min, int32_t* sec);
```
```rust
dev.hardware().get_time() -> Result<(i32, i32, i32, i32, i32, i32, i32), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.get_time()   # returns value; check dev.ok
```

## set_time

Set Time. Set the board RTC date and time; the weekday is computed from the date

Sets the real-time clock kept by the board-manager PIC. Arguments are year (2000-2099), month, day, hour (24-hour), minute, second; the weekday is derived from the date. See Also: t (Get Time).

Wire command: `h\c`

| Arg | Wire type |
|---|---|
| year | dec |
| month | dec |
| day | dec |
| hour | dec |
| min | dec |
| sec | dec |

Returns: none (Ok/Err only)

```python
dev.hardware.set_time(year: int, month: int, day: int, hour: int, min: int, sec: int) -> Result
```
```c
ow_status ow_hardware_set_time(ow_device* dev, int32_t year, int32_t month, int32_t day, int32_t hour, int32_t min, int32_t sec);
```
```rust
dev.hardware().set_time(year: i32, month: i32, day: i32, hour: i32, min: i32, sec: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.set_time(year, month, day, hour, min, sec)   # check dev.ok
```

## Sub-menus

- [Device Settings](settings_home.md) - `dev.hardware.settings_home`
- [System Functions](system.md) - `dev.hardware.system`
- [File System](file_system.md) - `dev.hardware.file_system`
- [Power Management](power_management.md) - `dev.hardware.power_management`
- [Display Functions](display_functions.md) - `dev.hardware.display_functions`
