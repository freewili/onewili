"""Serial LEDs menu - generated from fwMenuSerialLEDs. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from .. import enums
from ..menubase import MenuBase
from ..transport import Transport


class SerialLEDs(MenuBase):
    r"""Serial LEDs (``i\l``)."""

    def configure_strip(self, strip: int, gpio: int, length: int, led_type: enums.owSerialLEDType | int, inverted: bool) -> Result:
        r"""Configure Strip.

        Wire: ``i\l\c``

        Configure one of 8 serial LED strips: 0-based strip index, external GPIO (0=disabled; valid: 8-17,25,26,27), LED count (1-1024), LED type (rgb=3-byte WS2812, rgbw=4-byte SK6812), inverted polarity flag

        Configures a serial LED strip output.
GPIO must be one of the external header pins (8-17, 25, 26, 27); 0 disables the strip.
Length is 1-1024 LEDs; type rgb=WS2812-style 3-byte, rgbw=SK6812-style 4-byte; inverted=1 when driving through an inverting buffer.

        Enter strip (0-7), gpio (8-17,25-27; 0=off), length (1-1024), type (0=rgb,1=rgbw), inverted (0/1)

        Args:
            strip: strip (decU8).
            gpio: gpio (decU8).
            length: length (decU32).
            led_type: led_type (owSerialLEDType).
            inverted: inverted (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(strip), encoding.enc_int(gpio), encoding.enc_int(length), encoding.enc_int(led_type), encoding.enc_bool(inverted)], [])

    def show_config(self) -> Result:
        r"""Show Config.

        Wire: ``i\l\s``

        Prints the configuration of all 8 serial LED strips and PSRAM buffer availability

        Prints each strip's saved GPIO, length, LED type and polarity.
Also reports the live driver state and whether the PSRAM LED buffer heap is present.
Disabled strips show as off.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def set_leds(self, strip: int, start: int, count: int, red: int, green: int, blue: int, white: int) -> Result:
        r"""Set LEDs.

        Wire: ``i\l\v``

        Sets a run of LEDs on a strip to an RGB(W) value: strip 0-7, start index, repeat count, then red/green/blue/white 0-255 (white ignored on 3-byte strips)

        Writes count LEDs starting at start on the given strip.
Values are raw 0-255 per channel; white applies only to rgbw strips.
Switches the strip's show to manual.

        Enter strip (0-7), start index, count, then R G B W (0-255 each)

        Args:
            strip: strip (decU8).
            start: start (decU32).
            count: count (decU32).
            red: red (decU8).
            green: green (decU8).
            blue: blue (decU8).
            white: white (decU8).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_int(strip), encoding.enc_int(start), encoding.enc_int(count), encoding.enc_int(red), encoding.enc_int(green), encoding.enc_int(blue), encoding.enc_int(white)], [])

    def set_show(self, strip: int, show: enums.owLEDLightShow | int) -> Result:
        r"""Set Show.

        Wire: ``i\l\w``

        Runs a light show pattern on one strip (0-7) or all strips (-1)

        Selects the light show pattern for a strip.
Use strip -1 to apply to every configured strip.
Show values match the Light Show app list (manual..accel).

        Enter strip (0-7, -1=all) and show (0-13)

        Args:
            strip: strip (decS32).
            show: show (owLEDLightShow).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_int(strip), encoding.enc_int(show)], [])

    def enable_jambu_orca(self, num_strips: int) -> Result:
        r"""Enable Jambu Orca.

        Wire: ``i\l\j``

        Configures strips 1..N for the Jambu Orca 8-channel LED breakout (GPIOs 13,14,11,15,26,25,9,10)

        One-step setup for the Jambu Orca breakout board.
numStrips (1-8) strips are mapped to the Jambu header pins in order 13,14,11,15,26,25,9,10.
Existing strip lengths/types are kept (default 30 RGB if unset); edit with Configure Strip afterwards.

        Enter number of strips (1-8)

        Args:
            num_strips: num_strips (decU8).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_int(num_strips)], [])

    def auto_show(self) -> Result:
        r"""Auto Show.

        Wire: ``i\l\a``

        Automatically run the light show selected in the Light Show app on all serial LED strips

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])
