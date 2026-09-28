/* onewili_stream.c -- peer streams. See onewili_stream.h and ow_stream_wire.h.
 *
 * Two routes behind one API: a transport-bound fast path (dev->stream), and
 * a text fallback over MAIN's h\a\w / h\a\p / h\a\c commands for clients with
 * no push channel (PC host, CM0). The fallback calls the generated command
 * functions, so a menu move is a compile error rather than a wire mismatch. */
#include "onewili_stream.h"

#include <string.h>

void ow_stream_bind(ow_device* dev, const ow_stream_ops* ops)
{
    if (dev) dev->stream = ops;
}

static int ow__peer_ok(uint32_t p) { return p < OW_STREAM_PEER_COUNT; }

ow_status ow_stream_write(ow_device* dev, ow_peer dst, const void* buf, uint32_t len)
{
    if (!dev || !buf || len == 0 || len > OW_STREAM_MTU || !ow__peer_ok((uint32_t)dst))
        return OW_ERR_ARG;
    if (dev->stream && dev->stream->write)
        return dev->stream->write(dev->stream->ctx, (uint8_t)dst, (const uint8_t*)buf, len);

    /* Text route. MAIN reports whether it delivered (or queued) the datagram;
     * one it dropped is counted on MAIN and shows up in ow_stream_drops, the
     * same as on a push link, so the result here is OK either way. */
    bool delivered = false;
    return ow_hardware_system_stream_write(dev, (int32_t)dst, (const uint8_t*)buf,
                                           (size_t)len, &delivered);
}

int ow_stream_poll(ow_device* dev, ow_peer* src, void* buf, uint32_t cap)
{
    if (!dev || !buf) return -(int)OW_ERR_ARG;
    if (dev->stream && dev->stream->poll) {
        uint8_t s = 0;
        int n = dev->stream->poll(dev->stream->ctx, &s, (uint8_t*)buf, cap);
        if (n > 0 && src) *src = (ow_peer)s;
        return n;
    }

    /* Text route: h\a\p hands back up to OW_STREAM_STASH bytes of packed
     * [src u8][len u8][data] records in one round trip (CM0 executes one
     * command per MAIN loop pass, so one datagram per trip would be slow);
     * serve them from the per-device stash. */
    if (dev->stream_stash_pos >= dev->stream_stash_len) {
        int32_t frames = 0, queued = 0, dropped = 0;
        size_t n = 0;
        dev->stream_stash_len = dev->stream_stash_pos = 0;
        ow_status r = ow_hardware_system_stream_poll(dev, (int32_t)OW_STREAM_STASH,
                                                     &frames, &queued, &dropped,
                                                     dev->stream_stash,
                                                     sizeof dev->stream_stash, &n);
        if (r != OW_OK) return -(int)r;
        (void)queued; (void)dropped;
        if (frames <= 0 || n == 0) return 0;
        dev->stream_stash_len = (uint16_t)n;
    }

    uint32_t pos = dev->stream_stash_pos;
    uint32_t end = dev->stream_stash_len;
    if (end - pos < 2u) {   /* a cut record header: its datagram is gone */
        dev->stream_stash_pos = dev->stream_stash_len;
        dev->stream_local_drops++;
        return -(int)OW_ERR_PROTOCOL;
    }
    uint8_t s = dev->stream_stash[pos];
    uint32_t len = dev->stream_stash[pos + 1];
    if (len == 0 || len > OW_STREAM_MTU || end - pos - 2u < len || !ow__peer_ok(s)) {
        dev->stream_stash_pos = dev->stream_stash_len;   /* resync: drop the rest */
        dev->stream_local_drops++;
        return -(int)OW_ERR_PROTOCOL;
    }
    dev->stream_stash_pos = (uint16_t)(pos + 2u + len);
    if (len > cap) { dev->stream_local_drops++; return -(int)OW_ERR_BUFFER; }
    memcpy(buf, &dev->stream_stash[pos + 2u], len);
    if (src) *src = (ow_peer)s;
    return (int)len;
}

uint32_t ow_stream_drops(ow_device* dev)
{
    if (!dev) return 0;
    if (dev->stream && dev->stream->drops)
        return dev->stream->drops(dev->stream->ctx);
    int32_t mtu = 0, queued = 0, to = 0, from = 0;
    if (ow_hardware_system_stream_status(dev, &mtu, &queued, &to, &from) != OW_OK)
        return dev->stream_local_drops;
    return dev->stream_local_drops + (uint32_t)to + (uint32_t)from;
}

/* ---- push-link client protocol ------------------------------------------ */

void ow_stream_link_init(ow_stream_link* l, uint8_t self,
                         int (*send)(void*, const uint8_t*, uint32_t),
                         uint32_t (*now_ms)(void*), void* ctx)
{
    memset(l, 0, sizeof *l);
    l->self = self;
    l->send = send;
    l->now_ms = now_ms;
    l->ctx = ctx;
}

void ow_stream_link_reset(ow_stream_link* l)
{
    /* Fold what MAIN reported so far into the carry, then wait for a fresh
     * CREDIT to realign. */
    l->drop_accum += (l->from_last - l->from_base) + (l->to_last - l->to_base);
    l->from_base = l->from_last = l->to_base = l->to_last = 0;
    l->confirmed = 0;
    l->hello_sent = 0;
}

static void ow__link_hello(ow_stream_link* l, uint32_t now)
{
    uint8_t p[OW_STREAM_HDR + 1] = { (uint8_t)OW_STREAM_CTL_HELLO, l->self,
                                     (uint8_t)OW_STREAM_VERSION };
    l->last_hello_ms = now;
    l->hello_sent = 1;
    (void)l->send(l->ctx, p, sizeof p);
}

void ow_stream_link_service(ow_stream_link* l)
{
    uint32_t now = l->now_ms(l->ctx);
    uint32_t every = l->confirmed ? OW_STREAM_KEEPALIVE_MS : OW_STREAM_HELLO_RETRY_MS;
    if (l->credit_fresh) {
        l->credit_fresh = 0;
        if (l->confirmed && l->sent_total != l->consumed &&
            (uint32_t)(now - l->last_tx_ms) >= OW_STREAM_EXPIRE_MS) {
            /* MAIN takes frames off the link in order, within one of its
             * loop passes, and sends a CREDIT after each. If the newest
             * CREDIT still does not cover everything sent this long after
             * the last data frame, those frames died on the way (a lapped
             * receive ring, a failed checksum) and will never be counted;
             * left alone, every such loss would shrink the window for good
             * until writes stopped. Judged here rather than per CREDIT: the
             * transports buffer CREDITs, and an older one processed late
             * would report losses that never happened. The byte count only
             * bounds how many datagrams that was, so count the fewest.
             * A client that never pauses for OW_STREAM_EXPIRE_MS keeps the
             * loss until the shrunken window refuses writes long enough. */
            uint32_t lost = l->sent_total - l->consumed;
            l->tx_lost += (lost + OW_STREAM_WIRE_BYTES(OW_STREAM_MTU) - 1u)
                        / OW_STREAM_WIRE_BYTES(OW_STREAM_MTU);
            l->sent_total = l->consumed;
        }
    }
    if (!l->hello_sent || (uint32_t)(now - l->last_hello_ms) >= every)
        ow__link_hello(l, now);
}

void ow_stream_link_control(ow_stream_link* l, const uint8_t* p, uint32_t len)
{
    if (len < OW_STREAM_HDR || p[0] != OW_STREAM_CTL_CREDIT) return;   /* others: ignore */
    if (len < OW_STREAM_HDR + OW_STREAM_CREDIT_LEN) return;
    uint32_t consumed = ow_stream_get_u32(p + 2);
    uint32_t from     = ow_stream_get_u32(p + 6);
    uint32_t to       = ow_stream_get_u32(p + 10);
    l->credits++;
    if (l->confirmed && (int32_t)(consumed - l->consumed) >= 0
                     && (int32_t)(from - l->from_last) >= 0
                     && (int32_t)(to - l->to_last) >= 0) {
        l->consumed = consumed;
        l->from_last = from;
        l->to_last = to;
    } else {
        /* First CREDIT, or MAIN's totals went backwards (MAIN restarted):
         * align to MAIN's view. Nothing sent before this point can still be
         * in MAIN's receive ring in a way that matters to the window. */
        if (l->confirmed) ow_stream_link_reset(l);
        l->confirmed = 1;
        l->hello_sent = 1;
        l->consumed = consumed;
        l->sent_total = consumed;
        l->from_base = l->from_last = from;
        l->to_base = l->to_last = to;
    }
    if ((int32_t)(l->sent_total - l->consumed) < 0)
        l->sent_total = l->consumed;   /* written off, then turned up after all */
    l->credit_fresh = 1;               /* ow_stream_link_service() judges losses */
}

ow_status ow_stream_link_write(ow_stream_link* l, uint8_t dst, const uint8_t* data, uint32_t len)
{
    if (len == 0 || len > OW_STREAM_MTU || !ow__peer_ok(dst)) return OW_ERR_ARG;
    ow_stream_link_service(l);
    if (!l->confirmed) { l->tx_refused++; return OW_ERR_BUFFER; }
    uint32_t in_flight = l->sent_total - l->consumed;
    if (in_flight + OW_STREAM_WIRE_BYTES(len) > OW_STREAM_WINDOW) { l->tx_refused++; return OW_ERR_BUFFER; }
    uint8_t p[OW_STREAM_HDR + OW_STREAM_MTU];
    p[0] = dst;
    p[1] = l->self;
    memcpy(p + OW_STREAM_HDR, data, len);
    if (l->send(l->ctx, p, OW_STREAM_HDR + len) < 0) { l->tx_refused++; return OW_ERR_IO; }
    l->sent_total += OW_STREAM_WIRE_BYTES(len);
    l->last_tx_ms = l->now_ms(l->ctx);
    l->tx_frames++;
    return OW_OK;
}

uint32_t ow_stream_link_drops(const ow_stream_link* l)
{
    return l->tx_refused + l->tx_lost + l->drop_accum
         + (l->from_last - l->from_base) + (l->to_last - l->to_base);
}
