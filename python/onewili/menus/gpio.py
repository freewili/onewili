"""GPIO Functions menu - generated from fwMenuGPIO. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .io_direction_settings import IODirectionSettings


class GPIO(MenuBase):
    r"""GPIO Functions (``i\g``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "gpioReport": {"binary": True, "header_type": 0, "payload": [("time_stamp_ns", "hexU64"), ("gpio_bitfield", "hexU32")], "description": "Periodic GPIO bitfield report (binary API)"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.io_direction_settings = IODirectionSettings(transport, nav_path + "\\a")

    def set_io_high(self, pin: int) -> Result:
        r"""High.

        Wire: ``i\g\s``

        Sets a GPIO high

        # High

Drive a GPIO pin **high** (logic 1, ~3.3 V).

## Usage

Enter the GPIO pin number when prompted:

```
s 25
```

## Arguments

- `pin` — GPIO number to drive high (unsigned decimal, e.g. `25`).

## Returns

- `success` — `true` if the pin is valid and was driven high, `false` otherwise.

## Behavior

- Reconfigures the pin to `GPIO_FUNC_SIO`.
- Sets the output latch to `1`.
- Sets pin direction to **output**.

## Valid Pins

Only pins exposed on the FreeWili connectors are accepted. On the standard
build these include: `8`, `9`, `10`, `11`, `12`, `13`, `14`, `15`, `16`, `17`,
`25`, `26`, `27`. Invalid pins return `false`.

## See Also

- `l` — Low (drive a GPIO low)
- `t` — Toggle (invert a GPIO)
- `u` — Get All IOs (read full GPIO bitfield)

        Enter GPIO Number

        Args:
            pin: pin (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(pin)], [])

    def set_io_low(self, pin: int) -> Result:
        r"""Low.

        Wire: ``i\g\l``

        Sets a GPIO low

        # Low

Drive a GPIO pin **low** (logic 0, ~0 V).

## Usage

Enter the GPIO pin number when prompted:

```
l 25
```

## Arguments

- `pin` — GPIO number to drive low (unsigned decimal, e.g. `25`).

## Returns

- `success` — `true` if the pin is valid and was driven low, `false` otherwise.

## Behavior

- Reconfigures the pin to `GPIO_FUNC_SIO`.
- Sets the output latch to `0`.
- Sets pin direction to **output**.

## Valid Pins

Only pins exposed on the FreeWili connectors are accepted. On the standard
build these include: `8`, `9`, `10`, `11`, `12`, `13`, `14`, `15`, `16`, `17`,
`25`, `26`, `27`. Invalid pins return `false`.

## See Also

- `s` — High (drive a GPIO high)
- `t` — Toggle (invert a GPIO)
- `u` — Get All IOs (read full GPIO bitfield)

        Enter GPIO Number

        Args:
            pin: pin (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_int(pin)], [])

    def set_io_toggle(self, pin: int) -> Result:
        r"""Toggle.

        Wire: ``i\g\t``

        Toggles the specified GPIO

        # Toggle

**Invert** the current output level of a GPIO pin. If the pin is currently low it
is driven high; if it is currently high it is driven low.

## Usage

Enter the GPIO pin number when prompted:

```
t 25
```

## Arguments

- `pin` — GPIO number to toggle (decimal, e.g. `25`).

## Returns

- `success` — `true` if the pin is valid and was toggled, `false` otherwise.

## Behavior

- Reads the current pin state with `gpio_get()`.
- Reconfigures the pin to `GPIO_FUNC_SIO`.
- Sets the output latch to the **opposite** of the current state.
- Sets pin direction to **output**.

## Valid Pins

Only pins exposed on the FreeWili connectors are accepted. On the standard
build these include: `8`, `9`, `10`, `11`, `12`, `13`, `14`, `15`, `16`, `17`,
`25`, `26`, `27`. Invalid pins return `false`.

## Notes

- If the pin was previously configured as an input (e.g. floating or pulled),
  the read state determines the initial toggle direction before the pin is
  switched to output.
- To force a known level instead of toggling, use `s` (High) or `l` (Low).

## See Also

- `s` — High (drive a GPIO high)
- `l` — Low (drive a GPIO low)
- `p` — PWM IO (drive a GPIO with PWM)
- `u` — Get All IOs (read full GPIO bitfield)

        Enter GPIO Number

        Args:
            pin: pin (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_int(pin)], [])

    def set_pwm(self, gpio_number: int, freq: float, duty: float) -> Result:
        r"""PWM IO.

        Wire: ``i\g\p``

        Enables the PWM feature of GPIO

        # PWM IO

Enable **PWM** (pulse-width modulation) output on a GPIO pin at a given
frequency and duty cycle.

## Usage

Enter the GPIO number, frequency (Hz), and duty cycle (%) separated by
spaces:

```
p 25 1000 50
```

The example above drives pin `25` with a 1 kHz square wave at 50 % duty.

## Arguments

- `GpioNumber` — GPIO pin to drive (unsigned decimal, e.g. `25`).
- `Freq` — PWM frequency in **Hz** (float, e.g. `1000.0`).
- `Duty` — PWM duty cycle in **percent**, `0.0`–`100.0` (float).

## Returns

- `success` — `true` if the pin is valid and the PWM slice was
  configured successfully, `false` otherwise.

## Behavior

- Validates that the pin is a FreeWili-exposed GPIO.
- Reconfigures the pin for PWM and programs the matching PWM slice via
  `fwPWMManager::generate()` with the requested frequency and duty.
- Reads the achieved frequency and duty back from the PWM hardware
  (actual values may be quantized by the slice divider/wrap settings).

## Valid Pins

Only pins exposed on the FreeWili connectors are accepted. On the
standard build these include: `8`, `9`, `10`, `11`, `12`, `13`, `14`,
`15`, `16`, `17`, `25`, `26`, `27`. Invalid pins return `false`.

## Notes

- Very low or very high frequencies may not be exactly representable;
  the achieved value depends on the RP2350 PWM clock divider and wrap.
- Duty cycles of `0` or `100` produce a steady low or high output,
  respectively.
- To stop PWM and return the pin to plain GPIO, drive it with `s`, `l`,
  or `t`.

## See Also

- `s` — High (drive a GPIO high)
- `l` — Low (drive a GPIO low)
- `t` — Toggle (invert a GPIO)
- `u` — Get All IOs (read full GPIO bitfield)

        Enter GPIO Number, freq, and duty (0-100) separated by spaces:

        Args:
            gpio_number: gpio_number (decU32).
            freq: freq (float).
            duty: duty (float).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(gpio_number), encoding.enc_float(freq), encoding.enc_float(duty)], [])

    def read_all(self) -> Result:
        r"""Get All IOs (hex).

        Wire: ``i\g\u``

        Reads all the IOs in a bitfield

        # Get All IOs (hex)

Read the **state of every GPIO pin** at once and print the result as a
hexadecimal bitfield.

## Usage

```
u
```

No arguments are required.

## Arguments

None.

## Returns

- `success` — always `true`. The GPIO bitfield is printed to the console as
  4 uppercase hex digits (e.g. `2A0F`).

## Behavior

- Calls `gpio_get_all()` to sample the **current logic level** of all RP2350
  GPIO pins in a single read.
- Prints the value with `%04X` formatting. Bit *n* of the printed value
  corresponds to GPIO *n*: `1` = high, `0` = low.

## Decoding the Output

For example, an output of `0200`:

```
0x0200 = 0000 0010 0000 0000
                ^
                bit 9 → GPIO 9 is high
```

All other pins shown are low.

## Notes

- Pins are sampled as **inputs** regardless of their current direction; this
  reads the pad level, so an output pin reads back the level it is driving.
- Only GPIOs exposed on the FreeWili connectors are typically meaningful
  (`8`–`17`, `25`–`27` on the standard build); other bits reflect internal
  signals and may not be useful.
- The printed width is fixed at 4 hex digits and does not cover all RP2350
  GPIOs — use it primarily for the lower 16 pins.

## See Also

- `s` — High (drive a GPIO high)
- `l` — Low (drive a GPIO low)
- `t` — Toggle (invert a GPIO)
- `o` — Stream IO reads (periodically report the GPIO bitfield)

        Returns:
            Result: Ok(gpiostate: int) or Err(message).
        """
        return self._call("u", [], ["hex"])

    def stream_io(self, reportratems: int) -> Result:
        r"""Stream IO reads.

        Wire: ``i\g\o``

        Sends GPIO values as a specific millisecond rate to host

        # Stream IO reads

Periodically sample **all GPIO pins** and stream the bitfield to the host as
`gpioReport` binary events at a fixed millisecond rate.

## Usage

Enter the report rate in milliseconds when prompted:

```
o 10
```

The example above streams the full GPIO bitfield every 10 ms. Pass `0` to
**stop** streaming.

## Arguments

- `reportratems` — Report interval in **milliseconds** (unsigned decimal).
  - `0` disables streaming.
  - Values `< 1` are clamped to `1` ms.

## Returns

- `success` — `true` if the rate was accepted and the stream timer was
  (re)configured, `false` if the argument could not be parsed.

## Behavior

- Configures a periodic timer at `reportratems` ms.
- On each tick, samples `gpio_get_all()` and emits a binary `gpioReport`
  event containing:
  - `ui64TimeStampNs` — host-relative timestamp in nanoseconds.
  - `uiGpioBitfield` — full RP2350 GPIO bitfield (bit *n* = GPIO *n*).
- Setting the rate to `0` halts further reports.

## Notes

- The stream uses the **binary API** (`addEvent_gpioReport`), not the text
  console, so reports do not appear as printed lines.
- Very small intervals increase USB/CPU load; choose the largest rate that
  meets your needs.
- Only one stream rate is active at a time; sending a new value replaces the
  previous one.

## See Also

- `u` — Get All IOs (one-shot read of the GPIO bitfield as hex)
- `s` — High (drive a GPIO high)
- `l` — Low (drive a GPIO low)
- `t` — Toggle (invert a GPIO)

        Enter Sample Time in milliseconds

        Args:
            reportratems: reportratems (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(reportratems)], [])

    def toggle_hsbdio(self, pin: int) -> Result:
        r"""Toggle High-Speed Bidirectional IO.

        Wire: ``i\g\e``

        Toggle utilizing GPIO27 to set the direction of GPIO26.

        # Toggle High-Speed Bidirectional IO

Enable or disable the **High-Speed Bidirectional IO (HSBDIO)** path, which uses
**GPIO27** to control the **direction** of **GPIO26** through the FPGA. This
allows GPIO26 to be driven as a fast bidirectional data line under FPGA control
instead of being a plain RP2350 GPIO.

## Usage

Enter `1` to enable HSBDIO or `0` to disable it:

```
e 1
e 0
```

## Arguments

- `enable` — HSBDIO mode flag (decimal):
  - `0` — disable HSBDIO (GPIO26 returns to normal GPIO usage).
  - `1` — enable HSBDIO (GPIO27 drives the direction of GPIO26 via the FPGA).

Non-zero values are treated as `1`.

## Returns

- `success` — `true` if the command was parsed and the FPGA configuration
  sequence completed, `false` if the argument could not be parsed or the FPGA
  configuration link could not be opened/closed.

## Behavior

1. Briefly asserts the FPGA **IO config enable** line so the RP2350 can talk to
   the FPGA's IO-direction register.
2. Calls `fwProgIODir::setHSBDIO()` with the requested enable value, which
   programs the FPGA to use **GPIO27 as the direction control** for **GPIO26**.
3. De-asserts the FPGA IO config enable line, returning normal IO control to
   the FPGA.

If either FPGA config-enable step fails, an error is printed in red and the
command aborts without printing the standard success response.

## Pins Used

- `GPIO26` — high-speed bidirectional data line.
- `GPIO27` — direction control for GPIO26 (driven by the host while HSBDIO is
  enabled).

While HSBDIO is enabled, do **not** drive GPIO26 or GPIO27 with the normal
GPIO commands (`s`, `l`, `t`, `p`) — disable HSBDIO first with `e 0`.

## See Also

- `s` — High (drive a GPIO high)
- `l` — Low (drive a GPIO low)
- `t` — Toggle (invert a GPIO)
- `u` — Get All IOs (read full GPIO bitfield)

        Enter 1 to enable or 0 to disable

        Args:
            pin: pin (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(pin)], [])

    def set_io_voltage_source(self, source: int) -> Result:
        r"""Set IO Voltage Source.

        Wire: ``i\g\v``

        Selects the voltage source connected to the external IO voltage rail.

        # Set IO Voltage Source

Selects which source the DISPLAY IO expander connects to the external IO voltage rail.

- `0` - disconnected
- `1` - 3.3 V
- `2` - 5.0 V
- `3` - Trig_IN/VREF
- `4` - programmable Vout


        Enter 0=None, 1=3.3V, 2=5.0V, 3=Trig_IN/VREF, 4=Prog Vout

        Args:
            source: source (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_int(source)], [])
