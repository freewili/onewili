/* Peer streams (onewili_stream.h) without hardware: the push-link client
 * protocol against a scripted MAIN, the ow_stream_bind fast path, and the
 * text route over a fake MAIN that speaks h\a\w / h\a\p / h\a\c.
 * c/, cm0/ and wilibsp/ ship the same onewili_stream.c, so this covers all
 * three. */
#include "onewili_stream.h"
#ifdef NDEBUG
#undef NDEBUG /* assertions perform the test calls in Release configurations too */
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- push-link protocol ------------------------------------------------ */

typedef struct link_io {
    uint8_t  last[OW_STREAM_HDR + OW_STREAM_MTU];
    uint32_t last_len, frames, hellos;
    uint32_t now;
    int      fail;
} link_io;

static int link_send(void* ctx, const uint8_t* p, uint32_t len) {
    link_io* io = (link_io*)ctx;
    if (io->fail) return -1;
    assert(len <= sizeof io->last);
    memcpy(io->last, p, len);
    io->last_len = len;
    io->frames++;
    if (p[0] == OW_STREAM_CTL_HELLO) io->hellos++;
    return 0;
}
static uint32_t link_now(void* ctx) { return ((link_io*)ctx)->now; }

static void credit(ow_stream_link* l, uint32_t consumed, uint32_t from, uint32_t to) {
    uint8_t p[OW_STREAM_HDR + OW_STREAM_CREDIT_LEN] = { OW_STREAM_CTL_CREDIT, OW_STREAM_PEER_MAIN };
    ow_stream_put_u32(p + 2, consumed);
    ow_stream_put_u32(p + 6, from);
    ow_stream_put_u32(p + 10, to);
    ow_stream_link_control(l, p, sizeof p);
}

static void test_link(void) {
    link_io io;
    ow_stream_link l;
    uint8_t data[OW_STREAM_MTU];
    const uint32_t big = OW_STREAM_WIRE_BYTES(OW_STREAM_MTU);
    uint32_t i, fit, drops;
    memset(&io, 0, sizeof io);
    memset(data, 0x5A, sizeof data);
    io.now = 1000;
    ow_stream_link_init(&l, OW_STREAM_PEER_DISPLAY, link_send, link_now, &io);

    /* Argument checks come before anything touches the link. */
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_ESP32, data, 0) == OW_ERR_ARG);
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_ESP32, data, OW_STREAM_MTU + 1) == OW_ERR_ARG);
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_COUNT, data, 1) == OW_ERR_ARG);
    assert(io.frames == 0 && l.tx_refused == 0);

    /* Unconfirmed: the write is refused and counted, and HELLO goes out. */
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_ESP32, data, 4) == OW_ERR_BUFFER);
    assert(l.tx_refused == 1 && ow_stream_link_drops(&l) == 1);
    assert(io.hellos == 1 && io.last_len == OW_STREAM_HDR + 1);
    assert(io.last[1] == OW_STREAM_PEER_DISPLAY && io.last[2] == OW_STREAM_VERSION);

    /* HELLO retries every OW_STREAM_HELLO_RETRY_MS until a CREDIT arrives. */
    io.now += OW_STREAM_HELLO_RETRY_MS - 1; ow_stream_link_service(&l);
    assert(io.hellos == 1);
    io.now += 1; ow_stream_link_service(&l);
    assert(io.hellos == 2);

    /* Short or foreign control frames are ignored. */
    { uint8_t p[OW_STREAM_HDR + 4] = { OW_STREAM_CTL_CREDIT, 0 };
      ow_stream_link_control(&l, p, sizeof p); }
    { uint8_t p[OW_STREAM_HDR + OW_STREAM_CREDIT_LEN] = { OW_STREAM_CTL_HELLO, 0 };
      ow_stream_link_control(&l, p, sizeof p); }
    assert(!l.confirmed && l.credits == 0);

    /* The first CREDIT confirms the link and aligns to MAIN's totals: drops
     * MAIN counted before this client arrived are not this client's. */
    credit(&l, 5000, 9, 11);
    assert(l.confirmed && l.credits == 1);
    assert(ow_stream_link_drops(&l) == 1);

    /* Once confirmed, HELLO becomes a keepalive every OW_STREAM_KEEPALIVE_MS. */
    io.now += OW_STREAM_HELLO_RETRY_MS; ow_stream_link_service(&l);
    assert(io.hellos == 2);
    io.now += OW_STREAM_KEEPALIVE_MS; ow_stream_link_service(&l);
    assert(io.hellos == 3);

    /* A datagram is one frame: dst, src, data. */
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_ESP32, (const uint8_t*)"hey", 3) == OW_OK);
    assert(io.last_len == OW_STREAM_HDR + 3 && io.last[0] == OW_STREAM_PEER_ESP32);
    assert(io.last[1] == OW_STREAM_PEER_DISPLAY && memcmp(io.last + 2, "hey", 3) == 0);
    assert(l.sent_total - l.consumed == OW_STREAM_WIRE_BYTES(3));
    credit(&l, 5000 + OW_STREAM_WIRE_BYTES(3), 9, 11);
    assert(l.sent_total == l.consumed);

    /* The window counts wire bytes: max-size datagrams fill it, and the one
     * that would overflow it is refused and counted, not sent. */
    fit = OW_STREAM_WINDOW / big;
    for (i = 0; i < fit; ++i)
        assert(ow_stream_link_write(&l, OW_STREAM_PEER_CM0, data, OW_STREAM_MTU) == OW_OK);
    drops = ow_stream_link_drops(&l);
    { uint32_t before = io.frames;
      assert(ow_stream_link_write(&l, OW_STREAM_PEER_CM0, data, OW_STREAM_MTU) == OW_ERR_BUFFER);
      assert(io.frames == before); }
    assert(ow_stream_link_drops(&l) == drops + 1);

    /* A CREDIT covering them reopens the window. */
    credit(&l, l.sent_total, 9, 11);
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_CM0, data, OW_STREAM_MTU) == OW_OK);
    credit(&l, l.sent_total, 9, 11);

    /* MAIN's drop counters, both directions, add to this client's drops. */
    drops = ow_stream_link_drops(&l);
    credit(&l, l.consumed, 9 + 2, 11 + 3);
    assert(ow_stream_link_drops(&l) == drops + 5);

    /* A send failure is refused and counted, and uses no window. */
    drops = ow_stream_link_drops(&l);
    io.fail = 1;
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_CM0, data, 1) == OW_ERR_IO);
    io.fail = 0;
    assert(ow_stream_link_drops(&l) == drops + 1 && l.sent_total == l.consumed);

    /* Frames MAIN never counts: not written off while the last one is recent,
     * then written off (as the fewest datagrams) once a CREDIT still misses
     * them OW_STREAM_EXPIRE_MS after the last data frame. */
    drops = ow_stream_link_drops(&l);
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_CM0, data, 10) == OW_OK);
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_CM0, data, 10) == OW_OK);
    credit(&l, l.consumed, 11, 14);
    io.now += OW_STREAM_EXPIRE_MS - 1; ow_stream_link_service(&l);
    assert(l.tx_lost == 0 && l.sent_total != l.consumed);
    credit(&l, l.consumed, 11, 14);
    io.now += 1; ow_stream_link_service(&l);
    assert(l.tx_lost == 1 && l.sent_total == l.consumed);
    assert(ow_stream_link_drops(&l) == drops + 1);
    /* Judged once per CREDIT, not on every service pass. */
    io.now += OW_STREAM_EXPIRE_MS; ow_stream_link_service(&l);
    assert(l.tx_lost == 1);

    /* MAIN restarted (its totals went backwards): realign without losing or
     * double-counting the drops reported so far. */
    drops = ow_stream_link_drops(&l);
    credit(&l, 40, 1, 0);
    assert(l.confirmed && l.consumed == 40 && l.sent_total == 40);
    assert(ow_stream_link_drops(&l) == drops);
    credit(&l, 40, 2, 1);
    assert(ow_stream_link_drops(&l) == drops + 2);

    /* An explicit reset keeps the counters and waits for a fresh CREDIT. */
    drops = ow_stream_link_drops(&l);
    ow_stream_link_reset(&l);
    assert(!l.confirmed && ow_stream_link_drops(&l) == drops);
    assert(ow_stream_link_write(&l, OW_STREAM_PEER_CM0, data, 1) == OW_ERR_BUFFER);
    assert(ow_stream_link_drops(&l) == drops + 1);
}

/* ---- bound fast path --------------------------------------------------- */

typedef struct fake_ops { uint8_t dst; uint32_t len, writes, polls; } fake_ops;

static ow_status ops_write(void* ctx, uint8_t dst, const uint8_t* data, uint32_t len) {
    fake_ops* f = (fake_ops*)ctx;
    (void)data;
    f->dst = dst; f->len = len; f->writes++;
    return OW_OK;
}
static int ops_poll(void* ctx, uint8_t* src, uint8_t* buf, uint32_t cap) {
    fake_ops* f = (fake_ops*)ctx;
    assert(cap >= 2);
    f->polls++;
    *src = OW_STREAM_PEER_ESP32;
    buf[0] = 'o'; buf[1] = 'k';
    return 2;
}
static uint32_t ops_drops(void* ctx) { (void)ctx; return 42; }

/* ---- fake MAIN on the text route --------------------------------------- */

typedef struct rec { uint8_t src, len; uint8_t data[OW_STREAM_MTU]; } rec;
typedef struct fake_main {
    rec      q[16];
    unsigned qn;
    int32_t  dropped_to, dropped_from;
    unsigned polls, status_calls;
    int      corrupt;            /* next poll reply carries a bad record */
    char     out[8192];
    size_t   out_len, out_pos;
} fake_main;

static void reply(fake_main* m, const char* path, const char* body) {
    int n = snprintf(m->out + m->out_len, sizeof m->out - m->out_len,
                     "[%s 0 0 %s 1]\r\n", path, body);
    assert(n > 0 && (size_t)n < sizeof m->out - m->out_len);
    m->out_len += (size_t)n;
}

static int main_write(void* ctx, const uint8_t* data, size_t len) {
    fake_main* m = (fake_main*)ctx;
    char line[1100];
    char body[4096];
    size_t bl = 0;
    assert(len > 0 && data[0] == 0x02);
    if (len <= 2) return (int)len;                      /* ow_open's reset */
    assert(len - 1 < sizeof line && data[len - 1] == '\n');
    memcpy(line, data + 1, len - 2);
    line[len - 2] = 0;
    if (strncmp(line, "h\\a\\w ", 6) == 0) {
        char* cur = line + 6;
        long dst = strtol(cur, &cur, 10);
        rec r;
        r.src = OW_STREAM_PEER_HOST;
        r.len = 0;
        while (*cur) r.data[r.len++] = (uint8_t)strtoul(cur, &cur, 16);
        assert(r.len >= 1 && r.len <= OW_STREAM_MTU);
        if (dst == OW_STREAM_PEER_HOST && m->qn < 16) {
            m->q[m->qn++] = r;
            reply(m, "h\\a\\w", "1");
        } else {
            m->dropped_from++;
            reply(m, "h\\a\\w", "0");
        }
    } else if (strncmp(line, "h\\a\\p ", 6) == 0) {
        long max = strtol(line + 6, NULL, 10);
        unsigned popped = 0, k;
        size_t used = 0;
        char recs[4096];
        size_t rl = 0;
        m->polls++;
        recs[0] = 0;
        while (popped < m->qn && used + 2u + m->q[popped].len <= (size_t)max) {
            const rec* r = &m->q[popped];
            rl += (size_t)snprintf(recs + rl, sizeof recs - rl, " %02X %02X", r->src, r->len);
            for (k = 0; k < r->len; ++k)
                rl += (size_t)snprintf(recs + rl, sizeof recs - rl, " %02X", r->data[k]);
            used += 2u + r->len;
            popped++;
        }
        if (m->corrupt) {   /* a record claiming more bytes than follow it */
            m->corrupt = 0;
            rl += (size_t)snprintf(recs + rl, sizeof recs - rl, " 04 7F 01");
            if (!popped) popped = 1;
        }
        memmove(m->q, m->q + (popped < m->qn ? popped : m->qn),
                (m->qn - (popped < m->qn ? popped : m->qn)) * sizeof(rec));
        m->qn -= popped < m->qn ? popped : m->qn;
        bl = (size_t)snprintf(body, sizeof body, "%u %u %d%s", popped, m->qn,
                              (int)m->dropped_to, recs);
        assert(bl < sizeof body);
        reply(m, "h\\a\\p", body);
    } else if (strcmp(line, "h\\a\\c") == 0) {
        m->status_calls++;
        snprintf(body, sizeof body, "%u %u %d %d", OW_STREAM_MTU, m->qn,
                 (int)m->dropped_to, (int)m->dropped_from);
        reply(m, "h\\a\\c", body);
    } else {
        reply(m, line, "unknown");
    }
    return (int)len;
}

static int main_read(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms) {
    fake_main* m = (fake_main*)ctx;
    size_t n = m->out_len - m->out_pos;
    (void)timeout_ms;
    if (n > cap) n = cap;
    memcpy(buf, m->out + m->out_pos, n);
    m->out_pos += n;
    if (m->out_pos == m->out_len) m->out_pos = m->out_len = 0;
    return (int)n;
}

static ow_device dev;   /* large: keep it off the stack */

static void test_bound_and_text(void) {
    fake_main m;
    fake_ops f;
    ow_stream_ops ops = { &f, ops_write, ops_poll, ops_drops };
    ow_transport t = { &m, main_write, main_read };
    uint8_t buf[OW_STREAM_MTU];
    uint8_t big[OW_STREAM_MTU];
    ow_peer src = OW_PEER_MAIN;
    int n;
    memset(&m, 0, sizeof m);
    memset(&f, 0, sizeof f);
    memset(big, 0xC3, sizeof big);
    assert(ow_open(&dev, &t) == OW_OK);

    /* Common argument checks, whichever route. */
    assert(ow_stream_write(&dev, OW_PEER_ESP32, buf, 0) == OW_ERR_ARG);
    assert(ow_stream_write(&dev, OW_PEER_ESP32, buf, OW_STREAM_MTU + 1) == OW_ERR_ARG);
    assert(ow_stream_write(&dev, (ow_peer)OW_STREAM_PEER_COUNT, buf, 1) == OW_ERR_ARG);
    assert(ow_stream_write(NULL, OW_PEER_ESP32, buf, 1) == OW_ERR_ARG);
    assert(ow_stream_poll(&dev, &src, NULL, sizeof buf) == -(int)OW_ERR_ARG);

    /* Bound: every call goes to the transport's ops, nothing to MAIN's text. */
    ow_stream_bind(&dev, &ops);
    assert(ow_stream_write(&dev, OW_PEER_CM0, "abc", 3) == OW_OK);
    assert(f.writes == 1 && f.dst == OW_PEER_CM0 && f.len == 3);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == 2);
    assert(src == OW_PEER_ESP32 && buf[0] == 'o' && buf[1] == 'k');
    assert(ow_stream_drops(&dev) == 42);
    assert(m.polls == 0 && m.status_calls == 0 && m.qn == 0);

    /* ow_open clears the binding: back on the text route. */
    assert(ow_open(&dev, &t) == OW_OK);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == 0);   /* nothing waiting */
    assert(m.polls == 1);

    /* Loopback to self; one h\a\p serves the whole batch, in order. */
    assert(ow_stream_write(&dev, OW_PEER_HOST, "one", 3) == OW_OK);
    assert(ow_stream_write(&dev, OW_PEER_HOST, "two", 3) == OW_OK);
    assert(ow_stream_write(&dev, OW_PEER_HOST, big, OW_STREAM_MTU) == OW_OK);
    assert(m.qn == 3);
    m.polls = 0;
    n = ow_stream_poll(&dev, &src, buf, sizeof buf);
    assert(n == 3 && src == OW_PEER_HOST && memcmp(buf, "one", 3) == 0);
    n = ow_stream_poll(&dev, &src, buf, sizeof buf);
    assert(n == 3 && memcmp(buf, "two", 3) == 0);
    n = ow_stream_poll(&dev, &src, buf, sizeof buf);
    assert(n == (int)OW_STREAM_MTU && memcmp(buf, big, OW_STREAM_MTU) == 0);
    assert(m.polls == 1);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == 0);
    assert(m.polls == 2);

    /* A batch holds whole records only, up to OW_STREAM_STASH bytes: three
     * max-size datagrams (3 x 130) take two round trips. */
    assert(ow_stream_write(&dev, OW_PEER_HOST, big, OW_STREAM_MTU) == OW_OK);
    assert(ow_stream_write(&dev, OW_PEER_HOST, big, OW_STREAM_MTU) == OW_OK);
    assert(ow_stream_write(&dev, OW_PEER_HOST, big, OW_STREAM_MTU) == OW_OK);
    m.polls = 0;
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == (int)OW_STREAM_MTU);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == (int)OW_STREAM_MTU);
    assert(m.polls == 1 && m.qn == 1);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == (int)OW_STREAM_MTU);
    assert(m.polls == 2 && m.qn == 0);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == 0);

    /* MAIN dropping a datagram is not an error for the writer; it is a drop. */
    assert(ow_stream_drops(&dev) == 0);
    assert(ow_stream_write(&dev, OW_PEER_MAIN, "x", 1) == OW_OK);
    assert(ow_stream_drops(&dev) == 1);
    m.dropped_to = 2;
    assert(ow_stream_drops(&dev) == 3);

    /* A buffer smaller than the datagram: dropped, counted, reported. */
    assert(ow_stream_write(&dev, OW_PEER_HOST, "longer", 6) == OW_OK);
    assert(ow_stream_poll(&dev, &src, buf, 4) == -(int)OW_ERR_BUFFER);
    assert(ow_stream_drops(&dev) == 4);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == 0);

    /* A corrupt record: the rest of the batch is dropped and one loss counted. */
    assert(ow_stream_write(&dev, OW_PEER_HOST, "ok", 2) == OW_OK);
    m.corrupt = 1;
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == 2 && memcmp(buf, "ok", 2) == 0);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == -(int)OW_ERR_PROTOCOL);
    assert(ow_stream_drops(&dev) == 5);
    assert(ow_stream_poll(&dev, &src, buf, sizeof buf) == 0);
}

int main(void) {
    test_link();
    test_bound_and_text();
    puts("stream: ok");
    return 0;
}
