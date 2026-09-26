# Serial LEDs

`dev.io.serial_leds` - wire path `i\l` - generated from `fwMenuSerialLEDs`.

## configure_strip

Configure Strip. Configure one of 8 serial LED strips: 0-based strip index, external GPIO (0=disabled; valid: 8-17,25,26,27), LED count (1-1024), LED type (rgb=3-byte WS2812, rgbw=4-byte SK6812), inverted polarity flag

Configures a serial LED strip output.
GPIO must be one of the external header pins (8-17, 25, 26, 27); 0 disables the strip.
Length is 1-1024 LEDs; type rgb=WS2812-style 3-byte, rgbw=SK6812-style 4-byte; inverted=1 when driving through an inverting buffer.

Wire command: `i\l\c`

| Arg | Wire type |
|---|---|
| strip | decU8 |
| gpio | decU8 |
| length | decU32 |
| led_type | [owSerialLEDType](enums.md#owserialledtype) |
| inverted | bool |

Returns: none (Ok/Err only)

```python
dev.io.serial_leds.configure_strip(strip: int, gpio: int, length: int, led_type: enums.owSerialLEDType | int, inverted: bool) -> Result
```
```c
ow_status ow_io_serial_leds_configure_strip(ow_device* dev, int32_t strip, int32_t gpio, int32_t length, ow_ow_serial_led_type led_type, bool inverted);
```
```rust
dev.io().serial_leds().configure_strip(strip: i32, gpio: i32, length: i32, led_type: owSerialLEDType, inverted: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.serial_leds.configure_strip(strip, gpio, length, led_type, inverted)   # check dev.ok
```

## show_config

Show Config. Prints the configuration of all 8 serial LED strips and PSRAM buffer availability

Prints each strip's saved GPIO, length, LED type and polarity.
Also reports the live driver state and whether the PSRAM LED buffer heap is present.
Disabled strips show as off.

Wire command: `i\l\s`

Returns: none (Ok/Err only)

```python
dev.io.serial_leds.show_config() -> Result
```
```c
ow_status ow_io_serial_leds_show_config(ow_device* dev);
```
```rust
dev.io().serial_leds().show_config() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.serial_leds.show_config()   # check dev.ok
```

## set_leds

Set LEDs. Sets a run of LEDs on a strip to an RGB(W) value: strip 0-7, start index, repeat count, then red/green/blue/white 0-255 (white ignored on 3-byte strips)

Writes count LEDs starting at start on the given strip.
Values are raw 0-255 per channel; white applies only to rgbw strips.
Switches the strip's show to manual.

Wire command: `i\l\v`

| Arg | Wire type |
|---|---|
| strip | decU8 |
| start | decU32 |
| count | decU32 |
| red | decU8 |
| green | decU8 |
| blue | decU8 |
| white | decU8 |

Returns: none (Ok/Err only)

```python
dev.io.serial_leds.set_leds(strip: int, start: int, count: int, red: int, green: int, blue: int, white: int) -> Result
```
```c
ow_status ow_io_serial_leds_set_leds(ow_device* dev, int32_t strip, int32_t start, int32_t count, int32_t red, int32_t green, int32_t blue, int32_t white);
```
```rust
dev.io().serial_leds().set_leds(strip: i32, start: i32, count: i32, red: i32, green: i32, blue: i32, white: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.serial_leds.set_leds(strip, start, count, red, green, blue, white)   # check dev.ok
```

## set_show

Set Show. Runs a light show pattern on one strip (0-7) or all strips (-1)

Selects the light show pattern for a strip.
Use strip -1 to apply to every configured strip.
Show values match the Light Show app list (manual..accel).

Wire command: `i\l\w`

| Arg | Wire type |
|---|---|
| strip | decS32 |
| show | [owLEDLightShow](enums.md#owledlightshow) |

Returns: none (Ok/Err only)

```python
dev.io.serial_leds.set_show(strip: int, show: enums.owLEDLightShow | int) -> Result
```
```c
ow_status ow_io_serial_leds_set_show(ow_device* dev, int32_t strip, ow_ow_led_light_show show);
```
```rust
dev.io().serial_leds().set_show(strip: i32, show: owLEDLightShow) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.serial_leds.set_show(strip, show)   # check dev.ok
```

## enable_jambu_orca

Enable Jambu Orca. Configures strips 1..N for the Jambu Orca 8-channel LED breakout (GPIOs 13,14,11,15,26,25,9,10)

One-step setup for the Jambu Orca breakout board.
numStrips (1-8) strips are mapped to the Jambu header pins in order 13,14,11,15,26,25,9,10.
Existing strip lengths/types are kept (default 30 RGB if unset); edit with Configure Strip afterwards.

Wire command: `i\l\j`

| Arg | Wire type |
|---|---|
| num_strips | decU8 |

Returns: none (Ok/Err only)

```python
dev.io.serial_leds.enable_jambu_orca(num_strips: int) -> Result
```
```c
ow_status ow_io_serial_leds_enable_jambu_orca(ow_device* dev, int32_t num_strips);
```
```rust
dev.io().serial_leds().enable_jambu_orca(num_strips: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.serial_leds.enable_jambu_orca(num_strips)   # check dev.ok
```

## auto_show

Auto Show. Automatically run the light show selected in the Light Show app on all serial LED strips

Wire command: `i\l\a`

Returns: none (Ok/Err only)

```python
dev.io.serial_leds.auto_show() -> Result
```
```c
ow_status ow_io_serial_leds_auto_show(ow_device* dev);
```
```rust
dev.io().serial_leds().auto_show() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.serial_leds.auto_show()   # check dev.ok
```
