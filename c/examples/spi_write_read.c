/* Full-duplex SPI transaction: clock out a frame on MOSI, decode the MISO
 * response bytes to stdout, wait for a key, exit.
 *
 * Usage: spi_write_read <port> [hex bytes...]     default frame: 9F 00 00 00
 *        e.g. spi_write_read COM5 AB CD EF
 */
#include "onewili.h"
#include "serial_pc.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
/* _isatty() is true for any char device (even NUL) - ask for a real console. */
static int stdin_is_console(void) {
    DWORD mode;
    return GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode) != 0;
}
#define wait_key() (void)_getch()
#else
#include <unistd.h>
#define stdin_is_console() isatty(fileno(stdin))
#define wait_key() (void)getchar()
#endif

int main(int argc, char** argv) {
    if (argc < 2) { printf("usage: %s <serial port> [hex bytes...]\n", argv[0]); return 2; }

    /* Frame to send: argv bytes, or the SPI-flash JEDEC-ID probe 9F 00 00 00. */
    uint8_t tx[256];
    size_t txlen = 0;
    if (argc > 2) {
        for (int i = 2; i < argc && txlen < sizeof tx; ++i)
            tx[txlen++] = (uint8_t)strtoul(argv[i], NULL, 16);
    } else {
        tx[0] = 0x9F; tx[1] = 0x00; tx[2] = 0x00; tx[3] = 0x00;
        txlen = 4;
    }

    serial_pc* sp = serial_pc_open(argv[1]);
    if (!sp) { printf("cannot open %s\n", argv[1]); return 1; }
    ow_transport t = serial_pc_transport(sp);
    ow_device dev;
    if (ow_open(&dev, &t) != OW_OK) { serial_pc_close(sp); return 1; }

    printf("SPI tx (%zu bytes):", txlen);
    for (size_t i = 0; i < txlen; ++i) printf(" %02X", tx[i]);
    printf("\n");

    uint8_t rx[256];
    size_t rxlen = 0;
    ow_status r = ow_io_spi_s_pi_write(&dev, tx, txlen, rx, sizeof rx, &rxlen);
    if (r == OW_OK) {
        printf("SPI rx (%zu bytes):", rxlen);
        for (size_t i = 0; i < rxlen; ++i) printf(" %02X", rx[i]);
        printf("\n           as text: \"");
        for (size_t i = 0; i < rxlen; ++i)
            putchar(rx[i] >= 0x20 && rx[i] < 0x7F ? rx[i] : '.');
        printf("\"\n");
    } else {
        printf("SPI transfer failed: ow_status %d\n", (int)r);
    }

    ow_close(&dev);
    serial_pc_close(sp);

    if (stdin_is_console()) {           /* scriptable: only wait on a real console */
        printf("\nPress any key to close...\n");
        fflush(stdout);
        wait_key();
    }
    return r == OW_OK ? 0 : 1;
}
