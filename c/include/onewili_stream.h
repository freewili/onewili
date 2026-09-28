/* onewili_stream.h -- peer streams: best-effort datagrams between OneWili
 * clients (DISPLAY, ESP32, CM0, the PC host), routed by MAIN.
 *
 * The same API, with the same meaning, on every target. Only the route
 * differs: a push-capable transport (the DISPLAY's FwGUI link, the ESP32's
 * Bottlenose link) binds fast-path ops with ow_stream_bind() after ow_open;
 * everywhere else (PC, CM0) the calls ride three MAIN menu commands.
 *
 *     ow_stream_write(&dev, OW_PEER_ESP32, "hi", 2);
 *     uint8_t buf[OW_STREAM_MTU]; ow_peer from;
 *     int n = ow_stream_poll(&dev, &from, buf, sizeof buf);   // >0 = a datagram
 *
 * Semantics (identical everywhere):
 *  - A datagram is 1..OW_STREAM_MTU bytes, delivered whole or not at all.
 *    There is no ordering guarantee across different senders, no retry and
 *    no acknowledgement. Layer reliability on top if you need it.
 *  - Nothing ever waits for the destination: a datagram it cannot take right
 *    now is dropped and counted. ow_stream_write returns OW_OK once the
 *    datagram has left this client; OW_ERR_BUFFER if this client refused it
 *    (flow-control window full, or the link not yet confirmed by MAIN);
 *    OW_ERR_IO if the link could not send it; OW_ERR_ARG for a bad length or
 *    peer. On the text route a failed command returns its own status.
 *  - ow_stream_poll returns one datagram per call: its length (>0), 0 when
 *    none is waiting, or -(ow_status) on error. A datagram longer than cap is
 *    dropped, counted, and reported as -(int)OW_ERR_BUFFER, so always pass a
 *    buffer of OW_STREAM_MTU bytes. On push links it never blocks.
 *  - ow_stream_drops counts every datagram involving this client that was
 *    lost anywhere: refused here, lost on the link to MAIN, dropped at MAIN
 *    on its way out, dropped at MAIN on its way in, or dropped here for lack
 *    of room (or for a buffer smaller than the datagram). Free-running.
 *  - Datagrams reach a client only while it is using streams: a push-link
 *    client counts as present while it keeps calling ow_stream_* (the link
 *    sends a keepalive at least once a second); text clients are always
 *    queued for, up to a small per-client queue on MAIN.
 *  - OW_PEER_MAIN is reserved: MAIN has no stream consumer and drops
 *    datagrams addressed to it. Addressing yourself is allowed (loopback).
 *
 * Push links are single-threaded: call ow_stream_* for one device from one
 * task (or core), and do not interleave them with ow_* calls on another task
 * unless the transport documents otherwise.
 */
#ifndef ONEWILI_STREAM_H
#define ONEWILI_STREAM_H

#include "onewili.h"
#include "ow_stream_wire.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OW_PEER_MAIN    = OW_STREAM_PEER_MAIN,
    OW_PEER_DISPLAY = OW_STREAM_PEER_DISPLAY,
    OW_PEER_ESP32   = OW_STREAM_PEER_ESP32,
    OW_PEER_CM0     = OW_STREAM_PEER_CM0,
    OW_PEER_HOST    = OW_STREAM_PEER_HOST
} ow_peer;

ow_status ow_stream_write(ow_device* dev, ow_peer dst, const void* buf, uint32_t len);
int       ow_stream_poll (ow_device* dev, ow_peer* src, void* buf, uint32_t cap);
uint32_t  ow_stream_drops(ow_device* dev);

/* ---- for transport implementers ---------------------------------------- */

/* Fast-path ops a push-capable transport installs after ow_open. */
typedef struct ow_stream_ops {
    void* ctx;
    ow_status (*write)(void* ctx, uint8_t dst, const uint8_t* data, uint32_t len);
    int       (*poll) (void* ctx, uint8_t* src, uint8_t* buf, uint32_t cap);
    uint32_t  (*drops)(void* ctx);
} ow_stream_ops;

void ow_stream_bind(ow_device* dev, const ow_stream_ops* ops);

/* Client half of the push-link protocol in ow_stream_wire.h (HELLO,
 * keepalive, CREDIT window, drop accounting), shared by every push
 * transport. Single-threaded: call everything from the thread that calls
 * ow_stream_*. The transport supplies:
 *   send   -- transmit one stream payload P (dst, src, data) as exactly one
 *             link frame; return 0 on success, <0 if it could not be sent
 *   now_ms -- a free-running millisecond clock
 * and feeds every received control frame (P[0] >= OW_STREAM_CTL_FIRST) to
 * ow_stream_link_control(). Data frames are the transport's to queue.
 * Apply every control frame already received before calling
 * ow_stream_link_service(): it judges lost frames on the newest CREDIT. */
typedef struct ow_stream_link {
    uint8_t  self;             /* this client's peer id */
    uint8_t  confirmed;        /* a CREDIT has arrived */
    uint8_t  hello_sent;
    uint8_t  credit_fresh;     /* a CREDIT arrived since the last service() */
    uint32_t sent_total;       /* OW_STREAM_WIRE_BYTES sent, MAIN-aligned */
    uint32_t consumed;         /* last consumed_total from MAIN */
    uint32_t from_base, from_last;
    uint32_t to_base, to_last;
    uint32_t drop_accum;       /* MAIN-reported drops carried over a MAIN restart */
    uint32_t tx_refused;       /* writes refused here (window / unconfirmed / send failure) */
    uint32_t tx_lost;          /* datagrams that died before MAIN counted them (lower bound) */
    uint32_t tx_frames, credits;
    uint32_t last_hello_ms;
    uint32_t last_tx_ms;       /* when the last data frame went out */
    int      (*send)(void* ctx, const uint8_t* p, uint32_t len);
    uint32_t (*now_ms)(void* ctx);
    void*    ctx;
} ow_stream_link;

void      ow_stream_link_init(ow_stream_link* l, uint8_t self,
                              int (*send)(void*, const uint8_t*, uint32_t),
                              uint32_t (*now_ms)(void*), void* ctx);
void      ow_stream_link_reset(ow_stream_link* l);   /* back to unconfirmed, counters kept */
void      ow_stream_link_service(ow_stream_link* l); /* HELLO retry / keepalive / loss write-off */
void      ow_stream_link_control(ow_stream_link* l, const uint8_t* p, uint32_t len);
ow_status ow_stream_link_write(ow_stream_link* l, uint8_t dst, const uint8_t* data, uint32_t len);
uint32_t  ow_stream_link_drops(const ow_stream_link* l);  /* excludes the transport's own RX drops */

#ifdef __cplusplus
}
#endif

#endif /* ONEWILI_STREAM_H */
