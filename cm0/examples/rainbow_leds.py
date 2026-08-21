#!/usr/bin/env python3
"""OneWili CM0 example - a live rainbow across the FreeWili 2 board LEDs, driven
from the CM0 (Raspberry Pi) over the FPGA mailbox and nudged by the GPIO inputs.

Runs the whole onewili_cm0 flow: connect over the mailbox, read an input
(io.gpio.read_all), drive outputs (gui.set_led_color) in a loop, clean up on
Ctrl-C.

    python3 rainbow_leds.py

Note: the accelerometer stream (io.sensors.enable_accel_stream) is accepted by a
menu command but its data is NOT delivered to the CM0 over the mailbox console,
so this uses the working GPIO read as its live input instead.
"""
import colorsys
import time

from onewili_cm0 import connect_cm0

NUM_LEDS = 8


def _rgb(hue):
    r, g, b = colorsys.hsv_to_rgb(hue % 1.0, 1.0, 1.0)
    return int(r * 255), int(g * 255), int(b * 255)


def main():
    dev = connect_cm0()
    print("connected to MAIN over the FPGA mailbox; Ctrl-C to stop")
    phase = 0.0
    try:
        while True:
            io = dev.io.gpio.read_all().unwrap_or(0)
            offset = (io & 0xFF) / 256.0
            for i in range(NUM_LEDS):
                r, g, b = _rgb(phase + offset + i / NUM_LEDS)
                dev.gui.set_led_color(i, r, g, b, 0, 0)
            phase += 0.03
            time.sleep(0.05)
    except KeyboardInterrupt:
        pass
    finally:
        for i in range(NUM_LEDS):
            dev.gui.set_led_color(i, 0, 0, 0, 0, 0)
        dev.close()
        print("\nLEDs off; disconnected")


if __name__ == "__main__":
    main()
