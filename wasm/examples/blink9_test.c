/* Task 3 HW-verification aid (not a checked-in fixture): toggles GPIO 9
 * (within i\g\u's 16-bit read_all window, unlike GPIO25) so the launch +
 * yieldFromWasm proof can be confirmed bit-exact over the wire instead of
 * by camera. Mirrors blink.c's ow_call path exactly, just a different pin
 * and a slower/longer cadence so a polling script can catch every edge. */
#include "onewili_wasm.h"

__attribute__((export_name("_start")))
void _start(void) {
    ow_device dev;
    int i;
    if (ow_open_wasm(&dev) != OW_OK) return;
    for (i = 0; i < 6; ++i) {
        ow_io_gpio_set_io_toggle(&dev, 9);
        waitms(700);
    }
}
