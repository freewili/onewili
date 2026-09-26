# OneWili — CM0 (mailbox) target

The generated OneWili C device API (`dev.*` menu commands) reachable from the
CM0 Linux host over the FPGA mailbox console link into MAIN's `fwMenuMain`.

Usage:
```cpp
MboxMux mux(router);            // from fwcm0 (LinuxTransport -> SramRouter -> MboxMux)
ConsoleClient client(mux);      // direct hardware access: stop fwcm0-bridge first
fwcm0::OneWiliLink link(client); // connects and confirms isolated API mode
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
pipes OneWili wire bytes through `fwcm0 api` instead of a serial port.
Requires MAIN mailbox protocol 1.2 or newer. API commands have their own input
buffer and captured replies, so USB console commands can run concurrently.
The interactive `fwcm0 console` command remains available separately.
Opening verifies a read-only Device State reply from MAIN. A missing bridge,
failed mailbox handshake, or disconnected process raises an exception instead
of returning a device that silently times out. `timeout=3.0` controls this probe.

## File transfers in both directions

With updated MAIN firmware, the normal Python file API uses framed transfers
automatically on CM0. Linux paths are local to the process; device paths refer
to the FreeWili SD card (not the Linux SD card):

```python
dev = connect_cm0()
try:
    dev.files.put_file("/home/pi/capture.bin", "/capture.bin")
    dev.files.get_file("/capture.bin", "/home/pi/download.bin")
    dev.files.put("/message.bin", b"hello")
    assert dev.files.get("/message.bin") == b"hello"
finally:
    dev.close()
```

The C/C++ package includes `onewili_framed_files.h` with
`ow_framed_file_put_mem` and `ow_framed_file_get_mem`. Supply a fresh nonzero
session token. All generated bindings also expose begin/read/write/finish/cancel
operations under the filesystem menu. The same framed Python API is explicitly
available to USB callers as `dev.files.framed`.

Each request transfers at most 192 bytes. MAIN menus and the Linux shell remain
available between requests. The SD card must be mounted and owned by MAIN;
transfers never change its ownership. One file session is active at a time and
expires after 30 seconds without an operation. Paths are absolute UTF-8 paths,
up to 127 bytes; long directory prefixes may leave insufficient room for a
staging filename. Maximum file size is 2 GiB minus one byte.

Size and CRC32 are verified in both directions. Uploads use a temporary sibling
and preserve the old destination until validation succeeds. Replacement uses a
backup during publication; this is not an atomic transaction across power loss.
A retained `.ow-<session>.bak` is the old file and `.ow-<session>.part` is an
incomplete upload. Failure messages distinguish publication from cleanup errors.
A timed-out finish can have completed: inspect the destination before retrying.
No transfer command is replayed automatically. Python file downloads are staged
locally too, so a failed download leaves an existing destination intact.

Directory listing via `dev.files.list()` and binary event streaming still require
the USB transport; this addition supports file upload and download over CM0.

The CM0's SRAM/PSRAM router is NOT part of this menu API. It is a local,
non-menu capability reached directly through `fwcm0::SramRouter`
(status/read/write/request_swap), analogous to the hand-written file-I/O
imports every WASM guest keeps alongside the generated surface.
