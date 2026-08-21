/* Wraps one SDFS frame as a FwGUI DISPLAY->MAIN event frame:
 *
 *   B0 1D | len u16le | 42 | sdfs-frame | cksum u16le
 *
 * `len` counts the SDFS frame ONLY -- the event-id byte is not included (see
 * the FwGUI protocol spec). The checksum is the 16-bit additive sum of every
 * byte from the sync words through the last frame byte.
 *
 * Byte-for-byte identical to the display firmware's request framing
 * (targets/fw2display/serial_comm_display.cpp:rmet_send), which is what lets
 * fw2main's SDFS server answer a WiliBSP app without any firmware change.
 *
 * Pure arithmetic, no pico-sdk dependency, so the host test suite can pin it. */
#ifndef OW_SDFS_FRAME_H
#define OW_SDFS_FRAME_H
#include <stddef.h>
#include <stdint.h>

#define OW_SDFS_EVENT_SYNC0    0xB0
#define OW_SDFS_EVENT_SYNC1    0x1D
#define OW_SDFS_EVENT_ID       42     /* FWGUI_EVENT_SDFS_REQUEST */
#define OW_SDFS_EVENT_OVERHEAD 7      /* sync(2) + len(2) + id(1) + cksum(2) */

/* Returns bytes written to `out`, or 0 when `len` is 0 or `cap` is too small. */
static inline size_t ow_sdfs_frame_event(uint8_t* out, size_t cap,
                                         const uint8_t* frame, size_t len) {
    size_t k = 0, i;
    uint16_t sum = 0;
    if (!out || !frame || len == 0) return 0;
    if (cap < OW_SDFS_EVENT_OVERHEAD || len > cap - OW_SDFS_EVENT_OVERHEAD) return 0;
    out[k++] = OW_SDFS_EVENT_SYNC0;
    out[k++] = OW_SDFS_EVENT_SYNC1;
    out[k++] = (uint8_t)(len & 0xFF);
    out[k++] = (uint8_t)((len >> 8) & 0xFF);
    out[k++] = OW_SDFS_EVENT_ID;
    for (i = 0; i < len; i++) out[k++] = frame[i];
    for (i = 0; i < k; i++) sum = (uint16_t)(sum + out[i]);
    out[k++] = (uint8_t)(sum & 0xFF);
    out[k++] = (uint8_t)((sum >> 8) & 0xFF);
    return k;
}

#endif /* OW_SDFS_FRAME_H */
