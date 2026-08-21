"""Named enumerations (fwMenuEnums.h) - generated. Do not edit."""
from __future__ import annotations

import enum


class owLEDManagerLEDMode(enum.IntEnum):
    """owLEDManagerLEDMode (uint8_t)."""

    SIMPLEVALUE = 0
    FLASH = 1
    PULSE = 2
    FLASHFADE = 3
    PULSEFADE = 4


class IOVoltageSource(enum.IntEnum):
    """IOVoltageSource (uint8_t)."""

    NONE = 0
    V3_3 = 1
    V5_0 = 2
    EXT_PIN = 3
    PROG_VOUT = 4


class dacWaveShapeMenu(enum.IntEnum):
    """dacWaveShapeMenu (uint8_t)."""

    OFF = 0
    TRIANGLE = 1
    SAWTOOTH = 2
    INVSAWTOOTH = 3
    SINE = 4


class dacWavePhase(enum.IntEnum):
    """dacWavePhase (uint8_t)."""

    DEG0 = 0
    DEG120 = 1
    DEG240 = 2
    DEG90 = 3


class resetLineState(enum.IntEnum):
    """resetLineState (uint8_t)."""

    HOLD_IN_RESET = 0
    RELEASE = 1


class owLEDLightShow(enum.IntEnum):
    """owLEDLightShow (uint8_t)."""

    MANUAL = 0
    RAINBOW = 1
    SNOWSTORM = 2
    REDCHASE = 3
    RAINBOWCHASE = 4
    BLUECHASE = 5
    GREENDOT = 6
    BLUEDOT = 7
    BLUESIN = 8
    WHITEFADE = 9
    BARGRAPH = 10
    ZYLON = 11
    AUDIO = 12
    ACCEL = 13


class owSerialLEDType(enum.IntEnum):
    """owSerialLEDType (uint8_t)."""

    RGB = 0
    RGBW = 1


class owScreenshotFileType(enum.IntEnum):
    """owScreenshotFileType (uint8_t)."""

    PNG = 0
    FWI = 1


class owGUIButton(enum.IntEnum):
    """owGUIButton (uint8_t)."""

    GRAY = 0
    YELLOW = 1
    GREEN = 2
    BLUE = 3
    RED = 4
    UP = 33
    DOWN = 34
    LEFT = 35
    RIGHT = 36
    CENTER = 37
    OK = 38
    CANCEL = 39
    HOME = 40
    PAGE = 41


class owButtonPressType(enum.IntEnum):
    """owButtonPressType (uint8_t)."""

    PRESS = 0
    LONGPRESS = 1
    PRESSANDSTAY = 2
    RELEASE = 3
