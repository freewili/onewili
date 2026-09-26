# GUI Functions

`dev.gui` - wire path `g` - generated from `fwMenuGUI`.

## set_led_color

Set Board LED. Sets a led to a specific color

Requires power zone 10 (LEDs). See [Errors](errors.md).

#### Set Board LED

Sets the color of one of the seven on-board RGB status LEDs. The new color is applied immediately — no separate refresh/commit call is required.

##### Arguments

| Name | Type | Range | Description |
|------|------|-------|-------------|
| `ledindex` | decU8 | `0`–`6` | Which on-board RGB LED to set |
| `red`      | decU8 | `0`–`255` | Red channel intensity |
| `green`    | decU8 | `0`–`255` | Green channel intensity |
| `blue`     | decU8 | `0`–`255` | Blue channel intensity |

##### Returns

- `success=basic` — `1` if the LED was updated, `0` if the arguments could not be parsed or `ledindex` is out of range.

##### Examples

Set LED 0 to full green:

```
0 0 255 0
```

Set LED 6 to white:

```
6 255 255 255
```

Turn LED 3 off:

```
3 0 0 0
```

##### Notes

- Indices outside `0`–`6` are rejected.
- Each channel is 8-bit (`0`–`255`); higher values are clamped/rejected by the `decU8` parser.
- To turn an LED off, set all three channels to `0`.

Wire command: `g\s`

| Arg | Wire type |
|---|---|
| ledindex | decU8 |
| red | decU8 |
| green | decU8 |
| blue | decU8 |
| duration | decS32 |
| mode | [owLEDManagerLEDMode](enums.md#owledmanagerledmode) |

Returns: none (Ok/Err only)

```python
dev.gui.set_led_color(ledindex: int, red: int, green: int, blue: int, duration: int, mode: enums.owLEDManagerLEDMode | int) -> Result
```
```c
ow_status ow_gui_set_led_color(ow_device* dev, int32_t ledindex, int32_t red, int32_t green, int32_t blue, int32_t duration, ow_ow_led_manager_led_mode mode);
```
```rust
dev.gui().set_led_color(ledindex: i32, red: i32, green: i32, blue: i32, duration: i32, mode: owLEDManagerLEDMode) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.set_led_color(ledindex, red, green, blue, duration, mode)   # check dev.ok
```

## show_fwi_image

Show FWI Image. Shows an freewili image (fwi) file from the file system.

#### Show FWI Image

Loads and displays a FreeWili Image (`.fwi`) file from the on-device filesystem on the 3.5" touchscreen display.

##### Arguments

| Name | Type | Description |
|------|------|-------------|
| `filename` | string | Path to the `.fwi` file to display |

###### Path resolution

- If `filename` begins with `\`, it is treated as an **absolute path** on the filesystem (e.g. `\images\logo.fwi`).
- Otherwise, it is resolved **relative to `\images\`** — `logo.fwi` is loaded as `\images\logo.fwi`.
- Include the `.fwi` extension explicitly; it is not appended automatically.
- The file must already exist on the filesystem.

##### Returns

- `success=basic` — `1` if the file was found and displayed, `0` if the path could not be resolved, the file does not exist, or the image failed to load.

##### Examples

Show an image stored in the default `\images\` folder:

```
logo.fwi
```

Show an image using an absolute path:

```
\images\subdir\splash.fwi
```

##### Notes

- `.fwi` is the FreeWili native image format. Use the FreeWili GUI tools to convert PNG/JPEG assets into `.fwi` files and copy them to the device's SD card or internal filesystem.
- To display an image bundled as a built-in asset (by ID) rather than a file, use **Show Asset Image** (`a`) instead.
- To clear the screen after viewing, use **Reset Display** (`t`).

Wire command: `g\l`

| Arg | Wire type |
|---|---|
| filename | string |

Returns: none (Ok/Err only)

```python
dev.gui.show_fwi_image(filename: str) -> Result
```
```c
ow_status ow_gui_show_fwi_image(ow_device* dev, const char* filename);
```
```rust
dev.gui().show_fwi_image(filename: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.show_fwi_image(filename)   # check dev.ok
```

## clear_display

Reset Display. Clears any GUI menu actions done to the display such as show image or show text

#### Reset Display

Clears the 3.5" touchscreen of any content drawn by menu commands such as **Show FWI Image** (`l`), **Show Asset Image** (`a`), or **Show Text Display** (`p`), returning the display to its default state.

##### Arguments

None.

##### Returns

- `success=basic` — `1` if the display was reset, `0` otherwise.

##### Examples

Reset the display after viewing an image or text:

```
t
```

##### Notes

- Use this after any of the following to restore the default UI:
  - **Show FWI Image** (`l`)
  - **Show Asset Image** (`a`)
  - **Show Text Display** (`p`)
- This command takes no arguments — any input is ignored.
- It only affects the display contents; it does not change LED state, button state, or any other peripheral.

Wire command: `g\t`

Returns: none (Ok/Err only)

```python
dev.gui.clear_display() -> Result
```
```c
ow_status ow_gui_clear_display(ow_device* dev);
```
```rust
dev.gui().clear_display() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.clear_display()   # check dev.ok
```

## show_text

Show Text Display. Show text on the free wili display

#### Show Text Display

Draws a text overlay on top of the current GUI window on the 3.5" touchscreen display. The underlying GUI is not modified — the text is rendered over it until cleared.

##### Arguments

| Name | Type | Description |
|------|------|-------------|
| `texttodisplay` | string | The text to render as an overlay |

##### Returns

- `success=basic` — `1` if the text was accepted and drawn, `0` if the argument could not be parsed.

##### Examples

Show a simple greeting:

```
Hello Wili
```

Show a status message:

```
Ready
```

Show a multi-word string (spaces are part of the text):

```
Battery OK
```

##### Notes

- The text is drawn as an **overlay** above the current GUI window; the GUI underneath is preserved.
- Calling **Show Text Display** again replaces the previous overlay text.
- Text longer than the available screen area is **truncated** to fit — it is not scrolled or wrapped onto additional screens.
- To remove the overlay and restore the default display, use **Reset Display** (`t`).
- To show an image instead of text, use **Show FWI Image** (`l`) or **Show Asset Image** (`a`).

Wire command: `g\p`

| Arg | Wire type |
|---|---|
| texttodisplay | string |

Returns: none (Ok/Err only)

```python
dev.gui.show_text(texttodisplay: str) -> Result
```
```c
ow_status ow_gui_show_text(ow_device* dev, const char* texttodisplay);
```
```rust
dev.gui().show_text(texttodisplay: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.show_text(texttodisplay)   # check dev.ok
```

## read_all

Read All Buttons. Sets the baud rate for I2C in Hz

Wire command: `g\u`

Returns: none (Ok/Err only)

```python
dev.gui.read_all() -> Result
```
```c
ow_status ow_gui_read_all(ow_device* dev);
```
```rust
dev.gui().read_all() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.read_all()   # check dev.ok
```

## stream_io

Stream Buttons. Sends GPIO values as a specific rate to host

Wire command: `g\o`

| Arg | Wire type |
|---|---|
| pin | dec |

Returns: none (Ok/Err only)

```python
dev.gui.stream_io(pin: int) -> Result
```
```c
ow_status ow_gui_stream_io(ow_device* dev, int32_t pin);
```
```rust
dev.gui().stream_io(pin: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.stream_io(pin)   # check dev.ok
```

## show_image_asset_by_id

Show Asset Image. Reads the number from the address

Wire command: `g\a`

| Arg | Wire type |
|---|---|
| image_id | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.show_image_asset_by_id(image_id: int) -> Result
```
```c
ow_status ow_gui_show_image_asset_by_id(ow_device* dev, int32_t image_id);
```
```rust
dev.gui().show_image_asset_by_id(image_id: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.show_image_asset_by_id(image_id)   # check dev.ok
```

## screenshot

Screenshot. Saves the display to SD. Args: filename, png/fwi, counter 0/1, timestamp 0/1.

#### Screenshot

Argument order: `filename filetype counter timestamp`.

- `filename`: base filename only; DISPLAY adds the extension.
- `filetype`: `png` for a standard image or `fwi` for native RGB565 data.
- `counter`: `1` adds the DISPLAY screenshot counter; `0` does not.
- `timestamp`: `1` adds `YYYYMMDD-HHMMSS`; `0` does not.

Files are written under `/screenshots` on the SD card. When timestamp and counter are both enabled, the result is `filename-YYYYMMDD-HHMMSS-counter.ext`. The counter starts at 1, increments only when enabled, and resets when DISPLAY reboots. Directory components are rejected.

Examples:
- `screen png 0 0` -> `screen.png`
- `screen png 0 1` -> `screen-YYYYMMDD-HHMMSS.png`
- `screen fwi 1 0` -> `screen-1.fwi`
- `screen png 1 1` -> `screen-YYYYMMDD-HHMMSS-2.png`

Wire command: `g\i`

| Arg | Wire type |
|---|---|
| filename | string |
| filetype | [owScreenshotFileType](enums.md#owscreenshotfiletype) |
| counter | bool |
| timestamp | bool |

Returns: none (Ok/Err only)

```python
dev.gui.screenshot(filename: str, filetype: enums.owScreenshotFileType | int, counter: bool, timestamp: bool) -> Result
```
```c
ow_status ow_gui_screenshot(ow_device* dev, const char* filename, ow_ow_screenshot_file_type filetype, bool counter, bool timestamp);
```
```rust
dev.gui().screenshot(filename: &str, filetype: owScreenshotFileType, counter: bool, timestamp: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.screenshot(filename, filetype, counter, timestamp)   # check dev.ok
```

## simulate_keypress

Simulate Keypress. Injects a button action into the DISPLAY GUI.

#### Simulate Keypress

Arguments are `button presstype`. Buttons are `gray`, `yellow`, `green`, `blue`, `red`, `up`, `down`, `left`, `right`, `center`, `ok`, `cancel`, `home`, or `page`. Press types are `press` (normal tap), `longpress`, `pressandstay` (held until released), and `release`.

Examples:
- `up press`
- `right pressandstay`
- `right release`
- `red longpress`

Wire command: `g\k`

| Arg | Wire type |
|---|---|
| button | [owGUIButton](enums.md#owguibutton) |
| presstype | [owButtonPressType](enums.md#owbuttonpresstype) |

Returns: none (Ok/Err only)

```python
dev.gui.simulate_keypress(button: enums.owGUIButton | int, presstype: enums.owButtonPressType | int) -> Result
```
```c
ow_status ow_gui_simulate_keypress(ow_device* dev, ow_ow_gui_button button, ow_ow_button_press_type presstype);
```
```rust
dev.gui().simulate_keypress(button: owGUIButton, presstype: owButtonPressType) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.simulate_keypress(button, presstype)   # check dev.ok
```

## reinit_lcd_panel

Reinit LCD Panel. Tells DISPLAY to re-initialize its ST7796 LCD panel without rebooting. Arg: mode 0-2.

#### Reinit LCD Panel

Asks the DISPLAY CPU to re-initialize its ST7796 LCD panel in place, without rebooting either CPU. Use it when the screen goes black with the backlight still lit while the device otherwise responds — the panel has latched a DISPOFF/SLPIN-like state while the GUI kept blitting.

Modes:
- `0` — send DISPON only (recovers a latched display-off; GRAM and config untouched).
- `1` — send SLPOUT, wait 120 ms, then DISPON (recovers a latched sleep-in).
- `2` — full controller re-init, then repaint the current view and restore the backlight.

Start at mode `0` and escalate: the first mode that brings the picture back identifies which state the controller was in. The re-init runs on the DISPLAY's rendering core once any in-flight LCD DMA has finished.

Wire command: `g\r`

| Arg | Wire type |
|---|---|
| mode | decU8 |

Returns: none (Ok/Err only)

```python
dev.gui.reinit_lcd_panel(mode: int) -> Result
```
```c
ow_status ow_gui_reinit_lcd_panel(ow_device* dev, int32_t mode);
```
```rust
dev.gui().reinit_lcd_panel(mode: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.reinit_lcd_panel(mode)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `button` (text)

Button state report (1 = pressed, per button)

| Payload field | Wire type |
|---|---|
| gray | decU32 |
| yellow | decU32 |
| green | decU32 |
| blue | decU32 |
| red | decU32 |

## Sub-menus

- [GUI Panels](gui_panels.md) - `dev.gui.panels`
- [GUI Controls](gui_controls.md) - `dev.gui.controls`
- [GUI Control Properties](gui_control_properties.md) - `dev.gui.control_properties`
- [Dialogs](gui_dialogs.md) - `dev.gui.dialogs`
