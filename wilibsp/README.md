# OneWili C API for WiliBSP

The full FreeWili OneWili command API for firmware running on the FreeWili 2
**display CPU** (WiliBSP, RP2350B). The library is the same generated C
package the PC serial target uses; only the transport differs: commands
travel to the main CPU over the FwGUI display link (UART0, 8 Mbaud, hardware
flow control on GPIO0-3) and responses/events travel back on the same link.

The main CPU must run the default FreeWili 2 firmware (which carries the
OneWili display bridge).

## Use

```c
#include "onewili.h"
#include "onewili_fwgui.h"

ow_device dev;
ow_open_fwgui(&dev);                 /* UART0 + link handshake */
/* any generated call, e.g.: */
ow_io_gpio_set_io_toggle(&dev, 25);  /* toggles a MAIN-CPU gpio */
```

Text events arrive in-band — poll with `ow_poll_text_line(&dev, ...)`.
Binary events (`onewili_binary.h`):

```c
ow_binary_device bdev;
ow_transport bt = ow_fwgui_binary_transport();
ow_binary_open(&bdev, &bt);
ow_event ev;
while (ow_binary_poll(&bdev, &ev) == 1) { /* ... */ }
```

Poll events regularly: each stream buffers 1024 bytes and whole frames are
dropped (counted by `ow_fwgui_dropped_frames()`) when a buffer is full.
Logic-analyzer binary reports are never mirrored over the display link.

## SD card

The SD card is owned by the MAIN CPU. `ow_open_fwgui` arms an SD client that
reaches it over the same display link, so a wilibsp app can read and write the
card the same way the stock display firmware does:

```c
#include "onewili_sd.h"

ow_sd_file f;
if (ow_sd_open(&dev, &f, "/logs/run.txt", OW_SD_APPEND) == OW_OK) {
    ow_sd_write(&f, "hello\n", 6);
    if (ow_sd_close(&f) != OW_OK) { /* the write did not land -- see below */ }
}
```

Paths are absolute and `/`-rooted (there is no internal-flash route). At most
two files may be open at once. Writes are fire-and-forget, so **always check
`ow_sd_close`** — that is where a dropped chunk is reported. `ow_sd_last_error()`
gives the underlying sdfslib status; `ow_sd_set_timeout_ms()` changes the
2-second idle timeout. Whole-file helpers (`ow_sd_get_mem`, `ow_sd_put_mem`)
and metadata calls (`ow_sd_stat`, `ow_sd_list`, `ow_sd_mkdir`, `ow_sd_remove`,
`ow_sd_rename`) need no handle.

## Build

`CMakeLists.txt` builds a `onewili_fwgui` static library against the
pico-sdk. Link it from your wilibsp app target and add `include/` (PUBLIC,
automatic via CMake). `examples/blink.c` is a minimal app body.
