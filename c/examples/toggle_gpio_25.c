/* Toggle GPIO 25 on a FreeWili.  Usage: toggle_gpio_25 <port>
 * Find the port with the list_devices example (or Device Manager). */
#include "onewili.h"
#include "serial_pc.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc < 2) { printf("usage: %s <serial port>\n", argv[0]); return 2; }
    serial_pc* sp = serial_pc_open(argv[1]);
    if (!sp) { printf("cannot open %s\n", argv[1]); return 1; }
    ow_transport t = serial_pc_transport(sp);
    ow_device dev;
    if (ow_open(&dev, &t) != OW_OK) { serial_pc_close(sp); return 1; }
    ow_status r = ow_io_gpio_set_io_toggle(&dev, 25);
    printf(r == OW_OK ? "toggled GPIO 25\n" : "failed: %d\n", (int)r);
    ow_close(&dev);
    serial_pc_close(sp);
    return r == OW_OK ? 0 : 1;
}
