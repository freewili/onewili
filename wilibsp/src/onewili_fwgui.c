#include "onewili_fwgui.h"
#include <string.h>
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "onewili_sd.h"
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
#define OWFW_SDFS_SLOTS    8       /* SDFS RX frames buffered       */
#define OWFW_SDFS_POLL_US  1000    /* an idle recv poll costs ~1 ms */
#define OWFW_STREAM_MAX    1024    /* per-stream buffer             */
#define OWFW_FRAME_MAX     512     /* incoming command frame payload cap */

/* ── Per-stream byte FIFO ──────────────────────────────────────────────── */
typedef struct {
    uint8_t  buf[OWFW_STREAM_MAX];
    uint32_t head, count;
} owfw_fifo;

static owfw_fifo g_text;     /* 0x5D: responses + "[*" text events */
static owfw_fifo g_binary;   /* 0x5E: binary WILI event bytes      */
static uint32_t  g_dropped;

static void fifo_push_frame(owfw_fifo* f, const uint8_t* p, uint32_t n) {
    if (n > OWFW_STREAM_MAX - f->count) { g_dropped++; return; }  /* drop-newest, whole frame */
    for (uint32_t i = 0; i < n; i++) {
        f->buf[(f->head + f->count) % OWFW_STREAM_MAX] = p[i];
        f->count++;
    }
}

static uint32_t fifo_pop(owfw_fifo* f, uint8_t* out, uint32_t cap) {
    uint32_t n = f->count < cap ? f->count : cap;
    for (uint32_t i = 0; i < n; i++) {
        out[i] = f->buf[f->head];
        f->head = (f->head + 1) % OWFW_STREAM_MAX;
        f->count--;
    }
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
    if (n > SDFS_MAX_FRAME || g_sdfs_count >= OWFW_SDFS_SLOTS) { g_dropped++; return; }
    s = &g_sdfs[(g_sdfs_head + g_sdfs_count) % OWFW_SDFS_SLOTS];
    memcpy(s->buf, p, n);
    s->len = n;
    g_sdfs_count++;
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
    int      overlong;           /* payload > OWFW_FRAME_MAX: parse, discard */
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
        g_rx.sum += b; g_rx.len |= (uint16_t)(b << 8);
        g_rx.got = 0;
        g_rx.overlong = g_rx.len > OWFW_FRAME_MAX;
        g_rx.st = RX_CMD;
        break;
    case RX_CMD:
        g_rx.sum += b; g_rx.cmd = b;
        g_rx.st = g_rx.len ? RX_PAYLOAD : RX_CK0;
        break;
    case RX_PAYLOAD:
        g_rx.sum += b;
        if (!g_rx.overlong) g_rx.payload[g_rx.got] = b;
        if (++g_rx.got >= g_rx.len) g_rx.st = RX_CK0;
        break;
    case RX_CK0: g_rx.ck = b; g_rx.st = RX_CK1; break;
    case RX_CK1:
        g_rx.ck |= (uint16_t)(b << 8);
        if (g_rx.ck == g_rx.sum && !g_rx.overlong) {
            if (g_rx.cmd == OWFW_CMD_RESPONSE) fifo_push_frame(&g_text, g_rx.payload, g_rx.len);
            else if (g_rx.cmd == OWFW_CMD_BINARY) fifo_push_frame(&g_binary, g_rx.payload, g_rx.len);
            else if (g_rx.cmd == OWFW_CMD_SDFS) sdfs_push(g_rx.payload, g_rx.len);
            /* every other command code (GUI traffic) is discarded */
        }
        g_rx.st = RX_SYNC0;
        break;
    }
}

static void owfw_pump(void) {
    while (uart_is_readable(uart0))
        rx_byte((uint8_t)uart_getc(uart0));
}

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
}

/* LOCAL ADDITION — not in the generated package; re-apply after every re-copy.
 * Generic B0 1D event frame: sync | len u16le (excludes event code) |
 * event code | payload | cksum u16le (additive sum over every preceding
 * byte). Fire-and-forget, like owfw_send_chunk — no response is read. */
static void owfw_send_event(uint8_t event_code, const uint8_t* payload, uint8_t n) {
    uint8_t f[2 + 2 + 1 + 32 + 2];
    uint16_t len = (uint16_t)n;
    uint32_t k = 0;
    f[k++] = OWFW_EVT_SYNC0; f[k++] = OWFW_EVT_SYNC1;
    f[k++] = (uint8_t)(len & 0xFF); f[k++] = (uint8_t)(len >> 8);
    f[k++] = event_code;
    memcpy(&f[k], payload, n); k += n;
    uint16_t sum = 0;
    for (uint32_t i = 0; i < k; i++) sum = (uint16_t)(sum + f[i]);
    f[k++] = (uint8_t)(sum & 0xFF); f[k++] = (uint8_t)(sum >> 8);
    uart_write_blocking(uart0, f, k);
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

/* ── Public API ────────────────────────────────────────────────────────── */
ow_status ow_open_fwgui(ow_device* dev) {
    ow_status st;
    sdfs_transport_t sd;
    ow_transport t;
    memset(&g_rx, 0, sizeof g_rx);
    memset(&g_text, 0, sizeof g_text);
    memset(&g_binary, 0, sizeof g_binary);
    memset(g_sdfs, 0, sizeof g_sdfs);
    g_sdfs_head = g_sdfs_count = 0;
    g_dropped = 0;
    uart_init(uart0, 8000000);   /* OWFW_BAUD */
    gpio_set_function(OWFW_PIN_TX,  GPIO_FUNC_UART);
    gpio_set_function(OWFW_PIN_RX,  GPIO_FUNC_UART);
    gpio_set_function(OWFW_PIN_CTS, GPIO_FUNC_UART);
    gpio_set_function(OWFW_PIN_RTS, GPIO_FUNC_UART);
    uart_set_hw_flow(uart0, true, true);
    t.ctx = 0;
    t.write = owfw_write;
    t.read = owfw_read_text;
    st = ow_open(dev, &t);
    if (st != OW_OK) return st;
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

uint32_t ow_fwgui_dropped_frames(void) { return g_dropped; }

/* LOCAL ADDITION — not in the generated package; re-apply after every re-copy. */
void ow_fwgui_send_power_zones(uint32_t zone_mask) {
    uint8_t payload[3] = {
        (uint8_t)(zone_mask & 0xFF),
        (uint8_t)((zone_mask >> 8) & 0xFF),
        (uint8_t)((zone_mask >> 16) & 0xFF),
    };
    owfw_send_event(OWFW_EVT_POWER_ZONES, payload, 3);
}
