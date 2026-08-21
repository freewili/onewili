/* Watch FreeWili events on both ports for a few seconds.
 * Usage: watch_events <text port> <binary port> [seconds]   (default 10)
 * The binary port is the FTDI interface (VID 0x0403) - find both with
 * list_devices. NEVER match it by the "FW2" name: the text port is "FW2 v01". */
#include "onewili.h"
#include "onewili_binary.h"
#include "onewili_events.h"
#include "serial_pc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("usage: %s <text port> <binary port> [seconds]\n", argv[0]);
        return 2;
    }
    int seconds = argc > 3 ? atoi(argv[3]) : 10;

    serial_pc* sp = serial_pc_open(argv[1]);
    if (!sp) { printf("cannot open %s\n", argv[1]); return 1; }
    ow_transport t = serial_pc_transport(sp);
    ow_device dev;
    if (ow_open(&dev, &t) != OW_OK) { serial_pc_close(sp); return 1; }

    serial_pc* bsp = serial_pc_open(argv[2]);
    if (!bsp) {                     /* close the text port on binary failure */
        printf("cannot open binary port %s\n", argv[2]);
        ow_close(&dev);
        serial_pc_close(sp);
        return 1;
    }
    ow_transport bt = serial_pc_transport(bsp);
    ow_binary_device bdev;
    if (ow_binary_open(&bdev, &bt) != OW_OK) {
        ow_close(&dev);
        serial_pc_close(bsp);
        serial_pc_close(sp);
        return 1;
    }

    ow_io_gpio_stream_io(&dev, 10);   /* start streaming */

    unsigned bin_count = 0, txt_count = 0;
    time_t end = time(NULL) + seconds;
    while (time(NULL) < end) {
        ow_event ev;
        int got = 0;
        if (ow_binary_poll(&bdev, &ev) == 1) {
            ++bin_count; got = 1;
            printf("binary event kind=%d\n", (int)ev.kind);
        }
        if (ow_poll_text_event(&dev, &ev) == 1) {
            ++txt_count; got = 1;
            printf("text event [%s] %s\n", ev.u.text.id, ev.u.text.args);
        }
        if (!got) sleep_ms(5);
    }
    ow_io_gpio_stream_io(&dev, 0);   /* stop streaming */

    printf("%u binary + %u text events in %d s "
           "(unknown frames: %u, size mismatches: %u, dropped text: %u)\n",
           bin_count, txt_count, seconds,
           bdev.unknown_frames, bdev.size_mismatches, dev.dropped_text_events);
    ow_binary_close(&bdev);
    ow_close(&dev);
    serial_pc_close(bsp);
    serial_pc_close(sp);
    return 0;
}
