# Errors

A failed command answers `Err` with a message. Most messages are free text from the handler; the codes below are structured, so a binding may match on them.

## EPOWERZONE

The command needs hardware behind a power zone that is switched off, so the firmware refused it without running the handler.

```
EPOWERZONE <zone[,zone...]> <name[,name...]>
```

Zone numbers are decimal and ascending, and the names follow in the same order with internal spaces replaced by underscores, so the body is always three whitespace-separated tokens:

```
EPOWERZONE 7,13 SD_Card,NFC/RFID
```

Switch the zones on (`h\p\s`, or the Power Zones panel) and call again. A command whose reference page carries no `Requires power zone` line is never refused this way. A zone the firmware has just been told to switch on counts as on for three seconds, so a call issued immediately after enabling one is not refused while the board manager catches up; before the first zone report arrives after boot, nothing is refused at all.

## Power zones

| Zone | Name |
|---|---|
| 1 | Sensors |
| 2 | Display |
| 3 | Audio |
| 4 | Sub-GHz |
| 5 | ESP32 |
| 6 | FPGA |
| 7 | SD Card |
| 8 | USB Hub |
| 9 | Board LED |
| 10 | LEDs |
| 11 | Analog |
| 12 | Aux |
| 13 | NFC/RFID |
| 14 | FTDI |
| 15 | CAN |
| 16 | Debug CPU |
| 17 | CM0 |

Zone 9 drives the board-manager LED rather than a power rail, so no command declares it.
