#ifndef SDFS_TRANSPORT_H
#define SDFS_TRANSPORT_H

#include "sdfs_wire.h"

/* Frame transport bridging into the existing UART protocol.
 * Frames are delivered reliably, in order, and error-checked by the
 * underlying protocol; this layer adds no CRC or retransmission. */
typedef struct sdfs_transport {
    /* Send one complete frame. Return 0 on success, <0 if the frame could not
     * be queued (transient full or hard error); callers may retry on <0. */
    int (*send)(void *ctx, const uint8_t *frame, size_t len);
    /* Try to receive one frame. On success copy into buf, set *len, return 1.
     * Return 0 if no frame is available right now. Return <0 on error; a frame
     * too large for cap is discarded (not redeliverable) and reported as <0. */
    int (*recv)(void *ctx, uint8_t *buf, size_t cap, size_t *len);
    void *ctx;
} sdfs_transport_t;

#endif /* SDFS_TRANSPORT_H */
