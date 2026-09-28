#include "onewili.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── command assembly ──────────────────────────────────────────────────── */

static ow_status ow__cat(char* dst, size_t cap, size_t* pos, const char* s) {
    size_t n = strlen(s);
    if (*pos + n + 1 > cap) return OW_ERR_ARG;
    memcpy(dst + *pos, s, n); *pos += n; dst[*pos] = 0;
    return OW_OK;
}
static ow_status ow__cat_int(char* d, size_t c, size_t* p, long v) {
    char b[32]; snprintf(b, sizeof b, " %ld", v); return ow__cat(d, c, p, b);
}
static ow_status ow__cat_hex(char* d, size_t c, size_t* p, unsigned long v, int w) {
    char b[40]; snprintf(b, sizeof b, " %0*lX", w, v); return ow__cat(d, c, p, b);
}
static ow_status ow__cat_bool(char* d, size_t c, size_t* p, bool v) {
    return ow__cat(d, c, p, v ? " 1" : " 0");
}
static ow_status ow__cat_float(char* d, size_t c, size_t* p, double v) {
    char b[48]; snprintf(b, sizeof b, " %g", v); return ow__cat(d, c, p, b);
}
static ow_status ow__cat_str(char* d, size_t c, size_t* p, const char* s) {
    ow_status r = ow__cat(d, c, p, " ");
    return r != OW_OK ? r : ow__cat(d, c, p, s);
}
static ow_status ow__cat_bytes(char* d, size_t c, size_t* p,
                               const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        char b[8]; snprintf(b, sizeof b, " %02X", (unsigned)data[i]);
        ow_status r = ow__cat(d, c, p, b);
        if (r != OW_OK) return r;
    }
    return OW_OK;
}

/* ── framing: [<path> <hexTsNs> <seq> <response...> <ok>] ─────────────── */

static int ow__is_frame(const char* s) {
    if (s[0] != '[' || s[1] == '*') return 0;
    return s[1] == '?' || (s[1] >= 'A' && s[1] <= 'Z') || (s[1] >= 'a' && s[1] <= 'z');
}

/* Opens either kind of frame, response or event. Length-checked: the caller
 * asks about bytes that may still be mid-arrival. */
static int ow__opens_frame(const char* s, size_t len) {
    if (len < 2 || s[0] != '[') return 0;
    return s[1] == '*' || s[1] == '?' ||
           (s[1] >= 'A' && s[1] <= 'Z') || (s[1] >= 'a' && s[1] <= 'z');
}

/* Does s[0..len) end with the closing " <ok>]" token? Judged on the whole
 * token and never on a bare ']', because a payload can end a line with a ']'
 * of its own. */
static int ow__frame_closed(const char* s, size_t len) {
    if (len && s[len - 1] == '\r') --len;
    return len >= 3 && s[len - 1] == ']' &&
           (s[len - 2] == '0' || s[len - 2] == '1') &&
           (s[len - 3] == ' ' || s[len - 3] == '\t' || s[len - 3] == '\n');
}

static ow_status ow__parse_frame(const char* line, char* resp, size_t cap, int* ok) {
    size_t len = strlen(line);
    if (len < 5 || line[0] != '[' || line[len - 1] != ']') return OW_ERR_PROTOCOL;
    const char* p = line + 1;
    const char* end = line + len - 1;
    int spaces = 0;
    const char* body = NULL;
    for (const char* q = p; q < end; ++q)
        if (*q == ' ' && ++spaces == 3) { body = q + 1; break; }
    if (!body) return OW_ERR_PROTOCOL;
    const char* last = end;                 /* trailing " <ok>" token */
    while (last > body && last[-1] != ' ') --last;
    if (last <= body) return OW_ERR_PROTOCOL;
    *ok = (last[0] == '1');
    size_t rlen = (size_t)((last - 1) - body);
    /* A payload newline abutting the closing token is the line terminator the
     * firmware printed, not payload; newlines inside the payload stay. */
    while (rlen && (body[rlen - 1] == '\n' || body[rlen - 1] == '\r')) --rlen;
    if (rlen >= cap) return OW_ERR_BUFFER;
    memcpy(resp, body, rlen); resp[rlen] = 0;
    return OW_OK;
}

/* ── spontaneous text events ──────────────────────────────────────────── */

static int ow__is_event_line(const char* s) {
    return s[0] == '[' && s[1] == '*';
}

static void ow__evq_push(ow_device* dev, const char* line) {
    size_t slot;
    if (dev->evq_count == OW_TEXT_EVENT_QUEUE) {
        dev->evq_head = (dev->evq_head + 1) % OW_TEXT_EVENT_QUEUE;
        --dev->evq_count;
        ++dev->dropped_text_events;
    }
    slot = (dev->evq_head + dev->evq_count) % OW_TEXT_EVENT_QUEUE;
    strncpy(dev->evq[slot], line, OW_RESP_MAX - 1);
    dev->evq[slot][OW_RESP_MAX - 1] = 0;
    ++dev->evq_count;
}

/* Accumulate transport reads into dev->line until one '\n'-terminated line is
 * complete; the line (without EOL) is left at dev->line[0..] and the remainder
 * shifted down for the next call.
 *
 * A handler that prints a newline into its response splits the frame across
 * physical lines, so a newline inside an unclosed frame is not a line ending:
 * the scan keeps going until the line carrying the " <ok>]" token. dev->line
 * doubles as the reassembly buffer, so OW_RESP_MAX caps how much an unclosed
 * '[' can absorb (OW_ERR_BUFFER below), and the transport's own timeout_ms
 * bounds how long it can wait; ow__call clears line_len per command so a
 * half-frame never leaks into the next one. */
static ow_status ow__read_line(ow_device* dev, char* out, size_t outcap,
                               uint32_t timeout_ms) {
    for (;;) {
        int routed = 0;
        for (size_t i = 0; i < dev->line_len; ++i) {
            if (dev->line[i] == '\n') {
                size_t linelen;
                if (ow__opens_frame(dev->line, dev->line_len)
                    && !ow__frame_closed(dev->line, i)) {
                    if (!ow__opens_frame(dev->line + i + 1,
                                         dev->line_len - i - 1))
                        continue;   /* payload newline: wait for the token */
                    /* The next line opens a frame of its own, so the held one
                     * can never close. Drop it and rescan; guessing which of
                     * two open frames a later line belongs to is not
                     * decidable from the wire. */
                    memmove(dev->line, dev->line + i + 1,
                            dev->line_len - i - 1);
                    dev->line_len -= i + 1;
                    routed = 1;
                    break;
                }
                linelen = i;
                if (linelen && dev->line[linelen - 1] == '\r') --linelen;
                if (linelen >= outcap) return OW_ERR_BUFFER;
                memcpy(out, dev->line, linelen); out[linelen] = 0;
                memmove(dev->line, dev->line + i + 1, dev->line_len - i - 1);
                dev->line_len -= i + 1;
                if (ow__is_event_line(out)) { ow__evq_push(dev, out); routed = 1; break; }
                return OW_OK;
            }
        }
        if (routed) continue;   /* rescan the shifted buffer for more lines */
        if (dev->line_len + 1 >= sizeof dev->line) return OW_ERR_BUFFER;
        {
            int n = dev->t.read(dev->t.ctx, (uint8_t*)dev->line + dev->line_len,
                                sizeof dev->line - dev->line_len - 1, timeout_ms);
            if (n == 0) return OW_ERR_TIMEOUT;
            if (n < 0) return OW_ERR_IO;
            dev->line_len += (size_t)n;
        }
    }
}

/* Send one command (0x02 reset prefix + one-shot path) and wait for the next
 * standard response frame. resp receives the frame's response middle. */
static ow_status ow__call(ow_device* dev, const char* cmd,
                          char* resp, size_t respcap) {
    uint8_t out[OW_CMD_MAX + 2];
    size_t clen = strlen(cmd);
    if (clen + 2 > sizeof out) return OW_ERR_ARG;
    out[0] = 0x02;                          /* reset to root + quiet */
    memcpy(out + 1, cmd, clen);
    out[1 + clen] = '\n';
    dev->line_len = 0;                      /* flush stale input */
    if (dev->t.write(dev->t.ctx, out, clen + 2) < 0) return OW_ERR_IO;
    for (;;) {
        char linebuf[OW_RESP_MAX];
        ow_status r = ow__read_line(dev, linebuf, sizeof linebuf,
                                    OW_DEFAULT_TIMEOUT_MS);
        if (r != OW_OK) return r;
        if (!linebuf[0] || !ow__is_frame(linebuf)) continue;
        int ok = 0;
        r = ow__parse_frame(linebuf, resp, respcap, &ok);
        if (r != OW_OK) return r;
        return ok ? OW_OK : OW_ERR_FAILED;
    }
}

/* ── response token decoding ──────────────────────────────────────────── */

static char* ow__tok(char** cur) {
    char* s = *cur;
    while (*s == ' ') ++s;
    if (!*s) return NULL;
    char* e = s;
    while (*e && *e != ' ') ++e;
    if (*e) { *e = 0; *cur = e + 1; } else { *cur = e; }
    return s;
}
static ow_status ow__tok_long(char** cur, long* out, int base) {
    char* t = ow__tok(cur);
    if (!t) return OW_ERR_PROTOCOL;
    char* end = NULL;
    *out = strtol(t, &end, base);
    return (end && *end == 0) ? OW_OK : OW_ERR_PROTOCOL;
}
static ow_status ow__tok_ulong(char** cur, unsigned long* out, int base) {
    char* t = ow__tok(cur);
    if (!t) return OW_ERR_PROTOCOL;
    char* end = NULL;
    *out = strtoul(t, &end, base);
    return (end && *end == 0) ? OW_OK : OW_ERR_PROTOCOL;
}
static ow_status ow__tok_double(char** cur, double* out) {
    char* t = ow__tok(cur);
    if (!t) return OW_ERR_PROTOCOL;
    char* end = NULL;
    *out = strtod(t, &end);
    return (end && *end == 0) ? OW_OK : OW_ERR_PROTOCOL;
}
static ow_status ow__rest_bytes(char** cur, uint8_t* buf, size_t cap, size_t* n) {
    if (!buf || !n) return OW_ERR_ARG;
    *n = 0;
    for (;;) {
        char* t = ow__tok(cur);
        if (!t) return OW_OK;
        char* end = NULL;
        unsigned long v = strtoul(t, &end, 16);
        if (!end || *end != 0 || v > 0xFF) return OW_ERR_PROTOCOL;
        if (*n >= cap) return OW_ERR_BUFFER;
        buf[(*n)++] = (uint8_t)v;
    }
}
static ow_status ow__rest_str(char** cur, char* buf, size_t cap) {
    if (!buf) return OW_ERR_ARG;
    while (**cur == ' ') ++*cur;
    if (strlen(*cur) >= cap) return OW_ERR_BUFFER;
    strcpy(buf, *cur);
    *cur += strlen(*cur);
    return OW_OK;
}
static ow_status ow__tok_str(char** cur, char* buf, size_t cap) {
    if (!buf) return OW_ERR_ARG;
    char* t = ow__tok(cur);
    if (!t) return OW_ERR_PROTOCOL;
    if (strlen(t) >= cap) return OW_ERR_BUFFER;
    strcpy(buf, t);
    return OW_OK;
}

/* Some commands decode nothing - keep -Wunused-function quiet either way. */
typedef int ow__unused_guard;

/* ── open/close ───────────────────────────────────────────────────────── */

ow_status ow_open(ow_device* dev, const ow_transport* transport) {
    if (!dev || !transport || !transport->write || !transport->read)
        return OW_ERR_ARG;
    dev->t = *transport;
    dev->line_len = 0;
    dev->evq_head = dev->evq_count = 0;
    dev->dropped_text_events = 0;
    dev->stream = NULL;
    dev->stream_local_drops = 0;
    dev->stream_stash_len = dev->stream_stash_pos = 0;
    {
        const uint8_t reset[2] = {0x02, '\n'};
        if (dev->t.write(dev->t.ctx, reset, 2) < 0) return OW_ERR_IO;
    }
    return OW_OK;
}

void ow_close(ow_device* dev) {
    if (!dev || !dev->t.write) return;
    const uint8_t reset[1] = {0x02};
    (void)dev->t.write(dev->t.ctx, reset, 1);
}

static void ow_raw_stash(const char* line);   /* defined below */

int ow_poll_text_line(ow_device* dev, char* id, size_t id_cap,
                      char* args, size_t args_cap) {
    if (!dev || !id || !args || id_cap == 0 || args_cap == 0)
        return -(int)OW_ERR_ARG;
    if (dev->evq_count == 0) {
        /* One zero-timeout pass; ow__read_line routes any event lines it
         * completes into the queue and returns OW_ERR_TIMEOUT when only
         * events (or nothing) arrived. */
        char linebuf[OW_RESP_MAX];
        ow_status r = ow__read_line(dev, linebuf, sizeof linebuf, 0);
        if (r != OW_OK && r != OW_ERR_TIMEOUT) return -(int)r;
        /* A pipelined caller (ow_raw_send) may be waiting for this response:
         * keep it for ow_raw_next_response. Nothing else has a waiter. */
        if (r == OW_OK) ow_raw_stash(linebuf);
    }
    if (dev->evq_count == 0) return 0;
    {
        const char* line = dev->evq[dev->evq_head];
        const char* s = line + 2;               /* skip "[*" */
        size_t n = strlen(s);
        size_t idend = 0, idlen, alen;
        const char* a;
        if (n && s[n - 1] == ']') --n;          /* drop trailing ']' */
        while (idend < n && s[idend] != ' ') ++idend;
        idlen = idend;
        if (idlen >= id_cap) idlen = id_cap - 1;
        memcpy(id, s, idlen); id[idlen] = 0;
        a = s + idend;
        while (*a == ' ' && (size_t)(a - s) < n) ++a;
        alen = n - (size_t)(a - s);
        if (alen >= args_cap) alen = args_cap - 1;
        memcpy(args, a, alen); args[alen] = 0;
    }
    dev->evq_head = (dev->evq_head + 1) % OW_TEXT_EVENT_QUEUE;
    --dev->evq_count;
    return 1;
}

/* ── raw command hooks ─────────────────────────────────────────────────── */
/* For pipelined callers (onewili_fast.c): send a command without
 * waiting for its response, and fetch the next response frame while the
 * usual event routing (ow__read_line -> evq) stays in place. Single-flight:
 * do not interleave with a generated call on the same device. */
int ow_raw_send(ow_device* dev, const char* cmd) {
    uint8_t out[OW_CMD_MAX + 2];
    size_t clen;
    if (!dev || !cmd) return -(int)OW_ERR_ARG;
    clen = strlen(cmd);
    if (clen + 2 > sizeof out) return -(int)OW_ERR_ARG;
    out[0] = 0x02;                          /* reset to root + quiet */
    memcpy(out + 1, cmd, clen);
    out[1 + clen] = (uint8_t)10;            /* newline */
    if (dev->t.write(dev->t.ctx, out, clen + 2) < 0) return -(int)OW_ERR_IO;
    return (int)(clen + 2);
}

/* Response frames that ow_poll_text_line completed while a pipelined caller
 * had commands unanswered. Responses are short; longer ones are truncated. */
#define OW_RAW_STASH_N    32
#define OW_RAW_STASH_LINE 192
static char     ow_raw_stash_buf[OW_RAW_STASH_N][OW_RAW_STASH_LINE];
static unsigned ow_raw_stash_r, ow_raw_stash_n, ow_raw_stash_lost;

static void ow_raw_stash(const char* line) {
    unsigned w;
    if (ow_raw_stash_n >= OW_RAW_STASH_N) { ow_raw_stash_lost++; return; }
    w = (ow_raw_stash_r + ow_raw_stash_n) % OW_RAW_STASH_N;
    strncpy(ow_raw_stash_buf[w], line, OW_RAW_STASH_LINE - 1);
    ow_raw_stash_buf[w][OW_RAW_STASH_LINE - 1] = 0;
    ow_raw_stash_n++;
}

void ow_raw_stash_clear(void) { ow_raw_stash_r = ow_raw_stash_n = 0; }
unsigned ow_raw_stash_lost_count(void) { return ow_raw_stash_lost; }

ow_status ow_raw_next_response(ow_device* dev, char* resp, size_t cap, int* ok, uint32_t timeout_ms) {
    static char linebuf[OW_RESP_MAX];       /* single-flight; keeps 4 KB off the stack */
    if (!dev || !resp || !ok) return OW_ERR_ARG;
    if (ow_raw_stash_n) {
        const char* line = ow_raw_stash_buf[ow_raw_stash_r];
        ow_status r = ow__parse_frame(line, resp, cap, ok);
        ow_raw_stash_r = (ow_raw_stash_r + 1) % OW_RAW_STASH_N;
        ow_raw_stash_n--;
        return r;
    }
    for (;;) {
        ow_status r = ow__read_line(dev, linebuf, sizeof linebuf, timeout_ms);
        if (r != OW_OK) return r;
        if (!linebuf[0] || !ow__is_frame(linebuf)) continue;
        return ow__parse_frame(linebuf, resp, cap, ok);
    }
}

ow_status ow_io_gpio_set_io_high(ow_device* dev, int32_t pin)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_set_io_low(ow_device* dev, int32_t pin)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\l")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_set_io_toggle(ow_device* dev, int32_t pin)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_set_pwm(ow_device* dev, int32_t gpio_number, double freq, double duty)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)gpio_number)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, freq)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, duty)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_read_all(ow_device* dev, uint32_t* gpiostate)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\u")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (gpiostate) *gpiostate = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_io_gpio_stream_io(ow_device* dev, int32_t reportratems)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)reportratems)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_toggle_hsbdio(ow_device* dev, int32_t pin)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_set_io_voltage_source(ow_device* dev, int32_t source)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)source)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_s_pi1_rx12(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_g_pio2626(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\b")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_s_pi1cs13(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_g_pio27(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_u_art1_rx9(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_u_art1cts10(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\f")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_u_art1_tx8(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_u_art1rts11(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\m")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_s_pi1_tx15(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_s_pi1sclk14(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\j")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_g_pio2525(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\g\\a\\k")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_u_art_write(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\w")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_toggle_stream(ow_device* dev, uint8_t* data_bytes, size_t data_bytes_cap, size_t* data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_bytes(&cur, data_bytes, data_bytes_cap, data_bytes_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_uart_uart_enable_api_mode(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_settings_baud_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\s\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_settings_r_ts_hand_shaking(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\s\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_settings_c_ts_hand_shaking(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\s\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_settings_data_bits(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\s\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_settings_parity(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\s\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_settings_stop_bits(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\s\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_uart_settings_module(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\u\\s\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_poll_sfp(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_read_sfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, uint32_t* sfp_response)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\b")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)device_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, register_address, register_address_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (sfp_response) *sfp_response = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_io_mdio_mdio_write_sfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\c")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)device_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, register_address, register_address_len)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdiormwsfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\e")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)device_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, register_address, register_address_len)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, mask_bytes, mask_bytes_len)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_poll(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\y")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_read22(ow_device* dev, uint8_t phy_address, uint8_t register_address, uint32_t* mdio_response)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\g")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 2)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (mdio_response) *mdio_response = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_io_mdio_mdio_write22(ow_device* dev, uint8_t phy_address, uint8_t register_address, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\i")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdiormw22(ow_device* dev, uint8_t phy_address, uint8_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\j")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, mask_bytes, mask_bytes_len)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_read45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, uint32_t* mdio_response)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\k")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)mmd_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 4)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (mdio_response) *mdio_response = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_io_mdio_mdio_write45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\l")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)mmd_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 4)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdiormw45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\m")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)mmd_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 4)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, mask_bytes, mask_bytes_len)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_read_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, uint32_t* mdio_response)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\n")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)mmd_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 4)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (mdio_response) *mdio_response = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_io_mdio_mdio_write_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\o")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)mmd_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 4)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_mdio_mdiormw_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\m\\p")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)phy_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)mmd_address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_address, 4)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, mask_bytes, mask_bytes_len)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_sensors_enable_motion_stream(ow_device* dev, int32_t stream_rate_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\s\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)stream_rate_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_sensors_enable_field_stream(ow_device* dev, int32_t stream_rate_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\s\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)stream_rate_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_sensors_enable_env_stream(ow_device* dev, int32_t stream_rate_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\s\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)stream_rate_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_sensors_enable_orientation_stream(ow_device* dev, int32_t stream_rate_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\s\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)stream_rate_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_sensors_get_sensors(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\s\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_write(ow_device* dev, uint8_t address, uint8_t register_, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\i\\w")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)address, 2)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)register_, 2)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_read(ow_device* dev, uint8_t* i2crepsone, size_t i2crepsone_cap, size_t* i2crepsone_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\i\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_bytes(&cur, i2crepsone, i2crepsone_cap, i2crepsone_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_poll(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\i\\p")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_slave_enable(ow_device* dev, uint8_t address)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\i\\e")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)address, 2)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_slave_set_data(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\i\\l")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_i2c_settings_frequency(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\i\\s\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_i2c_settings_pull_ups(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\i\\s\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_spi_s_pi_write(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len, uint8_t* spi_response, size_t spi_response_cap, size_t* spi_response_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\e\\w")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_bytes(&cur, spi_response, spi_response_cap, spi_response_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_spi_s_pi_slave_enable(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\e\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_spi_s_pi_slave_set_data(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\e\\l")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_bytes, data_bytes_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_spi_settings_frequency(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\e\\s\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_spi_settings_chip_select_pin(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\e\\s\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_spi_settings_data_bits(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\e\\s\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_spi_settings_c_pol(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\e\\s\\p")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_spi_settings_c_pha(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\e\\s\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_enable_canfd_stream(ow_device* dev, int32_t channel, int32_t enabled)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enabled)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_write_canfd(ow_device* dev, int32_t channel, uint32_t arb_id, int32_t can_fd, int32_t xtd_id, const uint8_t* data_in, size_t data_in_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\w")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)arb_id, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)can_fd)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)xtd_id)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_in, data_in_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_write_canfd_periodic(ow_device* dev, int32_t index, int32_t enable, int32_t period, int32_t channel, uint32_t arb_id, int32_t can_fd, int32_t xtd_id, const uint8_t* data_in, size_t data_in_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)period)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)arb_id, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)can_fd)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)xtd_id)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data_in, data_in_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_setup_filter(ow_device* dev, int32_t channel, int32_t index, int32_t enable, int32_t xtd_id, uint32_t mask, uint32_t accept, uint32_t maskb0, uint32_t accept_b0, uint32_t maskb1, uint32_t accept_b1)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)xtd_id)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)mask, 8)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)accept, 8)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)maskb0, 8)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)accept_b0, 8)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)maskb1, 8)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)accept_b1, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_read_can_registers(ow_device* dev, int32_t channel, uint32_t start_address, int32_t word_count, char* registers, size_t registers_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)start_address, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)word_count)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, registers, registers_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_canfd_set_can_register(ow_device* dev, int32_t channel, uint32_t start_address, int32_t byte_count, uint32_t word_to_write)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)start_address, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)byte_count)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)word_to_write, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_enable_canfd_receive_queue(ow_device* dev, int32_t channel, int32_t enabled)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enabled)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_receive_canfd(ow_device* dev, int32_t channel, bool* frame, int32_t* queued, int32_t* dropped, uint32_t* arb_id, int32_t* xtd_id, int32_t* can_fd, int32_t* timestamp_us, int32_t* dlc, uint8_t* data, size_t data_cap, size_t* data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (frame) *frame = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (queued) *queued = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (dropped) *dropped = (int32_t)v; }
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (arb_id) *arb_id = (uint32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (xtd_id) *xtd_id = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (can_fd) *can_fd = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (timestamp_us) *timestamp_us = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (dlc) *dlc = (int32_t)v; }
    if ((r = ow__rest_bytes(&cur, data, data_cap, data_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_enable(ow_device* dev, bool enable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\e")) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_configure_addressing(ow_device* dev, uint32_t tx_id, uint32_t rx_id, bool extended_id, bool can_fd, int32_t tx_data_length, bool padding, uint32_t pad_byte, int32_t addressing_mode, uint32_t ext_address)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\c")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)tx_id, 8)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)rx_id, 8)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, extended_id)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, can_fd)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)tx_data_length)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, padding)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)pad_byte, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)addressing_mode)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)ext_address, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_configure_flow_control(ow_device* dev, int32_t block_size, uint32_t st_min, int32_t wft_max)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)block_size)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)st_min, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)wft_max)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_set_st_min_trim(ow_device* dev, int32_t st_min_trim_us, int32_t st_min_override_us)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)st_min_trim_us)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)st_min_override_us)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_send_message(ow_device* dev, const uint8_t* data, size_t data_len, int32_t* result, int32_t* bytes, int32_t* frames, int32_t* duration_us, int32_t* min_gap_us, int32_t* max_gap_us, int32_t* avg_gap_us)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\s")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data, data_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (result) *result = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (bytes) *bytes = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (frames) *frames = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (duration_us) *duration_us = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (min_gap_us) *min_gap_us = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (max_gap_us) *max_gap_us = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (avg_gap_us) *avg_gap_us = (int32_t)v; }
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_send_file(ow_device* dev, const char* file_path, int32_t* result, int32_t* bytes, int32_t* frames, int32_t* duration_us, int32_t* min_gap_us, int32_t* max_gap_us, int32_t* avg_gap_us)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\x")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, file_path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (result) *result = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (bytes) *bytes = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (frames) *frames = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (duration_us) *duration_us = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (min_gap_us) *min_gap_us = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (max_gap_us) *max_gap_us = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (avg_gap_us) *avg_gap_us = (int32_t)v; }
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_receive_message(ow_device* dev, int32_t* status, int32_t* length, int32_t* in_file, uint8_t* data, size_t data_cap, size_t* data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (status) *status = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (length) *length = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (in_file) *in_file = (int32_t)v; }
    if ((r = ow__rest_bytes(&cur, data, data_cap, data_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_set_receive_file_path(ow_device* dev, const char* file_path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\p")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, file_path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_abort(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_canfd_isotp_iso_tp_show_status(ow_device* dev, int32_t* state, int32_t* last_result, int32_t* rx_count, int32_t* tx_count, int32_t* errors)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\c\\t\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (state) *state = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (last_result) *last_result = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (rx_count) *rx_count = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (tx_count) *tx_count = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (errors) *errors = (int32_t)v; }
    return OW_OK;
}

ow_status ow_io_analog_in_enable_analog_in_stream(ow_device* dev, int32_t stream_rate_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\j\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)stream_rate_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_in_read_analog_in2024(ow_device* dev, double* v0, double* v1, double* v2, double* v3)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\j\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { double v; if ((r = ow__tok_double(&cur, &v)) != OW_OK) return r;
      if (v0) *v0 = v; }
    { double v; if ((r = ow__tok_double(&cur, &v)) != OW_OK) return r;
      if (v1) *v1 = v; }
    { double v; if ((r = ow__tok_double(&cur, &v)) != OW_OK) return r;
      if (v2) *v2 = v; }
    { double v; if ((r = ow__tok_double(&cur, &v)) != OW_OK) return r;
      if (v3) *v3 = v; }
    return OW_OK;
}

ow_status ow_io_analog_in_config_analog_in2024(ow_device* dev, int32_t channel, int32_t mux, int32_t range)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\j\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mux)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)range)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_in_set_data_rate2024(ow_device* dev, int32_t rate)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\j\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)rate)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_in_enable_analog_in2024_stream(ow_device* dev, int32_t stream_rate_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\j\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)stream_rate_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_out_set_analog_output(ow_device* dev, int32_t channel, double value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\a\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_out_set_trigger_window(ow_device* dev, double value_low, double value_high)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\a\\t")) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, value_low)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, value_high)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_out_set_enable_trigger(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\a\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_out_set_v_prog_vout(ow_device* dev, int32_t enable, double set_voltage)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\a\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, set_voltage)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_out_set_glitch(ow_device* dev, int32_t nano_seconds)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\a\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)nano_seconds)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_out_set_waveform(ow_device* dev, int32_t channel, ow_dac_wave_shape_menu waveform, double frequency_hz, double low_voltage, double high_voltage, ow_dac_wave_phase phase)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\a\\w")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)channel)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)waveform)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, frequency_hz)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, low_voltage)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, high_voltage)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)phase)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_analog_out_set_waveform_run(ow_device* dev, int32_t mask)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\a\\x")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mask)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_player_setup_player(ow_device* dev, int32_t sample_rate_ns, int32_t sample_count, int32_t pin_start, int32_t pin_stop, int32_t start_mode, int32_t trigger_pin, bool loop)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\p\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)sample_rate_ns)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)sample_count)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin_start)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin_stop)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)start_mode)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)trigger_pin)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, loop)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_player_setup_analog(ow_device* dev, int32_t mask, int32_t analog_rate_ns, int32_t analog_resolution)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\p\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mask)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)analog_rate_ns)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)analog_resolution)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_player_load_file(ow_device* dev, const char* file_path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\p\\l")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, file_path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_player_start(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\p\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_player_stop(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\p\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_setup_logic_analyzer(ow_device* dev, int32_t sample_rate_ns, int32_t sample_count, int32_t pin_start, int32_t pin_stop, int32_t trigger_pin, int32_t trigger_type, int32_t rearm)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\b\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)sample_rate_ns)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)sample_count)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin_start)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin_stop)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)trigger_pin)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)trigger_type)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)rearm)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_setup_analog(ow_device* dev, int32_t analog_mask, int32_t analog_rate_ns, int32_t analog_res)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\b\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)analog_mask)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)analog_rate_ns)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)analog_res)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_start(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\b\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_stop(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\b\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_trigger(ow_device* dev, int32_t trigger_type)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\b\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)trigger_type)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_take_picture(ow_device* dev, int32_t destination, const char* filename)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)destination)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_start_recording_video(ow_device* dev, const char* filename)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\v")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_stop_recording_video(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_toggle_ai_detection_stream(ow_device* dev, int32_t ai_stream_mode)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)ai_stream_mode)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_zoom_level(ow_device* dev, int32_t zoom)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)zoom)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_contrast(ow_device* dev, int32_t contrast)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)contrast)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_saturation(ow_device* dev, int32_t saturation)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\i")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)saturation)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_brightness(ow_device* dev, int32_t brightness)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)brightness)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_hue(ow_device* dev, int32_t hue)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)hue)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_resolution(ow_device* dev, int32_t resolutionstate)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\y")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)resolutionstate)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_flash_state(ow_device* dev, bool flash)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\f\\l")) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, flash)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_audio_play_audio_file(ow_device* dev, const char* file_path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\k\\f")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, file_path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_audio_record_audio_file(ow_device* dev, const char* file_name)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\k\\r")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, file_name)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_audio_play_audio_asset(ow_device* dev, const char* asset_name)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\k\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, asset_name)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_audio_enable_audio_stream(ow_device* dev, int32_t enable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\k\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_audio_numbers_to_speech(ow_device* dev, double number)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\k\\n")) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, number)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_audio_tone(ow_device* dev, double frequency, double duration_ms, double amplitude)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\k\\t")) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, frequency)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, duration_ms)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, amplitude)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_audio_speak(ow_device* dev, const char* text)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\k\\v")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, text)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_serial_leds_configure_strip(ow_device* dev, int32_t strip, int32_t gpio, int32_t length, ow_ow_serial_led_type led_type, bool inverted)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\l\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)strip)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)gpio)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)length)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)led_type)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, inverted)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_serial_leds_show_config(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\l\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_serial_leds_set_leds(ow_device* dev, int32_t strip, int32_t start, int32_t count, int32_t red, int32_t green, int32_t blue, int32_t white)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\l\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)strip)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)start)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)count)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)red)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)green)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)blue)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)white)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_serial_leds_set_show(ow_device* dev, int32_t strip, ow_ow_led_light_show show)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\l\\w")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)strip)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)show)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_serial_leds_enable_jambu_orca(ow_device* dev, int32_t num_strips)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\l\\j")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)num_strips)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_serial_leds_auto_show(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\l\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_nice_usb_nice_usb_start(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\n\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_nice_usb_nice_usb_stop(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\n\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_nice_usb_nice_usb_status(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\n\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_nice_usb_nice_usb_run_test_script(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\n\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_nice_usb_niceusb_script(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\n\\c")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_nice_usb_niceusb_force_vidpid(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\n\\o")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_nice_usb_niceusb_vid(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\n\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_nice_usb_niceusb_pid(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\n\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_cdc_perf_cdc_perf_blast(ow_device* dev, int32_t bytes, int32_t chunk, int32_t* bytes_out, int32_t* elapsed_us, uint32_t* crc32)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\y\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)bytes)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)chunk)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (bytes_out) *bytes_out = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (elapsed_us) *elapsed_us = (int32_t)v; }
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (crc32) *crc32 = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_io_cdc_perf_cdc_perf_sink(ow_device* dev, int32_t bytes, int32_t* bytes_out, int32_t* elapsed_us, uint32_t* crc32)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\y\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)bytes)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (bytes_out) *bytes_out = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (elapsed_us) *elapsed_us = (int32_t)v; }
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (crc32) *crc32 = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_io_cdc_perf_cdc_perf_echo(ow_device* dev, int32_t rounds, int32_t chunk, int32_t* rounds_out, int32_t* elapsed_us)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\y\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)rounds)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)chunk)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (rounds_out) *rounds_out = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (elapsed_us) *elapsed_us = (int32_t)v; }
    return OW_OK;
}

ow_status ow_io_t1s_t1s_status(ow_device* dev, char* status, size_t status_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, status, status_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_t1s_t1s_link_status(ow_device* dev, char* info, size_t info_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\k")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, info, info_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_t1s_t1s_reinit_phy(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_t1s_clear_counters(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_t1s_register_read(ow_device* dev, int32_t mms, uint32_t address, uint32_t* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mms)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)address, 4)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (value) *value = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_io_t1s_bridge(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\b")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_start_periodic(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\p")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_start_flood(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\f")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_start_line_rate(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_start_burst(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\b")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_send_count(ow_device* dev, int32_t count)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)count)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_stop(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\x")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_show_stats(ow_device* dev, char* stats, size_t stats_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, stats, stats_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_clear_stats(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_set_dest_mac(ow_device* dev, const uint8_t* dest_mac, size_t dest_mac_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\m")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, dest_mac, dest_mac_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_eth_test_link_status(ow_device* dev, char* info, size_t info_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\k")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, info, info_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_frame_size(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\i")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_period_us(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_burst_count(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\n")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_line_rate_percent(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_payload_crc(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\v")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_responder(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_loopback(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_eth_test_frame_type(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\t\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_plca_p_lca_enabled(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\p\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_plca_local_id(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\p\\l")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_plca_node_count(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\p\\n")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_plca_t_o_timer(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\p\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_plca_burst_max(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\p\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_plca_burst_timer(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\p\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_t1s_tc10_generate_wake(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_t1s_tc10_wake_status(ow_device* dev, char* status, size_t status_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, status, status_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_t1s_tc10_enter_sleep(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_t1s_tc10_local_wake_pulse(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\w")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_t1s_tc10_cancel_sleep(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_forward_to_mdi(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\m")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_forward_to_wakeout(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\o")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_wake_on_mdi(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_wake_on_wakein(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\n")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_t1s_tc10_sleep_inhibit_delay(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\r\\w\\y")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_net_status(ow_device* dev, char* status, size_t status_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, status, status_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_net_net_link_status(ow_device* dev, char* info, size_t info_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, info, info_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_net_net_dhcp_renew(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_net_ping(ow_device* dev, const char* ip, int32_t count, char* result, size_t result_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\p")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, ip)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)count)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, result, result_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_io_net_net_clear_counters(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_net_apply(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_n_cm_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_n_cmip(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\i")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_n_cm_netmask(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\k")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_n_cm_gateway(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\g")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_t1sip(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\j")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_t1s_netmask(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\u")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_echo_servers(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_io_net_h_ttp_server(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "i\\w\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_set_led_color(ow_device* dev, int32_t ledindex, int32_t red, int32_t green, int32_t blue, int32_t duration, ow_ow_led_manager_led_mode mode)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)ledindex)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)red)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)green)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)blue)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)duration)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mode)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_show_fwi_image(ow_device* dev, const char* filename)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\l")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_clear_display(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_show_text(ow_device* dev, const char* texttodisplay)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\p")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, texttodisplay)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_read_all(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\u")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_stream_io(ow_device* dev, int32_t pin)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)pin)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_show_image_asset_by_id(ow_device* dev, int32_t image_id)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)image_id)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_screenshot(ow_device* dev, const char* filename, ow_ow_screenshot_file_type filetype, bool counter, bool timestamp)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\i")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)filetype)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, counter)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, timestamp)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_simulate_keypress(ow_device* dev, ow_ow_gui_button button, ow_ow_button_press_type presstype)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)button)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)presstype)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_reinit_lcd_panel(ow_device* dev, int32_t mode)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mode)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_panels_add_panel(ow_device* dev, bool use_tile, int32_t tile_id, const char* color, bool show_menu)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\c\\a")) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, use_tile)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)tile_id)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, color)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, show_menu)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_panels_add_panel_picklist(ow_device* dev, bool use_tile, int32_t tile_id, int32_t icon_id, int32_t log_index, const char* back_color, const char* fore_color, const char* caption)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\c\\b")) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, use_tile)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)tile_id)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)icon_id)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)log_index)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, fore_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, caption)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_panels_show_panel(ow_device* dev, int32_t index)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\c\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_panels_set_menu_text(ow_device* dev, int32_t button, const char* text)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\c\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)button)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, text)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_panels_read_buttons(ow_device* dev, uint32_t* pressed)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\c\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (pressed) *pressed = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_gui_controls_add_led(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t color, int32_t size, bool inital_value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)color)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)size)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, inital_value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_log_list(ow_device* dev, int32_t index, int32_t log, int32_t x, int32_t y, int32_t width, int32_t height, int32_t font_type, int32_t font_size, const char* back_color, const char* fore_color, bool list_mode)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)log)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)height)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)font_type)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)font_size)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, fore_color)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, list_mode)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_plot(ow_device* dev, int32_t index, int32_t plot_data_index_bit_field, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min_y, int32_t max_y, const char* back_color)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)plot_data_index_bit_field)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)height)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)min_y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)max_y)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_number(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t font_type, int32_t font_size, const char* fore_color, const char* back_color, bool is_float, int32_t float_digit_count, bool is_hex_format, bool is_unsigned)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\l")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)font_type)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)font_size)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, fore_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, is_float)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)float_digit_count)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, is_hex_format)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, is_unsigned)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_text(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t font_type, int32_t font_size, const char* fore_color, const char* back_color, const char* text)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)font_type)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)font_size)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, fore_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, text)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_bargraph(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min, int32_t max, const char* bar_color)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)height)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)min)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)max)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, bar_color)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_meter(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min, int32_t max, const char* needle_color)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)height)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)min)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)max)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, needle_color)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_button(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, const char* fore_color, const char* back_color, const char* text)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\i")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)height)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, fore_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, text)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_picture(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t picture_id)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\j")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)picture_id)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_picture_from_file(ow_device* dev, int32_t index, int32_t x, int32_t y, const char* picture_path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, picture_path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_waterfall(ow_device* dev, int32_t index, int32_t plot_data_index, int32_t bin_count, int32_t x, int32_t y, int32_t width, int32_t height, const char* back_color)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)plot_data_index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)bin_count)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)height)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_wili8(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t scale, const char* back_color, int32_t animation, const char* script_path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\n")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)height)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)scale)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)animation)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, script_path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_controls_add_file_list(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t mode, const char* back_color, const char* start_path, const char* filter)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\b\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)x)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)y)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)width)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)height)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mode)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, back_color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, start_path)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filter)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_text(ow_device* dev, int32_t index, const char* text)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, text)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_int(ow_device* dev, int32_t index, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_float(ow_device* dev, int32_t index, double value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_list_item_text(ow_device* dev, int32_t log_index, int32_t list_item, int32_t color, const char* text)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)log_index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)list_item)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)color)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, text)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_min_max_int(ow_device* dev, int32_t index, bool enable, int32_t min, int32_t max)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, enable)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)min)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)max)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_min_max_float(ow_device* dev, int32_t index, bool enable, double min, double max)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\l")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, enable)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, min)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, max)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_plot_data(ow_device* dev, int32_t plot_data_index, int32_t settings, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)plot_data_index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)settings)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_list_item_selected(ow_device* dev, int32_t log_index, int32_t list_index)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)log_index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)list_index)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_list_item_top_index(ow_device* dev, int32_t log_item, int32_t list_index)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\i")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)log_item)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)list_index)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_property(ow_device* dev, int32_t index, int32_t property, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\e\\j")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)property)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_message_box(ow_device* dev, int32_t auto_close_half_sec, bool show_ok, bool show_ok_cancel, bool show_none, int32_t picture_index, const char* message)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)auto_close_half_sec)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, show_ok)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, show_ok_cancel)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, show_none)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)picture_index)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, message)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_set_dialog_description(ow_device* dev, const char* description)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\b")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, description)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_progress_bar(ow_device* dev, int32_t picture_index, bool ok_to_close, bool auto_close_at100, int32_t auto_close_half_sec, const char* title)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)picture_index)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, ok_to_close)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, auto_close_at100)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)auto_close_half_sec)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, title)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_number_edit(ow_device* dev, int32_t min, int32_t max, int32_t initial, bool use_min_max, bool is_unsigned, bool hex_fomat, const char* message)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)min)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)max)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)initial)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, use_min_max)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, is_unsigned)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, hex_fomat)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, message)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_number_edit_float(ow_device* dev, double min, double max, double initial, bool use_min_max, int32_t digit_count, const char* message)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\e")) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, min)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, max)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, initial)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, use_min_max)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)digit_count)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, message)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_text_edit(ow_device* dev, const char* message, const char* inital_value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\f")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, message)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, inital_value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_pick_list(ow_device* dev, int32_t log_index, const char* message)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)log_index)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, message)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_show_text_editor(ow_device* dev, int32_t editor_type, const char* message, const char* inital_value, bool* basic)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\i")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)editor_type)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, message)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, inital_value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (basic) *basic = (v != 0); }
    return OW_OK;
}

ow_status ow_gui_dialogs_set_progess_dialog_value(ow_device* dev, int32_t value0_to100)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\j")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value0_to100)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_gui_dialogs_file_picker(ow_device* dev, int32_t mode, const char* start_path, const char* filter)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "g\\f\\l")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mode)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, start_path)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filter)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_get_time(ow_device* dev, int32_t* year, int32_t* month, int32_t* day, int32_t* weekday, int32_t* hour, int32_t* min, int32_t* sec)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (year) *year = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (month) *month = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (day) *day = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (weekday) *weekday = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (hour) *hour = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (min) *min = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (sec) *sec = (int32_t)v; }
    return OW_OK;
}

ow_status ow_hardware_set_time(ow_device* dev, int32_t year, int32_t month, int32_t day, int32_t hour, int32_t min, int32_t sec)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)year)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)month)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)day)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)hour)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)min)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)sec)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_software_reset(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\1")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_software_reset_to_bootloader(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\2")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_all_settings_to_defaults(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\3")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_baud_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\u\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_r_ts_hand_shaking(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\u\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_c_ts_hand_shaking(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\u\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_data_bits(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\u\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_parity(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\u\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_stop_bits(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\u\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_module(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\u\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_i2c_settings_frequency(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\i\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_i2c_settings_pull_ups(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\i\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_accel_range(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\v\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_gyro_range(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\v\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_move_threshold(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\v\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_t_cal_scale(ow_device* dev, double value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\v\\s")) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_t_cal_offset(ow_device* dev, double value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\v\\o")) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_stream_defaults(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\v\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_frequency(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\s\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_chip_select_pin(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\s\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_data_bits(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\s\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_c_pol(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\s\\p")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_c_pha(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\s\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_s_pi1_rx12(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_g_pio2626(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\b")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_s_pi1cs13(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_g_pio27(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_u_art1_rx9(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_u_art1cts10(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\f")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_u_art1_tx8(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_u_art1rts11(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\m")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_s_pi1_tx15(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_s_pi1sclk14(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\j")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_g_pio2525(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\o\\k")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_fpga_clock_settings_clk_source(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\f\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_fpga_clock_settings_clk_divider_int(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\f\\i")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_fpga_clock_settings_clk_divider_frac(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\f\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_fpga_clock_settings_comms_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\f\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_frequency_mhz(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_modulation(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_devation(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_channel(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_channel_spacing(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_rx_bandwidth(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\y")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_data_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_power_amp(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_sync_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\1")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_sync_word(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\i")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_addr_check(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\j")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_address(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_white_data(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_packet_format(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\n")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_length_config(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_packet_length(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_c_rc_enabled(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\x")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_c_rc_auto_flush(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\0")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_d_c_blocking_filter(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_manchester(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_forword_error_correction(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_preamble_bytes(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_p_qt(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_append_status(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\r\\w")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_frequency_mhz(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_modulation(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_devation(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_channel(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_channel_spacing(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_rx_bandwidth(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\y")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_data_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_power_amp(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_sync_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\1")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_sync_word(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\i")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_addr_check(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\j")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_address(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_white_data(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_packet_format(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\n")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_length_config(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_packet_length(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_c_rc_enabled(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\x")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_c_rc_auto_flush(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\0")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_d_c_blocking_filter(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_manchester(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_forword_error_correction(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_preamble_bytes(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_p_qt(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_append_status(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\t\\w")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_fa_settings_default_view(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\a\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_year(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\c\\y")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_month(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\c\\n")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_day(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\c\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_day_of_week(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\c\\w")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_hours(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\c\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_minutes(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\c\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_seconds(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\c\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_trim(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\c\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_enable_station_mode(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\w\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_s_sid_for_station_mode(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\w\\e")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_password_for_station_mode(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\w\\p")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_enable_ap_mode(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\w\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_a_p_auth(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\w\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_a_p_hide_ssid(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\w\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_s_sid_for_ap(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\w\\g")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_password_for_ap(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\w\\x")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_ble_settings_enable_bt(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\b\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_ble_settings_b_t_terminal(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\b\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_ble_settings_b_t_advert_name(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\b\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_orca_settings_orca_com_over_uart(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\g\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_start_ws_server(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\k\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_w_s_server_port(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\k\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_auth_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\k\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_auth_username(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\k\\u")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_auth_password(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\k\\e")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1fdd_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_listen_only(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\y")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_tx_retry(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_cust_baud(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\f")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_cust_data_baud(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\g")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_termination(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\1")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1api_enabled(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_anapiid(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\j")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\l")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2fdd_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_listen_only(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\n")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_tx_retry(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_cust_baud(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\p")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_cust_data_baud(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\r")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_termination(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2api_enabled(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_l_in_master_en(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\u")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_l_in_baud_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_analog_in_en(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\p\\x")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_general_settings_startup_wasm_script(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\e\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_general_settings_startup_zoom_script(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\e\\b")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_general_settings_default_fpga_script(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\e\\c")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_general_settings_wasm_debug_level(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\e\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch0_input(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\0")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch1_input(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\1")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch2_input(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\2")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch3_input(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\3")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch0_range(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\4")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch1_range(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\5")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch2_range(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\6")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch3_range(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\7")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_data_rate(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\j\\8")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_quiet_threshold(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\n\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_speaker_volume(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\n\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_recording_volume(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\n\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_record_len_sec(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\n\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_system_sounds(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\n\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_brightness_powered(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_brightness_batt_gt70(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_brightness_batt_gt30(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_brightness_batt_lt30(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_battery_timeout(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_powered_timeout(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_wake_on_sound(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_wake_on_move(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\m")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_auto_power_zones(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\m\\o")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_light_show_settings_default_show(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\l\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_light_show_settings_l_ed_strips_enabled(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\l\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_light_show_settings_roku_led_control(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\l\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_light_show_settings_brightness(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\l\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_interface_settings_double_click_ms(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\x\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_settings_home_interface_settings_roku_gui_control(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\s\\x\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_system_enable_battery_stream(ow_device* dev, int32_t enable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\a\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_system_read_otp_info(ow_device* dev, int32_t offset, int32_t length, uint8_t* otp_blob, size_t otp_blob_cap, size_t* otp_blob_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\a\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)offset)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)length)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_bytes(&cur, otp_blob, otp_blob_cap, otp_blob_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_hardware_system_boot_uf2(ow_device* dev, const char* filename)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\a\\u")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_system_device_state(ow_device* dev, char* sd, size_t sd_cap, bool* hoststream, char* activemask, size_t activemask_cap, int32_t* clksyshz)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\a\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__tok_str(&cur, sd, sd_cap)) != OW_OK) return r;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (hoststream) *hoststream = (v != 0); }
    if ((r = ow__tok_str(&cur, activemask, activemask_cap)) != OW_OK) return r;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (clksyshz) *clksyshz = (int32_t)v; }
    return OW_OK;
}

ow_status ow_hardware_system_event_host_streaming(ow_device* dev, int32_t enable, bool* enabled)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\a\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (enabled) *enabled = (v != 0); }
    return OW_OK;
}

ow_status ow_hardware_system_stream_write(ow_device* dev, int32_t dst, const uint8_t* data, size_t data_len, bool* delivered)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\a\\w")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)dst)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data, data_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (delivered) *delivered = (v != 0); }
    return OW_OK;
}

ow_status ow_hardware_system_stream_poll(ow_device* dev, int32_t max, int32_t* frames, int32_t* queued, int32_t* dropped, uint8_t* data, size_t data_cap, size_t* data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\a\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)max)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (frames) *frames = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (queued) *queued = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (dropped) *dropped = (int32_t)v; }
    if ((r = ow__rest_bytes(&cur, data, data_cap, data_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_hardware_system_stream_status(ow_device* dev, int32_t* mtu, int32_t* queued, int32_t* droppedto, int32_t* droppedfrom)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\a\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (mtu) *mtu = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (queued) *queued = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (droppedto) *droppedto = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (droppedfrom) *droppedfrom = (int32_t)v; }
    return OW_OK;
}

ow_status ow_hardware_file_system_change_directory(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_create_directory(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\c")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_remove_file_or_directory(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\r")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_get_file_from_pc(ow_device* dev, const char* path, int32_t size, int32_t crc32)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\f")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)size)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)crc32)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_send_file_to_pc(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\u")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_print_file(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\p")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_create_blank_file(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\b")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_edit_file(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\e")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_rename_or_move_file_directory(ow_device* dev, const char* path, const char* new_path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\n")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, new_path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_list_directory(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\l")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_format_file_system(ow_device* dev, const char* confirm)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\t")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, confirm)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_toggle_sd_card_host_select(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_load_wili_project(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\w")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_set_sd_card_host(ow_device* dev, int32_t host)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)host)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_file_system_begin_file_read(ow_device* dev, uint32_t session, const char* path_hex, int32_t* size)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\0")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path_hex)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (size) *size = (int32_t)v; }
    return OW_OK;
}

ow_status ow_hardware_file_system_begin_file_write(ow_device* dev, uint32_t session, const char* path_hex, int32_t size, uint32_t crc32, bool overwrite, int32_t* size_out)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\1")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path_hex)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)size)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)crc32, 8)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, overwrite)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (size_out) *size_out = (int32_t)v; }
    return OW_OK;
}

ow_status ow_hardware_file_system_read_file_chunk(ow_device* dev, uint32_t session, int32_t offset, int32_t maximum, int32_t* count, char* data, size_t data_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\2")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)offset)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)maximum)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (count) *count = (int32_t)v; }
    if ((r = ow__rest_str(&cur, data, data_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_hardware_file_system_write_file_chunk(ow_device* dev, uint32_t session, int32_t offset, const char* data, int32_t* position)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\3")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)offset)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, data)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (position) *position = (int32_t)v; }
    return OW_OK;
}

ow_status ow_hardware_file_system_finish_file_transfer(ow_device* dev, uint32_t session, int32_t* size, uint32_t* crc32)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\4")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (size) *size = (int32_t)v; }
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (crc32) *crc32 = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_hardware_file_system_cancel_file_transfer(ow_device* dev, uint32_t session)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\x\\5")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_list_zones(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_get_zones(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_set_zone(ow_device* dev, int32_t zone, int32_t on)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)zone)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)on)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_set_zone_mask(ow_device* dev, int32_t mask)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mask)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_get_power_state(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_enable_power_stream(ow_device* dev, int32_t stream_rate_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)stream_rate_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_set_wio_reset_line(ow_device* dev, ow_reset_line_state state)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\w")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)state)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_set_cm0_run_line(ow_device* dev, ow_reset_line_state state)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)state)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_power_management_get_control_lines(ow_device* dev, bool* wio_released, bool* cm0_released, bool* main_rst_high)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\p\\n")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (wio_released) *wio_released = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (cm0_released) *cm0_released = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (main_rst_high) *main_rst_high = (v != 0); }
    return OW_OK;
}

ow_status ow_hardware_display_functions_list_display_apps(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_display_functions_restore_display_firmware(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_display_functions_display_bl_version(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\v")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_display_functions_reset_display_cpu(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\x")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_display_functions_power_cycle_display_cpu(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_display_functions_set_ram_app_arg(ow_device* dev, const char* text)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\g")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, text)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_display_functions_run_app_on_display(ow_device* dev, const char* filename)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_display_functions_run_psram_app(ow_device* dev, const char* filename)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\p")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_hardware_display_functions_load_psram_data(ow_device* dev, const char* filename, uint32_t offset)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "h\\v\\s")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)offset, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_e_sp32_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\e")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_enable_reader(ow_device* dev, int32_t enable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_print_card_info(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_get_status(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_list_saved_cards(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\s\\l")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_load_card(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\s\\o")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_save_current_card(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\s\\s")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_emulate_card(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\s\\e")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_stop_emulation(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\s\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_mifare_classic_read_with_keys(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\m\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_mifare_classic_dictionary_attack(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\m\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_mifare_classic_dump_card(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\m\\u")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_begin(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\k\\b")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_end(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\k\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_field(ow_device* dev, int32_t on)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\k\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)on)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_reg_write(ow_device* dev, uint32_t addr, uint32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\k\\w")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)addr, 1)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)value, 1)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_reg_read(ow_device* dev, uint32_t addr, uint32_t* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\k\\r")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)addr, 1)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (value) *value = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_cmd(ow_device* dev, uint32_t command)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\k\\c")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)command, 1)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_transceive(ow_device* dev, uint32_t flags, int32_t timeout_ms, const uint8_t* tx, size_t tx_len, uint32_t* status, uint8_t* rx, size_t rx_cap, size_t* rx_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\k\\t")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)flags, 1)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)timeout_ms)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, tx, tx_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (status) *status = (uint32_t)v; }
    if ((r = ow__rest_bytes(&cur, rx, rx_cap, rx_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_wireless_nfc_extra_halt_card(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\n\\x\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_enter_bootloader(ow_device* dev, int32_t upgrade_transmission_rate)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)upgrade_transmission_rate)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_enter_application(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_get_i_dand_security(ow_device* dev, int32_t* esp_chip_id, int32_t* version, bool* sb_en, bool* sbar_en, bool* sdm_en, bool* sbrk_1, bool* sbrk_2, bool* sbrk_3, bool* jtag_sw_dis, bool* jtag_hw_dis, bool* usb_dis, bool* flash_enc_en, bool* dcache_dis, bool* icache_dis)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (esp_chip_id) *esp_chip_id = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (version) *version = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (sb_en) *sb_en = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (sbar_en) *sbar_en = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (sdm_en) *sdm_en = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (sbrk_1) *sbrk_1 = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (sbrk_2) *sbrk_2 = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (sbrk_3) *sbrk_3 = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (jtag_sw_dis) *jtag_sw_dis = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (jtag_hw_dis) *jtag_hw_dis = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (usb_dis) *usb_dis = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (flash_enc_en) *flash_enc_en = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (dcache_dis) *dcache_dis = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (icache_dis) *icache_dis = (v != 0); }
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_read_flash_size(ow_device* dev, int32_t* flash_size_bytes)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\k")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (flash_size_bytes) *flash_size_bytes = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_read_esp32mac(ow_device* dev, char* esp32_mac, size_t esp32_mac_cap)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\m")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_str(&cur, esp32_mac, esp32_mac_cap)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_erase_all_flash(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_start_flash_operations(ow_device* dev, uint32_t offset, int32_t size, int32_t block_size)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\f")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)offset, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)size)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)block_size)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_stop_flash_operation(ow_device* dev, bool reboot)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\p")) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, reboot)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_write(ow_device* dev, const uint8_t* flash_data, size_t flash_data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\o")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, flash_data, flash_data_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_read(ow_device* dev, uint32_t offset, int32_t size, uint8_t* data, size_t data_cap, size_t* data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\j")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)offset, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)size)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__rest_bytes(&cur, data, data_cap, data_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_start_write_memory_operations(ow_device* dev, uint32_t offset, int32_t size, int32_t block_size)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\y")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)offset, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)size)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)block_size)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_memory_write(ow_device* dev, const uint8_t* data, size_t data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\0")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data, data_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_stop_memory_operation(ow_device* dev, uint32_t entry_address)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\t")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)entry_address, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_register_write(ow_device* dev, uint32_t offset, uint32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\g")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)offset, 8)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)value, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_register_read(ow_device* dev, uint32_t offset, uint32_t* memory_block)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\c")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)offset, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (memory_block) *memory_block = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_default(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\n")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_from_folder(ow_device* dev, const char* folder)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\w")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, folder)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_status(ow_device* dev, bool* flashing, int32_t* progress, int32_t* partition_index, int32_t* partition_count)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\a\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (flashing) *flashing = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (progress) *progress = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (partition_index) *partition_index = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (partition_count) *partition_count = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_wifi_toggle_events(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_start_access_point(ow_device* dev, const char* ssid, const char* password, int32_t authmode, bool hidessid)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, ssid)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, password)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)authmode)) != OW_OK) return r;
    if ((r = ow__cat_bool(cmd, sizeof cmd, &pos, hidessid)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_discconect_from_station(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_get_connected_devices(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_connect_to_station(ow_device* dev, const char* ssid, const char* password)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\c")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, ssid)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, password)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_discconect_from_station_2(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\f")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_scan_for_access_points(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_get_wif_info(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\p")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_http_get_to_sd(ow_device* dev, const char* url, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\l")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, url)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_http_get_abort(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\x")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_enable_station_mode(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\e\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_s_sid_for_station_mode(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\e\\e")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_password_for_station_mode(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\e\\p")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_enable_ap_mode(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\e\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_a_p_auth(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\e\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_a_p_hide_ssid(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\e\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_s_sid_for_ap(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\e\\g")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_password_for_ap(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\w\\e\\x")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_start_bt_advertising(ow_device* dev, const char* hostname)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, hostname)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_stop_bt_advertising(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_scan_bt_devices(ow_device* dev, int32_t durationms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)durationms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_enable_terminal(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_set_attribute(ow_device* dev, int32_t slot, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)slot)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_get_attribute(ow_device* dev, int32_t slot)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)slot)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_ble_settings_enable_bt(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\b\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_ble_settings_b_t_terminal(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\b\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_ble_settings_b_t_advert_name(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\b\\b\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_enable_ir_stream(ow_device* dev, int32_t enable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_send_ir_data(ow_device* dev, int32_t ir_code)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)ir_code)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_self_test(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_list_dir(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\l")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_list_buttons(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\b")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_send_button(ow_device* dev, int32_t index, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)index)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_save_capture(ow_device* dev, const char* name)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\c")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, name)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_status(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_i_r_carrier(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_ir_i_r_repeat(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\i\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_configure(ow_device* dev, int32_t freq_hz, int32_t sf, int32_t bw_enc, int32_t cr, int32_t power, int32_t preamble, uint8_t sync)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\l\\c")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)freq_hz)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)sf)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)bw_enc)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)cr)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)power)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)preamble)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)sync, 2)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_send_payload(ow_device* dev, const uint8_t* data, size_t data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\l\\s")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data, data_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_rx_enable(ow_device* dev, int32_t mode)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\l\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)mode)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_status(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\l\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_raw_frame(ow_device* dev, uint8_t cmd_, const uint8_t* payload, size_t payload_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\l\\f")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)cmd_, 2)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, payload, payload_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_select_circuit(ow_device* dev, int32_t band)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)band)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_release_circuit(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_read_state(ow_device* dev, int32_t* owner, int32_t* holder, int32_t* band, int32_t* want_v1, int32_t* want_v2, int32_t* have_valid, int32_t* have_v1, int32_t* have_v2, int32_t* lora_paused, int32_t* freq_hz, int32_t* active, int32_t* status, uint8_t* version)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (owner) *owner = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (holder) *holder = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (band) *band = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (want_v1) *want_v1 = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (want_v2) *want_v2 = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (have_valid) *have_valid = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (have_v1) *have_v1 = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (have_v2) *have_v2 = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (lora_paused) *lora_paused = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (freq_hz) *freq_hz = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (active) *active = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (status) *status = (int32_t)v; }
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (version) *version = (uint8_t)v; }
    return OW_OK;
}

ow_status ow_wireless_radio_select_band(ow_device* dev, int32_t band)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)band)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_comm_check(ow_device* dev, uint8_t* version)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (version) *version = (uint8_t)v; }
    return OW_OK;
}

ow_status ow_wireless_radio_set_frequency(ow_device* dev, int32_t freq_hz, int32_t* band)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)freq_hz)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (band) *band = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_radio_read_rssi(ow_device* dev, int32_t* rssi)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (rssi) *rssi = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_radio_carrier(ow_device* dev, int32_t on)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)on)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_rx_enable(ow_device* dev, int32_t on)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)on)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_idle(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\w")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_packet_send(ow_device* dev, int32_t freq_hz, const uint8_t* data, size_t data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\x")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)freq_hz)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data, data_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_packet_rx(ow_device* dev, int32_t on, int32_t freq_hz)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\y")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)on)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)freq_hz)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_packet_read(ow_device* dev, int32_t* rssi, int32_t* seq, uint8_t* data, size_t data_cap, size_t* data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\k")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (rssi) *rssi = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (seq) *seq = (int32_t)v; }
    if ((r = ow__rest_bytes(&cur, data, data_cap, data_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_wireless_radio_capture_start(ow_device* dev, int32_t freq_hz, int32_t preset)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\g")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)freq_hz)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)preset)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_capture_stop(ow_device* dev, int32_t* durations)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\j")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (durations) *durations = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_radio_replay(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\p")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_analyzer(ow_device* dev, int32_t on)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)on)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_spectrum(ow_device* dev, int32_t* peak_freq_hz, int32_t* peak_rssi, uint8_t* bins, size_t bins_cap, size_t* bins_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\n")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (peak_freq_hz) *peak_freq_hz = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (peak_rssi) *peak_rssi = (int32_t)v; }
    if ((r = ow__rest_bytes(&cur, bins, bins_cap, bins_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_wireless_radio_squelch(ow_device* dev, int32_t dbm)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)dbm)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_replay_invert(ow_device* dev, int32_t on)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\v")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)on)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_transmit_sub_file(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\m")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_radio_monitor(ow_device* dev, int32_t on)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\r\\l")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)on)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_rfid_enable_reader(ow_device* dev, int32_t enable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\r")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_rfid_get_status(ow_device* dev, int32_t* state, uint8_t* flags, int32_t* carrier_hz, int32_t* env_min, int32_t* env_max, int32_t* threshold, int32_t* frames, int32_t* tags)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (state) *state = (int32_t)v; }
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (flags) *flags = (uint8_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (carrier_hz) *carrier_hz = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (env_min) *env_min = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (env_max) *env_max = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (threshold) *threshold = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (frames) *frames = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (tags) *tags = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_rfid_read_tag(ow_device* dev, int32_t timeout_ms, int32_t* format, int32_t* modulation, uint8_t* id, size_t id_cap, size_t* id_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)timeout_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (format) *format = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (modulation) *modulation = (int32_t)v; }
    if ((r = ow__rest_bytes(&cur, id, id_cap, id_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_wireless_rfid_stream_tags(ow_device* dev, int32_t enable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\s")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_rfid_clear_stats(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_rfid_tune(ow_device* dev, int32_t param, int32_t value, int32_t* param_out, int32_t* value_out)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)param)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (param_out) *param_out = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (value_out) *value_out = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_rfid_raw_bits(ow_device* dev, int32_t* modulation, int32_t* length, uint8_t* bits, size_t bits_cap, size_t* bits_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\b")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (modulation) *modulation = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (length) *length = (int32_t)v; }
    if ((r = ow__rest_bytes(&cur, bits, bits_cap, bits_len)) != OW_OK) return r;
    return OW_OK;
}

ow_status ow_wireless_rfid_write_tag(ow_device* dev, int32_t block, uint32_t value, int32_t* result, int32_t* block_out)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\w")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)block)) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)value, 1)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (result) *result = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (block_out) *block_out = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_rfid_carrier_info(ow_device* dev, int32_t* carrier_hz, bool* psk_active, int32_t* psk_events, int32_t* poll_count, int32_t* clk_hz, bool* clock_ok, int32_t* env_samples, int32_t* overruns, int32_t* restarts, int32_t* psk_period)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (carrier_hz) *carrier_hz = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (psk_active) *psk_active = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (psk_events) *psk_events = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (poll_count) *poll_count = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (clk_hz) *clk_hz = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (clock_ok) *clock_ok = (v != 0); }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (env_samples) *env_samples = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (overruns) *overruns = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (restarts) *restarts = (int32_t)v; }
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (psk_period) *psk_period = (int32_t)v; }
    return OW_OK;
}

ow_status ow_wireless_rfid_enroll_id(ow_device* dev, const uint8_t* id, size_t id_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\n")) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, id, id_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_rfid_clone_capture(ow_device* dev, int32_t timeout_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\k")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)timeout_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_wireless_rfid_clone_write(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "w\\p\\j")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_launch_script(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_power_cycle_debugger(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_add(ow_device* dev, const char* name)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\i\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, name)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_remove(ow_device* dev, const char* name)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\i\\x")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, name)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_rename(ow_device* dev, const char* name, const char* new_name)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\i\\r")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, name)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, new_name)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_set(ow_device* dev, const char* name, double value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\i\\s")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, name)) != OW_OK) return r;
    if ((r = ow__cat_float(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_get(ow_device* dev, const char* name, char* name_out, size_t name_out_cap, double* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\i\\g")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, name)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__tok_str(&cur, name_out, name_out_cap)) != OW_OK) return r;
    { double v; if ((r = ow__tok_double(&cur, &v)) != OW_OK) return r;
      if (value) *value = v; }
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_wave(ow_device* dev, const char* name, int32_t wave)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\i\\w")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, name)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)wave)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_stream(ow_device* dev, int32_t stream_rate_ms)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\i\\t")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)stream_rate_ms)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_load(ow_device* dev, const char* filepath)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\f\\l")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filepath)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_save(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\f\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_reset(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\f\\r")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_default(ow_device* dev, const char* filepath)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\f\\m")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filepath)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_remove_default(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\f\\x")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_enable_rx_stream(ow_device* dev, int32_t enable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\b\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)enable)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_send_data(ow_device* dev, int32_t delay, const uint8_t* data, size_t data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\b\\w")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)delay)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data, data_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_update_table_data(ow_device* dev, int32_t table_index, int32_t delay, const uint8_t* data, size_t data_len)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\b\\u")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)table_index)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)delay)) != OW_OK) return r;
    if ((r = ow__cat_bytes(cmd, sizeof cmd, &pos, data, data_len)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_enable_schedule_table(ow_device* dev, int32_t number_of_entries)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\b\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)number_of_entries)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_compile_test(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\b\\c")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_run_zio(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\b\\r")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_stop_zio(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\b\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_exec_probe(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\b\\x")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_start(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\w\\c")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_breakpoints(ow_device* dev, const char* pcs)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\w\\j")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, pcs)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_step(ow_device* dev, const char* range)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\w\\e")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, range)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_continue(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\w\\f")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_pause(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\w\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_stop(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\w\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_locals(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\w\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_mem_read(ow_device* dev, const char* addr)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\w\\r")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, addr)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_start(ow_device* dev, const char* path)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\r\\c")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, path)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_breakpoints(ow_device* dev, const char* lines)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\r\\j")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, lines)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_step(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\r\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_continue(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\r\\f")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_pause(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\r\\g")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_stop(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\r\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_locals(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "s\\r\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_apps_launch_app(ow_device* dev, int32_t app_id)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "a\\a")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)app_id)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_apps_run_app(ow_device* dev, const char* filename)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "a\\r")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, filename)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_linux_enable_linux_cpu(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "l\\a")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_linux_open_shell(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "l\\b")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_linux_open_shell_session(ow_device* dev, uint32_t session, uint32_t* session_out)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "l\\c")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { unsigned long v; if ((r = ow__tok_ulong(&cur, &v, 16)) != OW_OK) return r;
      if (session_out) *session_out = (uint32_t)v; }
    return OW_OK;
}

ow_status ow_linux_close_shell_session(ow_device* dev, uint32_t session)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "l\\e")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_linux_write_shell_session(ow_device* dev, uint32_t session, const char* data, int32_t* accepted)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "l\\w")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, data)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (accepted) *accepted = (int32_t)v; }
    return OW_OK;
}

ow_status ow_linux_read_shell_session(ow_device* dev, uint32_t session, int32_t maximum, int32_t* count, char* data, size_t data_cap, bool* running)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "l\\r")) != OW_OK) return r;
    if ((r = ow__cat_hex(cmd, sizeof cmd, &pos, (unsigned long)session, 8)) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)maximum)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (count) *count = (int32_t)v; }
    if ((r = ow__tok_str(&cur, data, data_cap)) != OW_OK) return r;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (running) *running = (v != 0); }
    return OW_OK;
}

ow_status ow_linux_cm0_usb_mode(ow_device* dev, const char* mode, char* mode_out, size_t mode_out_cap, bool* switchable)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "l\\u")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, mode)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    char* cur = resp;
    if ((r = ow__tok_str(&cur, mode_out, mode_out_cap)) != OW_OK) return r;
    { long v; if ((r = ow__tok_long(&cur, &v, 10)) != OW_OK) return r;
      if (switchable) *switchable = (v != 0); }
    return OW_OK;
}

ow_status ow_logger_start(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\s")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_stop(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\e")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_trigger(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\t")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_status(ow_device* dev)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\i")) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_file_format(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\f")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_trigger_mode(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\m")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_trigger_button(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\b")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_trigger_expression(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\x")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_pre_trigger_ms(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\p")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_post_trigger_ms(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\o")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_events(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\v")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_active_instance(ow_device* dev, int32_t value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\n")) != OW_OK) return r;
    if ((r = ow__cat_int(cmd, sizeof cmd, &pos, (long)value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}

ow_status ow_logger_name(ow_device* dev, const char* value)
{
    char cmd[OW_CMD_MAX]; size_t pos = 0;
    char resp[OW_RESP_MAX];
    ow_status r;
    if ((r = ow__cat(cmd, sizeof cmd, &pos, "r\\a")) != OW_OK) return r;
    if ((r = ow__cat_str(cmd, sizeof cmd, &pos, value)) != OW_OK) return r;
    if ((r = ow__call(dev, cmd, resp, sizeof resp)) != OW_OK) return r;
    (void)resp;
    return OW_OK;
}
