# OneWili — CM0 (mailbox) target

The generated OneWili C device API (`dev.*` menu commands) reachable from the
CM0 Linux host over the FPGA mailbox console link into MAIN's `fwMenuMain`.

Usage:
```cpp
ConsoleClient client(router);   // from fwcm0 (LinuxTransport -> SramRouter -> ConsoleClient)
client.connect();
fwcm0::OneWiliLink link(client);
ow_transport t = link.transport();
ow_device dev; ow_open(&dev, &t);
ow_io_gpio_set_io_high(&dev, 25);
ow_close(&dev);
```

Python:
```python
from onewili_cm0 import connect_cm0
dev = connect_cm0()
dev.io.gpio.set_io_high(25)
dev.close()
```
Needs the `onewili` Python package (generated alongside this target with
`--langs python`) plus the `fwcm0` CLI installed and on `PATH`; `connect_cm0()`
pipes OneWili wire bytes through `fwcm0 console` instead of a serial port.

The CM0's SRAM/PSRAM router is NOT part of this menu API. It is a local,
non-menu capability reached directly through `fwcm0::SramRouter`
(status/read/write/request_swap), analogous to the hand-written file-I/O
imports every WASM guest keeps alongside the generated surface.
