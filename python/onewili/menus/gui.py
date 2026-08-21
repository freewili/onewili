"""GUI Functions menu - generated from fwMenuGUI. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from .. import enums
from ..menubase import MenuBase
from ..transport import Transport
from .gui_panels import GUIPanels
from .gui_controls import GUIControls
from .gui_control_properties import GUIControlProperties
from .gui_dialogs import GUIDialogs


class GUI(MenuBase):
    r"""GUI Functions (``g``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "button": {"binary": False, "payload": [("gray", "decU32"), ("yellow", "decU32"), ("green", "decU32"), ("blue", "decU32"), ("red", "decU32")], "description": "Button state report (1 = pressed, per button)"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.panels = GUIPanels(transport, nav_path + "\\c")
        self.controls = GUIControls(transport, nav_path + "\\b")
        self.control_properties = GUIControlProperties(transport, nav_path + "\\e")
        self.dialogs = GUIDialogs(transport, nav_path + "\\f")

    def set_led_color(self, ledindex: int, red: int, green: int, blue: int, duration: int, mode: enums.owLEDManagerLEDMode | int) -> Result:
        r"""Set Board LED.

        Wire: ``g\s``

        Sets a led to a specific color

        # Set Board LED

Sets the color of one of the seven on-board RGB status LEDs. The new color is applied immediately — no separate refresh/commit call is required.

## Arguments

| Name | Type | Range | Description |
|------|------|-------|-------------|
| `ledindex` | decU8 | `0`–`6` | Which on-board RGB LED to set |
| `red`      | decU8 | `0`–`255` | Red channel intensity |
| `green`    | decU8 | `0`–`255` | Green channel intensity |
| `blue`     | decU8 | `0`–`255` | Blue channel intensity |

## Returns

- `success=basic` — `1` if the LED was updated, `0` if the arguments could not be parsed or `ledindex` is out of range.

## Examples

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

## Notes

- Indices outside `0`–`6` are rejected.
- Each channel is 8-bit (`0`–`255`); higher values are clamped/rejected by the `decU8` parser.
- To turn an LED off, set all three channels to `0`.

        Enter LED (0-15) R G B (0-255) duration (ms) mode (0-4)

        Args:
            ledindex: ledindex (decU8).
            red: red (decU8).
            green: green (decU8).
            blue: blue (decU8).
            duration: duration (decS32).
            mode: mode (owLEDManagerLEDMode).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(ledindex), encoding.enc_int(red), encoding.enc_int(green), encoding.enc_int(blue), encoding.enc_int(duration), encoding.enc_int(mode)], [])

    def show_fwi_image(self, filename: str) -> Result:
        r"""Show FWI Image.

        Wire: ``g\l``

        Shows an freewili image (fwi) file from the file system.

        # Show FWI Image

Loads and displays a FreeWili Image (`.fwi`) file from the on-device filesystem on the 3.5" touchscreen display.

## Arguments

| Name | Type | Description |
|------|------|-------------|
| `filename` | string | Path to the `.fwi` file to display |

### Path resolution

- If `filename` begins with `\`, it is treated as an **absolute path** on the filesystem (e.g. `\images\logo.fwi`).
- Otherwise, it is resolved **relative to `\images\`** — `logo.fwi` is loaded as `\images\logo.fwi`.
- Include the `.fwi` extension explicitly; it is not appended automatically.
- The file must already exist on the filesystem.

## Returns

- `success=basic` — `1` if the file was found and displayed, `0` if the path could not be resolved, the file does not exist, or the image failed to load.

## Examples

Show an image stored in the default `\images\` folder:

```
logo.fwi
```

Show an image using an absolute path:

```
\images\subdir\splash.fwi
```

## Notes

- `.fwi` is the FreeWili native image format. Use the FreeWili GUI tools to convert PNG/JPEG assets into `.fwi` files and copy them to the device's SD card or internal filesystem.
- To display an image bundled as a built-in asset (by ID) rather than a file, use **Show Asset Image** (`a`) instead.
- To clear the screen after viewing, use **Reset Display** (`t`).

        Enter Path of Image

        Args:
            filename: filename (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_str(filename)], [])

    def clear_display(self) -> Result:
        r"""Reset Display.

        Wire: ``g\t``

        Clears any GUI menu actions done to the display such as show image or show text

        # Reset Display

Clears the 3.5" touchscreen of any content drawn by menu commands such as **Show FWI Image** (`l`), **Show Asset Image** (`a`), or **Show Text Display** (`p`), returning the display to its default state.

## Arguments

None.

## Returns

- `success=basic` — `1` if the display was reset, `0` otherwise.

## Examples

Reset the display after viewing an image or text:

```
t
```

## Notes

- Use this after any of the following to restore the default UI:
  - **Show FWI Image** (`l`)
  - **Show Asset Image** (`a`)
  - **Show Text Display** (`p`)
- This command takes no arguments — any input is ignored.
- It only affects the display contents; it does not change LED state, button state, or any other peripheral.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def show_text(self, texttodisplay: str) -> Result:
        r"""Show Text Display.

        Wire: ``g\p``

        Show text on the free wili display

        # Show Text Display

Draws a text overlay on top of the current GUI window on the 3.5" touchscreen display. The underlying GUI is not modified — the text is rendered over it until cleared.

## Arguments

| Name | Type | Description |
|------|------|-------------|
| `texttodisplay` | string | The text to render as an overlay |

## Returns

- `success=basic` — `1` if the text was accepted and drawn, `0` if the argument could not be parsed.

## Examples

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

## Notes

- The text is drawn as an **overlay** above the current GUI window; the GUI underneath is preserved.
- Calling **Show Text Display** again replaces the previous overlay text.
- Text longer than the available screen area is **truncated** to fit — it is not scrolled or wrapped onto additional screens.
- To remove the overlay and restore the default display, use **Reset Display** (`t`).
- To show an image instead of text, use **Show FWI Image** (`l`) or **Show Asset Image** (`a`).

        Enter Text To Display

        Args:
            texttodisplay: texttodisplay (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_str(texttodisplay)], [])

    def read_all(self) -> Result:
        r"""Read All Buttons.

        Wire: ``g\u``

        Sets the baud rate for I2C in Hz

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [], [])

    def stream_io(self, pin: int) -> Result:
        r"""Stream Buttons.

        Wire: ``g\o``

        Sends GPIO values as a specific rate to host

        Enter Sample Time in milliseconds

        Args:
            pin: pin (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(pin)], [])

    def show_image_asset_by_id(self, image_id: int) -> Result:
        r"""Show Asset Image.

        Wire: ``g\a``

        Reads the number from the address

        Enter ID of Image

        Args:
            image_id: image_id (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(image_id)], [])

    def screenshot(self, filename: str, filetype: enums.owScreenshotFileType | int, counter: bool, timestamp: bool) -> Result:
        r"""Screenshot.

        Wire: ``g\i``

        Saves the display to SD. Args: filename, png/fwi, counter 0/1, timestamp 0/1.

        # Screenshot

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

        Filename Type(png/fwi) Counter(0/1) Timestamp(0/1)

        Args:
            filename: filename (string).
            filetype: filetype (owScreenshotFileType).
            counter: counter (bool).
            timestamp: timestamp (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_str(filename), encoding.enc_int(filetype), encoding.enc_bool(counter), encoding.enc_bool(timestamp)], [])

    def simulate_keypress(self, button: enums.owGUIButton | int, presstype: enums.owButtonPressType | int) -> Result:
        r"""Simulate Keypress.

        Wire: ``g\k``

        Injects a button action into the DISPLAY GUI.

        # Simulate Keypress

Arguments are `button presstype`. Buttons are `gray`, `yellow`, `green`, `blue`, `red`, `up`, `down`, `left`, `right`, `center`, `ok`, `cancel`, `home`, or `page`. Press types are `press` (normal tap), `longpress`, `pressandstay` (held until released), and `release`.

Examples:
- `up press`
- `right pressandstay`
- `right release`
- `red longpress`

        Button PressType(press/longpress/pressandstay/release)

        Args:
            button: button (owGUIButton).
            presstype: presstype (owButtonPressType).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_int(button), encoding.enc_int(presstype)], [])

    def reinit_lcd_panel(self, mode: int) -> Result:
        r"""Reinit LCD Panel.

        Wire: ``g\r``

        Tells DISPLAY to re-initialize its ST7796 LCD panel without rebooting. Arg: mode 0-2.

        # Reinit LCD Panel

Asks the DISPLAY CPU to re-initialize its ST7796 LCD panel in place, without rebooting either CPU. Use it when the screen goes black with the backlight still lit while the device otherwise responds — the panel has latched a DISPOFF/SLPIN-like state while the GUI kept blitting.

Modes:
- `0` — send DISPON only (recovers a latched display-off; GRAM and config untouched).
- `1` — send SLPOUT, wait 120 ms, then DISPON (recovers a latched sleep-in).
- `2` — full controller re-init, then repaint the current view and restore the backlight.

Start at mode `0` and escalate: the first mode that brings the picture back identifies which state the controller was in. The re-init runs on the DISPLAY's rendering core once any in-flight LCD DMA has finished.

        Mode (0=DISPON 1=SLPOUT+DISPON 2=full reinit)

        Args:
            mode: mode (decU8).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_int(mode)], [])
