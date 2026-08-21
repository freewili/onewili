"""Logic Analyzer Functions menu - generated from fwMenuLogicAnalyzer. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class LogicAnalyzer(MenuBase):
    r"""Logic Analyzer Functions (``i\b``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "logicAnalyzerReport": {"binary": True, "header_type": 2, "payload": [("trigger_time_stamp_ns", "hexU64"), ("sample_rate_ns", "decU32"), ("samples", "hexbytes")], "description": "Logic analyzer capture report (binary API; header then digital + analog samples)"},
    }

    def setup_logic_analyzer(self, sample_rate_ns: int, sample_count: int, pin_start: int, pin_stop: int, trigger_pin: int, trigger_type: int, rearm: int) -> Result:
        r"""configure.

        Wire: ``i\b\c``

        Configures the logic analyzer capture.

        # Configure Logic Analyzer

Configures a digital logic capture on a contiguous range of GPIO pins, with an optional edge trigger and auto-rearm.

## Arguments

| # | Name | Description |
|---|------|-------------|
| 1 | `sampleRateNs` | Sample period in **nanoseconds** (time between samples). Smaller = faster. |
| 2 | `sampleCount` | Total number of samples to capture per run. |
| 3 | `pinStart` | First GPIO pin in the capture range (inclusive). |
| 4 | `pinStop` | Last GPIO pin in the capture range (inclusive). Bits per sample = `pinStop - pinStart + 1`. |
| 5 | `triggerPin` | GPIO pin used as the trigger source. |
| 6 | `triggerType` | Trigger mode (see below). |
| 7 | `rearm` | `0` = single-shot, `1` = automatically rearm after each capture. |

## Trigger Types

- `0` — Falling edge on `triggerPin`
- `1` — Rising edge on `triggerPin`
- `2` — None (one-shot, fires immediately on `start`)
- `3` — Continuous (free-running capture)

## Returns

- `success` — `true` if the configuration was accepted, `false` otherwise.

## Example

Capture 4096 samples on GPIO 0–7 at 1 µs/sample, triggering on a rising edge of GPIO 2, with auto-rearm enabled:

```
c 1000 4096 0 7 2 1 1
```

## Notes

- Issue `s` (start) after configuring to begin capture, and `e` (stop) to halt.
- For analog capture, configure separately with `a`.

        Enter SampleRate (ns) SampleCount PinStart PinStop TriggerPin TriggerType (0=falling,1-rising,2-none,3-Continuous) Rearm (0/1)

        Args:
            sample_rate_ns: sample_rate_ns (decS32).
            sample_count: sample_count (decS32).
            pin_start: pin_start (decS32).
            pin_stop: pin_stop (decS32).
            trigger_pin: trigger_pin (decS32).
            trigger_type: trigger_type (decS32).
            rearm: rearm (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(sample_rate_ns), encoding.enc_int(sample_count), encoding.enc_int(pin_start), encoding.enc_int(pin_stop), encoding.enc_int(trigger_pin), encoding.enc_int(trigger_type), encoding.enc_int(rearm)], [])

    def setup_analog(self, analog_mask: int, analog_rate_ns: int, analog_res: int) -> Result:
        r"""configure analog.

        Wire: ``i\b\a``

        Configures the analog capture inputs.

        Enter AnalogMask (bit0=GPIO43..bit3=GPIO46, 0=off) AnalogRateNs AnalogRes(8/16)

        Args:
            analog_mask: analog_mask (decS32).
            analog_rate_ns: analog_rate_ns (decS32).
            analog_res: analog_res (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(analog_mask), encoding.enc_int(analog_rate_ns), encoding.enc_int(analog_res)], [])

    def start(self) -> Result:
        r"""start.

        Wire: ``i\b\s``

        Starts logic analyzer capture.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def stop(self) -> Result:
        r"""stop.

        Wire: ``i\b\e``

        Stops logic analyzer capture.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def trigger(self, trigger_type: int) -> Result:
        r"""trigger.

        Wire: ``i\b\t``

        Manually triggers the logic analyzer.

        Enter trigger type (0=falling, 1=rising, 2=none, 3=continuous)

        Args:
            trigger_type: trigger_type (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_int(trigger_type)], [])
