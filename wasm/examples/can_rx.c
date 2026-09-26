/* Minimal WASM guest: echo every CAN(FD) frame received on channel 0 back
 * out with the arbitration id + 1.
 *
 * On-device scripts (rthon, WASM) never see the spontaneous `[*can0 ...]`
 * stream events: the local menu executor hands a script only a command's own
 * response. The intended pattern is therefore a poll loop - arm the MAIN-side
 * receive queue once, then call receive_canfd repeatedly. receive_canfd
 * services the controller FIFO itself, and waitms() yields to the firmware
 * main loop, so a tight poll with a short sleep neither starves the device
 * nor loses frames (a burst that overruns the queue shows up in `dropped`).
 * Build like blink.c (see WiliWasm/test/fixtures/README.md). */
#include "onewili_wasm.h"

__attribute__((export_name("_start")))
void _start(void) {
    ow_device dev;
    if (ow_open_wasm(&dev) != OW_OK) return;
    if (ow_io_canfd_enable_canfd_receive_queue(&dev, 0, 1) != OW_OK) return;
    for (;;) {
        bool frame = false;
        int32_t queued = 0, dropped = 0, xtd_id = 0, can_fd = 0;
        int32_t timestamp_us = 0, dlc = 0;
        uint32_t arb_id = 0;
        uint8_t data[64];
        size_t data_len = 0;
        if (ow_io_canfd_receive_canfd(&dev, 0, &frame, &queued, &dropped, &arb_id,
                                      &xtd_id, &can_fd, &timestamp_us, &dlc,
                                      data, sizeof data, &data_len) == OW_OK && frame) {
            ow_io_canfd_write_canfd(&dev, 0, arb_id + 1, can_fd, xtd_id, data, data_len);
        } else {
            waitms(5);
        }
    }
}
