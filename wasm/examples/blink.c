/* Minimal WASM guest: toggle GPIO 25 ten times, half a second apart.
 * Build with wiliclang for wasm32; the firmware runs it. */
#include "onewili_wasm.h"

__attribute__((export_name("_start")))
void _start(void) {
    ow_device dev;
    int i;
    if (ow_open_wasm(&dev) != OW_OK) return;
    for (i = 0; i < 10; ++i) {
        ow_io_gpio_set_io_toggle(&dev, 25);
        waitms(500);
    }
}
