# Enumerations

Named enumerations from `fwMenuEnums.h` - usable as argument and return types. Wire encoding is the plain decimal value.

## owLEDManagerLEDMode

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| simplevalue | 0 |
| flash | 1 |
| pulse | 2 |
| flashfade | 3 |
| pulsefade | 4 |

Used by: `gui.set_led_color`

## IOVoltageSource

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| none | 0 |
| v3_3 | 1 |
| v5_0 | 2 |
| ext_pin | 3 |
| prog_vout | 4 |

## dacWaveShapeMenu

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| off | 0 |
| triangle | 1 |
| sawtooth | 2 |
| invsawtooth | 3 |
| sine | 4 |

Used by: `io.analog_out.set_waveform`

## dacWavePhase

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| deg0 | 0 |
| deg120 | 1 |
| deg240 | 2 |
| deg90 | 3 |

Used by: `io.analog_out.set_waveform`

## resetLineState

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| hold_in_reset | 0 |
| release | 1 |

Used by: `hardware.power_management.set_wio_reset_line` `hardware.power_management.set_cm0_run_line`

## owLEDLightShow

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| manual | 0 |
| rainbow | 1 |
| snowstorm | 2 |
| redchase | 3 |
| rainbowchase | 4 |
| bluechase | 5 |
| greendot | 6 |
| bluedot | 7 |
| bluesin | 8 |
| whitefade | 9 |
| bargraph | 10 |
| zylon | 11 |
| audio | 12 |
| accel | 13 |

Used by: `io.serial_leds.set_show`

## owSerialLEDType

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| rgb | 0 |
| rgbw | 1 |

Used by: `io.serial_leds.configure_strip`

## owScreenshotFileType

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| png | 0 |
| fwi | 1 |

Used by: `gui.screenshot`

## owGUIButton

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| gray | 0 |
| yellow | 1 |
| green | 2 |
| blue | 3 |
| red | 4 |
| up | 33 |
| down | 34 |
| left | 35 |
| right | 36 |
| center | 37 |
| ok | 38 |
| cancel | 39 |
| home | 40 |
| page | 41 |

Used by: `gui.simulate_keypress`

## owButtonPressType

Underlying type: `uint8_t`

| Label | Value |
|---|---|
| press | 0 |
| longpress | 1 |
| pressandstay | 2 |
| release | 3 |

Used by: `gui.simulate_keypress`
