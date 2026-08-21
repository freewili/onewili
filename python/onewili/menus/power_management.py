"""Power Management menu - generated from fwMenuPowerManagement. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from .. import enums
from ..menubase import MenuBase
from ..transport import Transport


class PowerManagement(MenuBase):
    r"""Power Management (``h\p``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "power": {"binary": False, "payload": [("soc", "decS32"), ("current_ma", "decS32"), ("remain_mah", "decS32"), ("full_mah", "decS32"), ("vbus_mv", "decS32"), ("vsys_mv", "decS32"), ("vbat_mv", "decS32"), ("ichg_ma", "decS32"), ("chg_stat", "decS32"), ("vbus_stat", "decS32"), ("fault", "decS32"), ("zone_mask", "decU32"), ("tier_main", "decS32"), ("tier_display", "decS32"), ("backlight", "decS32"), ("idle_ms", "decS32"), ("valid", "bool")], "description": "Power Telemetry"},
    }

    def list_zones(self) -> Result:
        r"""List Zones.

        Wire: ``h\p\l``

        Lists all 17 power zones with their name and rail, then the three control lines (18-20).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [], [])

    def get_zones(self) -> Result:
        r"""Get Zones.

        Wire: ``h\p\g``

        Shows which power zones are currently on, then the reset state of the three control lines.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [], [])

    def set_zone(self, zone: int, on: int) -> Result:
        r"""Set Zone.

        Wire: ``h\p\s``

        Switches one power zone on or off. Zone 9 is the board-manager LED, not a power rail; zones 18-20 are reset lines with their own commands.

        Enter zone number and 1 or 0

        Args:
            zone: zone (decS32).
            on: on (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(zone), encoding.enc_int(on)], [])

    def set_zone_mask(self, mask: int) -> Result:
        r"""Set Zone Mask.

        Wire: ``h\p\m``

        Sets every user-controllable zone at once from a bit mask; bit 0 is zone 1.

        Enter the awake zone mask

        Args:
            mask: mask (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(mask)], [])

    def get_power_state(self) -> Result:
        r"""Get Power State.

        Wire: ``h\p\t``

        Prints the most recent power telemetry sample.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def enable_power_stream(self, stream_rate_ms: int) -> Result:
        r"""Stream Power.

        Wire: ``h\p\o``

        Streams battery, charger and power-zone telemetry to the host at the given rate. 0 stops the stream.

        Enter Sample Time in milliseconds

        Args:
            stream_rate_ms: stream_rate_ms (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(stream_rate_ms)], [])

    def set_wio_reset_line(self, state: enums.resetLineState | int) -> Result:
        r"""Set WIO Reset Line.

        Wire: ``h\p\w``

        Holds or releases the LoRa module's reset line (zone 18, WIO_RST). 1 lets the module run, 0 holds it in reset.

        # Set WIO Reset Line

Zone 18 is `WIO_RST`, the PIC's `RG3` output into the LoRa module's `NRST` pin. It is a reset line and not a power rail, so it has a level rather than an on/off, and `NRST` is active low.

## Usage

```
w 1
```

## Arguments

- `state` - 0 = `hold_in_reset`, line driven low, the module is held in reset. 1 = `release`, line driven high, the module is free to run.

## Notes

- Ok is deferred until the PIC's status frame reports that pin back, so it means the line really moved. A timeout prints what it read instead.
- Zone 4 must be on. This command only moves a reset line, and releasing a module whose `3V3_S4` rail is off achieves nothing.
- Releasing reset says nothing about what firmware the module holds. It is what makes the module answerable at all.

        Enter 0 to hold the LoRa module in reset, 1 to release it

        Args:
            state: state (resetLineState).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_int(state)], [])

    def set_cm0_run_line(self, state: enums.resetLineState | int) -> Result:
        r"""Set CM0 Run Line.

        Wire: ``h\p\c``

        Holds or releases the Linux CPU's run line (zone 19, CM0_RUNPG). 1 lets the module run, 0 holds it in reset.

        # Set CM0 Run Line

Zone 19 is `CM0_RUNPG`, the PIC's `RH0` output into the compute module's `RUN_PG` pin. Active high: low holds the Linux CPU in reset, high lets it run.

## Usage

```
c 1
```

## Arguments

- `state` - 0 = `hold_in_reset`, line driven low. 1 = `release`, line driven high, the CPU runs.

## Notes

- Zone 17 must be on. Driving this line high into an unpowered module is the fault that Enable Linux CPU in the Linux menu avoids by setting the rail and the line in one frame, so prefer that command for ordinary use and this one when you need the line alone.
- Ok is deferred until the PIC reports the pin at the requested level.

        Enter 0 to hold the Linux CPU in reset, 1 to release it

        Args:
            state: state (resetLineState).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(state)], [])

    def get_control_lines(self) -> Result:
        r"""Get Control Lines.

        Wire: ``h\p\n``

        Reads back the pin levels of the three control lines, WIO_RST, CM0_RUNPG and MAIN_PWR_RST.

        # Get Control Lines

Reads back the three control-line pin levels: `WIO_RST` (zone 18), `CM0_RUNPG` (zone 19) and `MAIN_PWR_RST` (zone 20).

## Usage

```
n
```

## Returns

Three flags in that order, 1 meaning the line is high. For zones 18 and 19 high means released. `MAIN_PWR_RST` is reported as a raw level because its active polarity is not documented in this repo, and it is never written from the console.

## Notes

- These bits are decoded from the PIC status frame's port bytes, the same source as the 17 rails, so they are a read-back rather than an echo of what was last requested.
- Get Zones prints the same three bits with labels.

        Returns:
            Result: Ok(wio_released: bool, cm0_released: bool, main_rst_high: bool) or Err(message).
        """
        return self._call("n", [], ["bool", "bool", "bool"])
