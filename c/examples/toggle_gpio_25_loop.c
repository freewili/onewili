/* Toggle GPIO 25 once a second.
 * Usage: toggle_gpio_25_loop <port> [count]   (default count: 10) */
#include "onewili.h"
#include "serial_pc.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

int main(int argc, char** argv) {
    if (argc < 2) { printf("usage: %s <serial port> [count]\n", argv[0]); return 2; }
    int count = argc > 2 ? atoi(argv[2]) : 10;

    serial_pc* sp = serial_pc_open(argv[1]);
    if (!sp) { printf("cannot open %s\n", argv[1]); return 1; }
    ow_transport t = serial_pc_transport(sp);
    ow_device dev;
    if (ow_open(&dev, &t) != OW_OK) { serial_pc_close(sp); return 1; }

    int failures = 0;
    for (int i = 1; i <= count; ++i) {
        ow_status r = ow_io_gpio_set_io_toggle(&dev, 25);
        printf("[%2d/%d] toggle GPIO 25: %s\n", i, count,
               r == OW_OK ? "ok" : "FAILED");
        fflush(stdout);
        if (r != OW_OK) ++failures;
        if (i < count) sleep_ms(1000);
    }

    ow_close(&dev);
    serial_pc_close(sp);
    printf("%d/%d toggles ok\n", count - failures, count);
    return failures ? 1 : 0;
}
