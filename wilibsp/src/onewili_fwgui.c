#include "onewili_fwgui.h"
#include <string.h>
#include "hardware/uart.h"
#include "hardware/gpio.h"
#ifndef OWFW_NO_IRQ
#include "hardware/irq.h"
#include "hardware/structs/uart.h"
#endif
#include "pico/time.h"
#include "onewili_sd.h"
#include "onewili_stream.h"
#include "ow_sdfs_frame.h"
#include "sdfs_wire.h"

/* ── Link constants (FwGUI protocol; see the firmware's protocol.md) ───── */
#define OWFW_BAUD          8000000
#define OWFW_PIN_TX        1
#define OWFW_PIN_RX        0
#define OWFW_PIN_CTS       2
#define OWFW_PIN_RTS       3
#define OWFW_EVT_SYNC0     0xB0    /* display->main event frames  */
#define OWFW_EVT_SYNC1     0x1D
#define OWFW_CMD_SYNC0     0xBE    /* main->display command frames */
#define OWFW_CMD_SYNC1     0xBA
#define OWFW_EVT_TERM      0x18    /* FWGUI_EVENT_M_TERM_INPUT (24) */
#define OWFW_EVT_POWER_ZONES 0x30  /* FWGUI_EVENT_POWER_ZONES (48)  */
#define OWFW_MARKER        0x01    /* OneWili chunk marker          */
#define OWFW_CHUNK_MAX     56      /* text bytes per event frame    */
#define OWFW_CMD_RESPONSE  0x5D    /* FWGUI_API_ONEWILL_RESPONSE    */
#define OWFW_CMD_BINARY    0x5E    /* FWGUI_API_ONEWILL_BINARY      */
#define OWFW_CMD_SDFS      0x5F    /* FWGUI_API_SDFS_DATA           */
#define OWFW_CMD_STREAM    0xF1    /* FWGUI_API_ONEWILL_STREAM      */
#define OWFW_EVT_STREAM    0xF1    /* FWGUI_EVENT_ONEWILI_STREAM (241) */
#define OWFW_SDFS_SLOTS    8       /* SDFS RX frames buffered       */
#define OWFW_SDFS_POLL_US  1000    /* an idle recv poll costs ~1 ms */
#define OWFW_STREAM_RING   2048    /* peer-stream datagram FIFO (bytes) */
#define OWFW_FRAME_MAX     512     /* incoming command frame payload cap */
#define OWFW_EVT_OVERHEAD  7       /* event frame: sync(2) + len(2) + code(1) + cksum(2) */

/* Receive path sizing.
 *
 * The link runs at 8 Mbaud (~800 KB/s) and MAIN's transmit is a blocking
 * FIFO write gated by hardware flow control. The UART's 32-byte RX FIFO is
 * 40 us of wire time, so a display app that drains it only from inside
 * ow_poll_* calls stalls MAIN on CTS every time it draws a line of text --
 * and a stalled MAIN main loop drops CAN frames. The UART RX interrupt below
 * moves bytes into a 32 KB ring (40 ms of wire time) regardless of what the
 * app is doing; the frame parser and the per-stream FIFOs run from the app's
 * context via owfw_pump(). The binary stream FIFO holds ~340 canRxReport
 * frames (96 bytes framed), the text FIFO ~35 64-byte CAN text events. */
#define OWFW_RING_BITS     15
#define OWFW_RING_SIZE     (1u << OWFW_RING_BITS)
#define OWFW_RING_MASK     (OWFW_RING_SIZE - 1u)
#define OWFW_TEXT_MAX      8192
#define OWFW_BINARY_MAX    32768

/* ── Per-stream byte FIFO ──────────────────────────────────────────────── */
typedef struct {
    uint8_t* buf;
    uint32_t size, head, count;
    uint32_t* max_fill;
} owfw_fifo;

static ow_fwgui_stats g_stats;
static uint8_t  g_text_buf[OWFW_TEXT_MAX];
static uint8_t  g_binary_buf[OWFW_BINARY_MAX];
static owfw_fifo g_text   = { g_text_buf,   OWFW_TEXT_MAX,   0, 0, &g_stats.text_max_fill };   /* 0x5D */
static owfw_fifo g_binary = { g_binary_buf, OWFW_BINARY_MAX, 0, 0, &g_stats.binary_max_fill }; /* 0x5E */

static void fifo_push_frame(owfw_fifo* f, const uint8_t* p, uint32_t n) {
    uint32_t w, first;
    if (n > f->size - f->count) { g_stats.dropped_frames++; return; }  /* drop-newest, whole frame */
    w = (f->head + f->count) % f->size;
    first = f->size - w;
    if (first > n) first = n;
    memcpy(f->buf + w, p, first);
    if (n > first) memcpy(f->buf, p + first, n - first);
    f->count += n;
    if (f->count > *f->max_fill) *f->max_fill = f->count;
}

static uint32_t fifo_pop(owfw_fifo* f, uint8_t* out, uint32_t cap) {
    uint32_t n = f->count < cap ? f->count : cap;
    uint32_t first = f->size - f->head;
    if (first > n) first = n;
    memcpy(out, f->buf + f->head, first);
    if (n > first) memcpy(out + first, f->buf, n - first);
    f->head = (f->head + n) % f->size;
    f->count -= n;
    return n;
}

/* ── SDFS frame ring ───────────────────────────────────────────────────── */
/* SDFS responses (0x5F) are whole frames, not a byte stream, so they get a
 * ring of complete frames rather than a FIFO. Drop-newest on overflow, counted
 * with the other dropped frames. */
typedef struct {
    uint16_t len;
    uint8_t  buf[SDFS_MAX_FRAME];
} owfw_sdfs_slot;

static owfw_sdfs_slot g_sdfs[OWFW_SDFS_SLOTS];
static uint32_t       g_sdfs_head, g_sdfs_count;

static void sdfs_push(const uint8_t* p, uint16_t n) {
    owfw_sdfs_slot* s;
    if (n > SDFS_MAX_FRAME || g_sdfs_count >= OWFW_SDFS_SLOTS) { g_stats.dropped_frames++; return; }
    s = &g_sdfs[(g_sdfs_head + g_sdfs_count) % OWFW_SDFS_SLOTS];
    memcpy(s->buf, p, n);
    s->len = n;
    g_sdfs_count++;
}

/* ── Peer-stream datagram FIFO ─────────────────────────────────────────── */
/* Datagrams (0xF1, see ow_stream_wire.h) keep their boundaries: each is one
 * [src][len][data] record in a byte FIFO, so small datagrams cost their size
 * rather than a whole MTU slot -- a sender's full credit window of 1-byte
 * datagrams (69) fits with room to spare. Drop-newest on overflow, counted
 * apart from dropped_frames: stream losses belong to ow_stream_drops, which
 * has one meaning on every target. Control frames (HELLO/CREDIT) are
 * hop-local and go straight to the shared push-link client instead. Filled
 * and drained only from owfw_pump's caller context, never from the IRQ. */
static uint8_t        g_stream[OWFW_STREAM_RING];
static uint32_t       g_stream_head, g_stream_used, g_stream_count;
static ow_stream_link g_link;

static void stream_put(const uint8_t* p, uint32_t n) {
    uint32_t tail = (g_stream_head + g_stream_used) % OWFW_STREAM_RING;
    uint32_t i;
    for (i = 0; i < n; i++) {
        g_stream[tail] = p[i];
        if (++tail == OWFW_STREAM_RING) tail = 0;
    }
    g_stream_used += n;
}

static void stream_take(uint8_t* out, uint32_t n) {
    uint32_t i;
    for (i = 0; i < n; i++) {
        if (out) out[i] = g_stream[g_stream_head];
        if (++g_stream_head == OWFW_STREAM_RING) g_stream_head = 0;
    }
    g_stream_used -= n;
}

static void stream_rx(const uint8_t* p, uint16_t n) {
    uint8_t hdr[2];
    if (n >= 1 && p[0] >= OW_STREAM_CTL_FIRST) {
        if (p[0] == OW_STREAM_CTL_CREDIT && n < OW_STREAM_HDR + OW_STREAM_CREDIT_LEN)
            g_stats.stream_malformed++;
        ow_stream_link_control(&g_link, p, n);
        return;
    }
    /* MAIN only ever routes us datagrams addressed to us, stamped with the
     * real sender, 1..MTU bytes long; anything else passed the checksum but
     * is not something we can hand to ow_stream_poll. */
    if (n < OW_STREAM_HDR + 1u || n > OW_STREAM_HDR + OW_STREAM_MTU ||
        p[0] != OW_STREAM_PEER_DISPLAY || p[1] >= OW_STREAM_PEER_COUNT) {
        g_stats.stream_malformed++;
        return;
    }
    if (OWFW_STREAM_RING - g_stream_used < 2u + (n - OW_STREAM_HDR)) { g_stats.stream_dropped++; return; }
    hdr[0] = p[1];
    hdr[1] = (uint8_t)(n - OW_STREAM_HDR);
    stream_put(hdr, 2);
    stream_put(p + OW_STREAM_HDR, hdr[1]);
    g_stream_count++;
    if (g_stream_used > g_stats.stream_max_fill) g_stats.stream_max_fill = g_stream_used;
}

/* ── RX: BE BA command-frame parser ────────────────────────────────────── */
/* frame: BE BA | len u16le | cmd u8 | payload[len] | cksum u16le
 * checksum = 16-bit additive sum of sync(2)+length(2)+cmd(1)+payload. */
typedef enum { RX_SYNC0, RX_SYNC1, RX_LEN0, RX_LEN1, RX_CMD, RX_PAYLOAD, RX_CK0, RX_CK1 } owfw_rx_state;

static struct {
    owfw_rx_state st;
    uint16_t len, got, sum, ck;
    uint8_t  cmd;
    uint8_t  payload[OWFW_FRAME_MAX];
} g_rx;

static void rx_byte(uint8_t b) {
    switch (g_rx.st) {
    case RX_SYNC0:
        if (b == OWFW_CMD_SYNC0) { g_rx.sum = b; g_rx.st = RX_SYNC1; }
        break;
    case RX_SYNC1:
        if (b == OWFW_CMD_SYNC1) { g_rx.sum += b; g_rx.st = RX_LEN0; }
        else g_rx.st = (b == OWFW_CMD_SYNC0) ? RX_SYNC1 : RX_SYNC0;
        break;
    case RX_LEN0: g_rx.sum += b; g_rx.len = b;               g_rx.st = RX_LEN1; break;
    case RX_LEN1:
        g_rx.len |= (uint16_t)(b << 8);
        if (g_rx.len > OWFW_FRAME_MAX) {
            /* Nothing MAIN sends is this long: this is a false sync or a
             * corrupt length, which would otherwise swallow up to 64 KB of
             * good frames as payload. Resync now, rescanning the two length
             * bytes -- they may be the start of the real next frame. */
            uint8_t lo = (uint8_t)(g_rx.len & 0xFF);
            g_stats.length_errors++;
            g_rx.st = RX_SYNC0;
            rx_byte(lo);
            rx_byte(b);
            break;
        }
        g_rx.sum += b;
        g_rx.got = 0;
        g_rx.st = RX_CMD;
        break;
    case RX_CMD:
        g_rx.sum += b; g_rx.cmd = b;
        g_rx.st = g_rx.len ? RX_PAYLOAD : RX_CK0;
        break;
    case RX_PAYLOAD:
        g_rx.sum += b;
        g_rx.payload[g_rx.got] = b;
        if (++g_rx.got >= g_rx.len) g_rx.st = RX_CK0;
        break;
    case RX_CK0: g_rx.ck = b; g_rx.st = RX_CK1; break;
    case RX_CK1:
        g_rx.ck |= (uint16_t)(b << 8);
        if (g_rx.ck == g_rx.sum) {
            if (g_rx.cmd == OWFW_CMD_RESPONSE) { g_stats.frames_text++;   fifo_push_frame(&g_text, g_rx.payload, g_rx.len); }
            else if (g_rx.cmd == OWFW_CMD_BINARY) { g_stats.frames_binary++; fifo_push_frame(&g_binary, g_rx.payload, g_rx.len); }
            else if (g_rx.cmd == OWFW_CMD_SDFS) { g_stats.frames_sdfs++;   sdfs_push(g_rx.payload, g_rx.len); }
            else if (g_rx.cmd == OWFW_CMD_STREAM) { g_stats.frames_stream++; stream_rx(g_rx.payload, g_rx.len); }
            else g_stats.frames_other++;     /* every other command code (GUI traffic) is discarded */
        } else {
            g_stats.checksum_errors++;
        }
        g_rx.st = RX_SYNC0;
        break;
    }
}

/* ── UART RX interrupt -> ring ─────────────────────────────────────────── */
/* OWFW_NO_IRQ (host test builds): no interrupt controller, the pump polls the
 * UART with uart_is_readable/uart_getc into the same ring instead. */
static uint8_t           g_ring[OWFW_RING_SIZE];
static volatile uint32_t g_ring_head;     /* written by the IRQ only  */
static volatile uint32_t g_ring_tail;     /* written by the pump only */
static bool              g_irq_installed;

#ifdef OWFW_NO_IRQ
static void owfw_poll_uart(void) {
    uint32_t head = g_ring_head;
    uint32_t tail = g_ring_tail;
    while (uart_is_readable(uart0)) {
        uint8_t b = (uint8_t)uart_getc(uart0);
        if (head - tail >= OWFW_RING_SIZE) { g_stats.ring_overrun_bytes++; continue; }
        g_ring[head & OWFW_RING_MASK] = b;
        head++;
    }
    g_ring_head = head;
    if (head - tail > g_stats.ring_max_fill) g_stats.ring_max_fill = head - tail;
}
#else
static void owfw_uart_irq(void) {
    uart_hw_t* hw = uart_get_hw(uart0);
    uint32_t head = g_ring_head;
    uint32_t tail = g_ring_tail;
    if (hw->rsr & UART_UARTRSR_OE_BITS) {
        g_stats.hw_overruns++;
        hw->rsr = UART_UARTRSR_OE_BITS;      /* write-to-clear */
    }
    while (!(hw->fr & UART_UARTFR_RXFE_BITS)) {
        uint8_t b = (uint8_t)hw->dr;         /* the read also pops error flags */
        if (head - tail >= OWFW_RING_SIZE) { g_stats.ring_overrun_bytes++; continue; }
        g_ring[head & OWFW_RING_MASK] = b;
        head++;
    }
    g_ring_head = head;
    if (head - tail > g_stats.ring_max_fill) g_stats.ring_max_fill = head - tail;
}
#endif

static void owfw_pump(void) {
#ifdef OWFW_NO_IRQ
    owfw_poll_uart();
#endif
    uint32_t tail = g_ring_tail;
    uint32_t head = g_ring_head;
    while (tail != head) {
        rx_byte(g_ring[tail & OWFW_RING_MASK]);
        tail++;
    }
    g_ring_tail = tail;
}

void ow_fwgui_pump(void) { owfw_pump(); }

uint32_t ow_fwgui_ring_fill(void) { return g_ring_head - g_ring_tail; }

/* ── TX: wrap command bytes into marked M_TERM_INPUT event frames ──────── */
/* frame: B0 1D | len u16le | payload | cksum u16le, where payload =
 * event code + marker + count + text and len counts payload EXCLUDING the
 * event code (per protocol.md); checksum covers sync+length+payload. */
static void owfw_send_chunk(const uint8_t* text, uint8_t n) {
    uint8_t f[2 + 2 + 3 + OWFW_CHUNK_MAX + 2];
    uint16_t len = (uint16_t)(2 + n);            /* marker + count + text */
    uint32_t k = 0;
    f[k++] = OWFW_EVT_SYNC0; f[k++] = OWFW_EVT_SYNC1;
    f[k++] = (uint8_t)(len & 0xFF); f[k++] = (uint8_t)(len >> 8);
    f[k++] = OWFW_EVT_TERM;
    f[k++] = OWFW_MARKER;
    f[k++] = n;
    memcpy(&f[k], text, n); k += n;
    uint16_t sum = 0;
    for (uint32_t i = 0; i < k; i++) sum = (uint16_t)(sum + f[i]);
    f[k++] = (uint8_t)(sum & 0xFF); f[k++] = (uint8_t)(sum >> 8);
    uart_write_blocking(uart0, f, k);
    g_stats.tx_bytes += k;
}

/* Generic B0 1D event frame: sync | len u16le (excludes event code) |
 * event code | payload | cksum u16le (additive sum over every preceding
 * byte). Returns the frame size, or 0 if it would not fit in cap. */
static size_t owfw_frame_event(uint8_t* f, size_t cap, uint8_t event_code,
                               const uint8_t* payload, size_t n) {
    size_t k = 0, i;
    uint16_t sum = 0;
    if (n > 0xFFFFu || n + OWFW_EVT_OVERHEAD > cap) return 0;
    f[k++] = OWFW_EVT_SYNC0; f[k++] = OWFW_EVT_SYNC1;
    f[k++] = (uint8_t)(n & 0xFF); f[k++] = (uint8_t)(n >> 8);
    f[k++] = event_code;
    if (n) memcpy(&f[k], payload, n);
    k += n;
    for (i = 0; i < k; i++) sum = (uint16_t)(sum + f[i]);
    f[k++] = (uint8_t)(sum & 0xFF); f[k++] = (uint8_t)(sum >> 8);
    return k;
}

/* Small fire-and-forget events, like owfw_send_chunk -- no response is read.
 * An event too long for the stack buffer is not sent rather than overrunning
 * it. */
static void owfw_send_event(uint8_t event_code, const uint8_t* payload, uint8_t n) {
    uint8_t f[OWFW_EVT_OVERHEAD + 32];
    size_t k = owfw_frame_event(f, sizeof f, event_code, payload, n);
    if (k == 0) return;
    uart_write_blocking(uart0, f, k);
    g_stats.tx_bytes += (uint32_t)k;
}

/* One stream payload P as exactly one event 0xF1 frame -- the send hook of
 * the shared push-link client (ow_stream_link). Its own buffer, sized for
 * the largest P. */
static int owfw_stream_send(void* ctx, const uint8_t* p, uint32_t n) {
    uint8_t f[OWFW_EVT_OVERHEAD + OW_STREAM_HDR + OW_STREAM_MTU];
    size_t k;
    (void)ctx;
    if (n == 0) return -1;
    k = owfw_frame_event(f, sizeof f, OWFW_EVT_STREAM, p, n);
    if (k == 0) return -1;
    uart_write_blocking(uart0, f, k);
    g_stats.tx_bytes += (uint32_t)k;
    return 0;
}

static int owfw_write(void* ctx, const uint8_t* data, size_t len) {
    (void)ctx;
    size_t off = 0;
    while (off < len) {
        size_t n = len - off;
        if (n > OWFW_CHUNK_MAX) n = OWFW_CHUNK_MAX;
        owfw_send_chunk(data + off, (uint8_t)n);
        off += n;
    }
    return (int)len;
}

static int owfw_read_stream(owfw_fifo* f, uint8_t* buf, size_t cap, uint32_t timeout_ms) {
    absolute_time_t deadline = make_timeout_time_ms(timeout_ms);
    for (;;) {
        owfw_pump();
        if (f->count) return (int)fifo_pop(f, buf, (uint32_t)cap);
        if (timeout_ms == 0 || time_reached(deadline)) return 0;
    }
}

static int owfw_read_text(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms) {
    (void)ctx; return owfw_read_stream(&g_text, buf, cap, timeout_ms);
}
static int owfw_read_binary(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms) {
    (void)ctx; return owfw_read_stream(&g_binary, buf, cap, timeout_ms);
}

/* ── SDFS transport (sdfslib client <-> the display link) ───────────────── */
/* Request frames go out as B0 1D events (id 42), byte-identical to the stock
 * display firmware; responses arrive as BE BA / 0x5F command frames. */
static int owfw_sdfs_send(void* ctx, const uint8_t* frame, size_t len) {
    uint8_t f[SDFS_MAX_FRAME + OW_SDFS_EVENT_OVERHEAD];
    size_t n;
    (void)ctx;
    n = ow_sdfs_frame_event(f, sizeof f, frame, len);
    if (n == 0) return -1;
    uart_write_blocking(uart0, f, n);
    g_stats.tx_bytes += (uint32_t)n;
    return 0;
}

/* Non-blocking in the sdfslib sense: it returns 0 rather than a frame, but
 * paces an empty poll to ~1 ms so the client's timeout_polls budget IS a
 * millisecond budget (see onewili_sd.c:apply_timeout). Draining the shared
 * parser here is what keeps OneWili text/binary events flowing while an SD
 * call blocks. */
static int owfw_sdfs_recv(void* ctx, uint8_t* buf, size_t cap, size_t* len) {
    absolute_time_t deadline = make_timeout_time_us(OWFW_SDFS_POLL_US);
    (void)ctx;
    for (;;) {
        owfw_sdfs_slot* s;
        owfw_pump();
        if (g_sdfs_count) {
            s = &g_sdfs[g_sdfs_head];
            g_sdfs_head = (g_sdfs_head + 1) % OWFW_SDFS_SLOTS;
            g_sdfs_count--;
            if (s->len > cap) return -1;
            memcpy(buf, s->buf, s->len);
            *len = s->len;
            return 1;
        }
        if (time_reached(deadline)) return 0;
    }
}

sdfs_transport_t ow_fwgui_sdfs_transport(void) {
    sdfs_transport_t t;
    t.send = owfw_sdfs_send;
    t.recv = owfw_sdfs_recv;
    t.ctx  = 0;
    return t;
}

/* ── Peer streams (onewili_stream.h fast path) ─────────────────────────── */
/* The protocol (HELLO, keepalive, CREDIT window, MAIN's drop totals) is the
 * shared ow_stream_link; this layer only frames it onto the link, queues
 * received datagrams, and never waits: poll and drops only pump what the
 * IRQ already buffered. */
#ifdef OWFW_NO_IRQ
static uint32_t g_owfw_host_ms;   /* host builds have no pico timer: tests drive this */
static uint32_t owfw_now_ms(void* ctx) { (void)ctx; return g_owfw_host_ms; }
#else
static uint32_t owfw_now_ms(void* ctx) { (void)ctx; return to_ms_since_boot(get_absolute_time()); }
#endif

static ow_status owfw_stream_write(void* ctx, uint8_t dst, const uint8_t* data, uint32_t len) {
    (void)ctx;
    owfw_pump();                  /* a CREDIT already in the ring may reopen the window */
    return ow_stream_link_write(&g_link, dst, data, len);
}

static int owfw_stream_poll(void* ctx, uint8_t* src, uint8_t* buf, uint32_t cap) {
    uint8_t hdr[2];
    (void)ctx;
    owfw_pump();
    ow_stream_link_service(&g_link);   /* HELLO: announces us and keeps MAIN's latch open */
    if (!g_stream_count) return 0;
    stream_take(hdr, 2);
    g_stream_count--;
    if (hdr[1] > cap) { stream_take(0, hdr[1]); g_stats.stream_dropped++; return -(int)OW_ERR_BUFFER; }
    stream_take(buf, hdr[1]);
    *src = hdr[0];
    return (int)hdr[1];
}

static uint32_t owfw_stream_drops(void* ctx) {
    (void)ctx;
    owfw_pump();                  /* fold in MAIN's latest totals */
    ow_stream_link_service(&g_link);   /* and write off frames that died on the way */
    return ow_stream_link_drops(&g_link) + g_stats.stream_dropped + g_stats.stream_malformed;
}

static const ow_stream_ops g_stream_ops = {
    0, owfw_stream_write, owfw_stream_poll, owfw_stream_drops
};

/* ── Public API ────────────────────────────────────────────────────────── */
ow_status ow_open_fwgui(ow_device* dev) {
    ow_status st;
    sdfs_transport_t sd;
    ow_transport t;
#ifndef OWFW_NO_IRQ
    if (g_irq_installed) {
        irq_set_enabled(UART0_IRQ, false);
    }
#endif
    memset(&g_rx, 0, sizeof g_rx);
    g_text.head = g_text.count = 0;
    g_binary.head = g_binary.count = 0;
    memset(g_sdfs, 0, sizeof g_sdfs);
    g_sdfs_head = g_sdfs_count = 0;
    g_stream_head = g_stream_used = g_stream_count = 0;
    ow_stream_link_init(&g_link, OW_STREAM_PEER_DISPLAY, owfw_stream_send, owfw_now_ms, 0);
    memset(&g_stats, 0, sizeof g_stats);
    g_ring_head = g_ring_tail = 0;
    uart_init(uart0, 8000000);   /* OWFW_BAUD */
    gpio_set_function(OWFW_PIN_TX,  GPIO_FUNC_UART);
    gpio_set_function(OWFW_PIN_RX,  GPIO_FUNC_UART);
    gpio_set_function(OWFW_PIN_CTS, GPIO_FUNC_UART);
    gpio_set_function(OWFW_PIN_RTS, GPIO_FUNC_UART);
    uart_set_hw_flow(uart0, true, true);
#ifndef OWFW_NO_IRQ
    uart_set_fifo_enabled(uart0, true);
    /* RX interrupt: fires at half a FIFO (16 bytes = 20 us of wire time) or
     * on the receive timeout for a partial FIFO, and drains into the ring. */
    if (!g_irq_installed) {
        irq_set_exclusive_handler(UART0_IRQ, owfw_uart_irq);
        g_irq_installed = true;
    }
    uart_set_irq_enables(uart0, true, false);
    hw_write_masked(&uart_get_hw(uart0)->ifls,
                    2u << UART_UARTIFLS_RXIFLSEL_LSB, UART_UARTIFLS_RXIFLSEL_BITS);
    irq_set_enabled(UART0_IRQ, true);
#else
    (void)g_irq_installed;
#endif
    t.ctx = 0;
    t.write = owfw_write;
    t.read = owfw_read_text;
    st = ow_open(dev, &t);
    if (st != OW_OK) return st;
    /* ow_open cleared dev->stream: from here on streams take this link's
     * push path instead of MAIN's text commands. */
    ow_stream_bind(dev, &g_stream_ops);
    /* Arm SD access (see onewili_sd.h). Failures here are not fatal: they just
     * mean MAIN is not answering yet, and ow_sd_* calls will report it. */
    sd = ow_fwgui_sdfs_transport();
    (void)ow_sd_arm(&sd);
    return OW_OK;
}

ow_transport ow_fwgui_binary_transport(void) {
    ow_transport t;
    t.ctx = 0;
    t.write = 0;                  /* the binary stream is read-only */
    t.read = owfw_read_binary;
    return t;
}

uint32_t ow_fwgui_dropped_frames(void) { return g_stats.dropped_frames; }

void ow_fwgui_get_stats(ow_fwgui_stats* out) {
    if (!out) return;
    *out = g_stats;
    out->stream_tx_frames  = g_link.tx_frames;
    out->stream_tx_refused = g_link.tx_refused;
    out->stream_tx_lost    = g_link.tx_lost;
    out->stream_credits    = g_link.credits;
    out->stream_confirmed  = g_link.confirmed;
}

void ow_fwgui_send_power_zones(uint32_t zone_mask) {
    uint8_t payload[3] = {
        (uint8_t)(zone_mask & 0xFF),
        (uint8_t)((zone_mask >> 8) & 0xFF),
        (uint8_t)((zone_mask >> 16) & 0xFF),
    };
    owfw_send_event(OWFW_EVT_POWER_ZONES, payload, 3);
}
