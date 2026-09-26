# OneWili

The complete command API for [FREE-WILi](https://freewili.com) devices, in
every language we ship it in.

One device API, one wire protocol, 614 commands across 82 menus — everything
the on-device menu system can do, callable from a PC, from another CPU on the
board, or from a program running on the device itself.

**Reference documentation: <https://freewili.com/onewili/>**

## Packages

| Package | Language | Runs on | Talks to the device over |
| --- | --- | --- | --- |
| [`python/`](python) | Python 3.10+ | your PC | USB serial (auto-discovery via `pyfwfinder`) |
| [`c/`](c) | C11 | your PC, or any host MCU | your own read/write callbacks; a Win32/POSIX serial one is included |
| [`rust/`](rust) | Rust 2021 | your PC | USB serial (`serialport`) |
| [`wilibsp/`](wilibsp) | C11 | the FreeWili 2 **display CPU** (RP2350B) | the FwGUI display link to the main CPU |
| [`wasm/`](wasm) | C11 + Rust | inside the device's **WASM interpreter** | a single `ow_call` host import |
| [`cm0/`](cm0) | C++ and Python | a **CM0 Linux host** | the FPGA mailbox console link |

The first three are host packages — plug a FREE-WILi into a PC and drive it.
The last three run *on* the hardware and reach the main CPU from wherever they
happen to live.

## Quick start

Python:

```bash
cd python && pip install -e .
```

```python
import onewili

dev = onewili.connect()          # finds the board over USB
dev.io.gpio.set_io_toggle(25)    # every firmware menu is an attribute
```

Rust:

```bash
cd rust && cargo run --example toggle_gpio_25
```

C:

```bash
cd c && cmake -S . -B build && cmake --build build
```

```c
#include "onewili.h"

ow_device dev;
ow_open(&dev, &my_transport);
ow_io_gpio_set_io_toggle(&dev, 25);
```

Each package has its own `README.md` with the transport details, event
handling and more examples.

## CM0 Linux applications

Use [WiliCM0BSP](https://github.com/freewili/wilicm0bsp) for the Linux driver,
Python/C++ starter apps, agent instructions, and a pinned OneWili dependency.
Place Linux apps in **`/home/apps/` on the CM0 Linux filesystem** for easy
launching from the device's Linux > Apps browser.

The current CM0 bindings use isolated API mode (MAIN mailbox protocol 1.2+),
probe the connection, preserve MAIN USB command traffic, and support framed
file upload/download. See [cm0/README.md](cm0/README.md).

## Events and binary streaming

The Python, C and Rust host packages receive WILI binary frames from the
separate FTDI interface, including CAN FD and variable-length logic-analyzer
sample buffers. Raw APIs preserve unknown frame types. Python exposes bounded
queue/drop/error counters and a [PWM capture example](python/examples/capture_pwm.py).
C supports caller-owned receive buffers for large captures; Rust owns its frames.
See each package README for buffer lifetimes and limits.

Text and binary events are separate channels. Python runs background readers;
C and Rust expose polling calls. CM0 supports request/reply commands and framed
file transfers, but does not route FTDI binary events or USB directory-list
events over the mailbox. The on-device adapters likewise need an actual event
transport; decoder availability alone does not supply one.

## Generated code

The language bindings and reference documentation are generated from the FREE-WILi firmware's
menu sources by the menutool API generator, so the bindings cannot drift from
what the firmware actually accepts. **Do not edit the generated files** — a
regeneration will overwrite them. Fixes belong in the firmware menu sources
or in the generator.

Command IDs are stable and append-only: a command keeps its numeric ID for
life, and new commands are appended. Code compiled against an older release
keeps working against newer firmware.

Root documentation, CI, regression tests and some Python examples are maintained
here. Preserve them when syncing generated output. See [AGENTS.md](AGENTS.md).

## Support

Issues and questions: <https://github.com/freewili/onewili/issues>
