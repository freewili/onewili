# Power Management

`dev.hardware.power_management` - wire path `h\p` - generated from `fwMenuPowerManagement`.

## list_zones

List Zones. Lists all 17 power zones with their name and rail, then the three control lines (18-20).

Wire command: `h\p\l`

Returns: none (Ok/Err only)

```python
dev.hardware.power_management.list_zones() -> Result
```
```c
ow_status ow_hardware_power_management_list_zones(ow_device* dev);
```
```rust
dev.hardware().power_management().list_zones() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.list_zones()   # check dev.ok
```

## get_zones

Get Zones. Shows which power zones are currently on, then the reset state of the three control lines.

Wire command: `h\p\g`

Returns: none (Ok/Err only)

```python
dev.hardware.power_management.get_zones() -> Result
```
```c
ow_status ow_hardware_power_management_get_zones(ow_device* dev);
```
```rust
dev.hardware().power_management().get_zones() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.get_zones()   # check dev.ok
```

## set_zone

Set Zone. Switches one power zone on or off. Zone 9 is the board-manager LED, not a power rail; zones 18-20 are reset lines with their own commands.

Wire command: `h\p\s`

| Arg | Wire type |
|---|---|
| zone | decS32 |
| on | decS32 |

Returns: none (Ok/Err only)

```python
dev.hardware.power_management.set_zone(zone: int, on: int) -> Result
```
```c
ow_status ow_hardware_power_management_set_zone(ow_device* dev, int32_t zone, int32_t on);
```
```rust
dev.hardware().power_management().set_zone(zone: i32, on: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.set_zone(zone, on)   # check dev.ok
```

## set_zone_mask

Set Zone Mask. Sets every user-controllable zone at once from a bit mask; bit 0 is zone 1.

Wire command: `h\p\m`

| Arg | Wire type |
|---|---|
| mask | decS32 |

Returns: none (Ok/Err only)

```python
dev.hardware.power_management.set_zone_mask(mask: int) -> Result
```
```c
ow_status ow_hardware_power_management_set_zone_mask(ow_device* dev, int32_t mask);
```
```rust
dev.hardware().power_management().set_zone_mask(mask: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.set_zone_mask(mask)   # check dev.ok
```

## get_power_state

Get Power State. Prints the most recent power telemetry sample.

Wire command: `h\p\t`

Returns: none (Ok/Err only)

```python
dev.hardware.power_management.get_power_state() -> Result
```
```c
ow_status ow_hardware_power_management_get_power_state(ow_device* dev);
```
```rust
dev.hardware().power_management().get_power_state() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.get_power_state()   # check dev.ok
```

## enable_power_stream

Stream Power. Streams battery, charger and power-zone telemetry to the host at the given rate. 0 stops the stream.

Wire command: `h\p\o`

| Arg | Wire type |
|---|---|
| stream_rate_ms | decS32 |

Returns: none (Ok/Err only)

```python
dev.hardware.power_management.enable_power_stream(stream_rate_ms: int) -> Result
```
```c
ow_status ow_hardware_power_management_enable_power_stream(ow_device* dev, int32_t stream_rate_ms);
```
```rust
dev.hardware().power_management().enable_power_stream(stream_rate_ms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.enable_power_stream(stream_rate_ms)   # check dev.ok
```

## set_wio_reset_line

Set WIO Reset Line. Holds or releases the LoRa module's reset line (zone 18, WIO_RST). 1 lets the module run, 0 holds it in reset.

Requires power zone 4 (Sub-GHz). See [Errors](errors.md).

#### Set WIO Reset Line

Zone 18 is `WIO_RST`, the PIC's `RG3` output into the LoRa module's `NRST` pin. It is a reset line and not a power rail, so it has a level rather than an on/off, and `NRST` is active low.

##### Usage

```
w 1
```

##### Arguments

- `state` - 0 = `hold_in_reset`, line driven low, the module is held in reset. 1 = `release`, line driven high, the module is free to run.

##### Notes

- Ok is deferred until the PIC's status frame reports that pin back, so it means the line really moved. A timeout prints what it read instead.
- Zone 4 must be on. This command only moves a reset line, and releasing a module whose `3V3_S4` rail is off achieves nothing.
- Releasing reset says nothing about what firmware the module holds. It is what makes the module answerable at all.

Wire command: `h\p\w`

| Arg | Wire type |
|---|---|
| state | [resetLineState](enums.md#resetlinestate) |

Returns: none (Ok/Err only)

```python
dev.hardware.power_management.set_wio_reset_line(state: enums.resetLineState | int) -> Result
```
```c
ow_status ow_hardware_power_management_set_wio_reset_line(ow_device* dev, ow_reset_line_state state);
```
```rust
dev.hardware().power_management().set_wio_reset_line(state: resetLineState) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.set_wio_reset_line(state)   # check dev.ok
```

## set_cm0_run_line

Set CM0 Run Line. Holds or releases the Linux CPU's run line (zone 19, CM0_RUNPG). 1 lets the module run, 0 holds it in reset.

Requires power zone 17 (CM0). See [Errors](errors.md).

#### Set CM0 Run Line

Zone 19 is `CM0_RUNPG`, the PIC's `RH0` output into the compute module's `RUN_PG` pin. Active high: low holds the Linux CPU in reset, high lets it run.

##### Usage

```
c 1
```

##### Arguments

- `state` - 0 = `hold_in_reset`, line driven low. 1 = `release`, line driven high, the CPU runs.

##### Notes

- Zone 17 must be on. Driving this line high into an unpowered module is the fault that Enable Linux CPU in the Linux menu avoids by setting the rail and the line in one frame, so prefer that command for ordinary use and this one when you need the line alone.
- Ok is deferred until the PIC reports the pin at the requested level.

Wire command: `h\p\c`

| Arg | Wire type |
|---|---|
| state | [resetLineState](enums.md#resetlinestate) |

Returns: none (Ok/Err only)

```python
dev.hardware.power_management.set_cm0_run_line(state: enums.resetLineState | int) -> Result
```
```c
ow_status ow_hardware_power_management_set_cm0_run_line(ow_device* dev, ow_reset_line_state state);
```
```rust
dev.hardware().power_management().set_cm0_run_line(state: resetLineState) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.set_cm0_run_line(state)   # check dev.ok
```

## get_control_lines

Get Control Lines. Reads back the pin levels of the three control lines, WIO_RST, CM0_RUNPG and MAIN_PWR_RST.

#### Get Control Lines

Reads back the three control-line pin levels: `WIO_RST` (zone 18), `CM0_RUNPG` (zone 19) and `MAIN_PWR_RST` (zone 20).

##### Usage

```
n
```

##### Returns

Three flags in that order, 1 meaning the line is high. For zones 18 and 19 high means released. `MAIN_PWR_RST` is reported as a raw level because its active polarity is not documented in this repo, and it is never written from the console.

##### Notes

- These bits are decoded from the PIC status frame's port bytes, the same source as the 17 rails, so they are a read-back rather than an echo of what was last requested.
- Get Zones prints the same three bits with labels.

Wire command: `h\p\n`

Returns: wio_released (bool), cm0_released (bool), main_rst_high (bool)

```python
dev.hardware.power_management.get_control_lines() -> Result
```
```c
ow_status ow_hardware_power_management_get_control_lines(ow_device* dev, bool* wio_released, bool* cm0_released, bool* main_rst_high);
```
```rust
dev.hardware().power_management().get_control_lines() -> Result<(bool, bool, bool), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.power_management.get_control_lines()   # returns value; check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `power` (text)

Power Telemetry

| Payload field | Wire type |
|---|---|
| soc | decS32 |
| current_ma | decS32 |
| remain_mah | decS32 |
| full_mah | decS32 |
| vbus_mv | decS32 |
| vsys_mv | decS32 |
| vbat_mv | decS32 |
| ichg_ma | decS32 |
| chg_stat | decS32 |
| vbus_stat | decS32 |
| fault | decS32 |
| zone_mask | decU32 |
| tier_main | decS32 |
| tier_display | decS32 |
| backlight | decS32 |
| idle_ms | decS32 |
| valid | bool |
