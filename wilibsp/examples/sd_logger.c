/* Append a line to the SD card once a second, then read the file back.
 * Exercises every layer: link handshake, SD arming, handle writes, whole-file
 * read, and directory listing. */
#include "onewili.h"
#include "onewili_fwgui.h"
#include "onewili_sd.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

static void print_entry(const char* name, bool is_dir, uint32_t size, void* user) {
    (void)user;
    printf("  %-24s %s %lu\n", name, is_dir ? "<DIR>" : "     ", (unsigned long)size);
}

int main(void) {
    stdio_init_all();
    ow_device dev;
    if (ow_open_fwgui(&dev) != OW_OK) return 1;

    /* mkdir is fine to repeat: an existing directory reports a clean error. */
    ow_sd_mkdir(&dev, "/owlog");

    for (int i = 0; i < 10; i++) {
        char line[64];
        int n = snprintf(line, sizeof line, "tick %d\n", i);
        ow_sd_file f;
        if (ow_sd_open(&dev, &f, "/owlog/run.txt", OW_SD_APPEND) != OW_OK) {
            printf("open failed (sdfs %d)\n", (int)ow_sd_last_error());
        } else {
            ow_sd_write(&f, line, (size_t)n);
            /* Writes are fire-and-forget; close is where failure shows up. */
            if (ow_sd_close(&f) != OW_OK)
                printf("close failed (sdfs %d)\n", (int)ow_sd_last_error());
        }
        sleep_ms(1000);
    }

    static char buf[1024];
    size_t got = 0;
    if (ow_sd_get_mem(&dev, "/owlog/run.txt", buf, sizeof buf - 1, &got) == OW_OK) {
        buf[got] = '\0';
        printf("read back %u bytes:\n%s", (unsigned)got, buf);
    } else {
        printf("read failed (sdfs %d)\n", (int)ow_sd_last_error());
    }

    printf("/owlog:\n");
    ow_sd_list(&dev, "/owlog", print_entry, 0);

    for (;;) sleep_ms(1000);
}
