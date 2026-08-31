/* Loopback transport: a scripted stand-in for a FreeWili, so the marshalling
 * can be tested without hardware.
 *
 * Compiled only when -DONEWILI_LV_LOOPBACK=ON. It is NOT in the shipped DLL:
 * the release build has no owlv_test_* exports and does not link this file.
 * The 540 command forwarders it exercises are byte-identical in both builds,
 * which is the point -- this gives full coverage of argument encoding and
 * output marshalling against code that ships unchanged.
 *
 * A test writes the response body it wants, calls a command, and then asserts
 * on both halves of the round trip: the wire line the encoder produced, and
 * the values the decoder handed back.
 *
 *     owlv_test_set_response(s, "DEADBEEF", 1);
 *     owlv_io_gpio_read_all(s, &bits);         -> bits == 0xDEADBEEF
 *     owlv_test_last_command(s, cmd, 64);      -> "i\g\u"
 */

#include "onewili_lv_internal.h"

#include <stdio.h>
#include <string.h>

#define OWLV_MOCK_RX 16384

typedef struct owlv_mock {
    int    used;
    char   partial[OW_CMD_MAX];   /* command bytes seen so far */
    size_t partial_len;
    char   last_cmd[OW_CMD_MAX];  /* last complete command, 0x02 stripped */
    char   body[OW_RESP_MAX];     /* what the next response frame carries */
    int    ok;
    char   rx[OWLV_MOCK_RX];      /* bytes waiting for the reader */
    size_t rx_len, rx_pos;
} owlv_mock;

static owlv_mock g_mocks[OWLV_MAX_SESSIONS];

static int owlv_mock__str_out(const char* src, char* buf, int cap)
{
    size_t n;
    if (!buf || cap <= 0) return OWLV_OK;
    if (!src) src = "";
    n = strlen(src);
    if (n + 1 > (size_t)cap) {
        memcpy(buf, src, (size_t)cap - 1);
        buf[cap - 1] = 0;
        return OWLV_ERR_BUFFER;
    }
    memcpy(buf, src, n + 1);
    return OWLV_OK;
}

static void owlv_mock__emit(owlv_mock* m, const char* text)
{
    size_t n = strlen(text);
    if (m->rx_len + n > sizeof m->rx) return;   /* test overran the buffer */
    memcpy(m->rx + m->rx_len, text, n);
    m->rx_len += n;
}

/* Turn a completed command line into the response frame the firmware would
 * send: [<path> <hex timestamp> <seq> <body> <ok>]. Only the first token of
 * the command is the path -- arguments must not shift the body, which the
 * parser locates by counting the first three spaces. */
static void owlv_mock__respond(owlv_mock* m)
{
    char frame[OW_RESP_MAX + 128];
    char path[OW_CMD_MAX];
    size_t i;

    if (!m->last_cmd[0]) return;     /* bare 0x02 reset: the device stays quiet */

    for (i = 0; m->last_cmd[i] && m->last_cmd[i] != ' ' && i + 1 < sizeof path; ++i)
        path[i] = m->last_cmd[i];
    path[i] = 0;

    snprintf(frame, sizeof frame, "[%s 0000000000000000 1 %s %d]\n",
             path, m->body, m->ok ? 1 : 0);
    owlv_mock__emit(m, frame);
}

static int owlv_mock__write(void* ctx, const uint8_t* data, size_t len)
{
    owlv_mock* m = (owlv_mock*)ctx;
    size_t i;
    for (i = 0; i < len; ++i) {
        char c = (char)data[i];
        if (c == '\n') {
            m->partial[m->partial_len] = 0;
            snprintf(m->last_cmd, sizeof m->last_cmd, "%s", m->partial);
            m->partial_len = 0;
            owlv_mock__respond(m);
            continue;
        }
        if (c == 0x02) continue;     /* reset-to-root prefix, not part of the line */
        if (m->partial_len + 1 < sizeof m->partial)
            m->partial[m->partial_len++] = c;
    }
    return (int)len;
}

static int owlv_mock__read(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms)
{
    owlv_mock* m = (owlv_mock*)ctx;
    size_t avail = m->rx_len - m->rx_pos;
    size_t n;
    (void)timeout_ms;
    if (avail == 0) {
        m->rx_len = m->rx_pos = 0;
        return 0;                    /* a timeout, exactly as a real port reports */
    }
    n = (avail < cap) ? avail : cap;
    memcpy(buf, m->rx + m->rx_pos, n);
    m->rx_pos += n;
    if (m->rx_pos == m->rx_len) m->rx_len = m->rx_pos = 0;
    return (int)n;
}

/* The mock behind a session, or NULL when that session is a real serial port.
 * Identified by the transport context pointing into our own table. */
static owlv_mock* owlv_mock__of(owlv_session* s)
{
    owlv_mock* m = (owlv_mock*)s->dev.t.ctx;
    if (m >= g_mocks && m < g_mocks + OWLV_MAX_SESSIONS) return m;
    return NULL;
}

/* ── the owlv_test_* ABI ───────────────────────────────────────────────── */

int owlv_test_open(int* session)
{
    owlv_mock* m = NULL;
    ow_transport t;
    int i, r;

    if (session) *session = 0;
    for (i = 0; i < OWLV_MAX_SESSIONS; ++i)
        if (!g_mocks[i].used) { m = &g_mocks[i]; break; }
    if (!m) return OWLV_ERR_LIMIT;

    memset(m, 0, sizeof *m);
    m->used = 1;
    m->ok = 1;

    t.ctx   = m;
    t.write = owlv_mock__write;
    t.read  = owlv_mock__read;

    r = owlv__open_with(&t, "loopback", NULL, session);
    if (r != OWLV_OK) m->used = 0;
    return r;
}

int owlv_test_close(int session)
{
    owlv_session* s = owlv__acquire(session);
    owlv_mock* m;
    if (!s) return OWLV_ERR_SESSION;
    m = owlv_mock__of(s);
    owlv__release(s, OW_OK, NULL);
    if (m) m->used = 0;
    return owlv_close(session);
}

int owlv_test_set_response(int session, const char* body, int ok)
{
    owlv_session* s = owlv__acquire(session);
    owlv_mock* m;
    if (!s) return OWLV_ERR_SESSION;
    m = owlv_mock__of(s);
    if (!m) { owlv__release(s, OW_OK, NULL); return OWLV_ERR_STATE; }
    snprintf(m->body, sizeof m->body, "%s", body ? body : "");
    m->ok = ok;
    owlv__release(s, OW_OK, NULL);
    return OWLV_OK;
}

int owlv_test_last_command(int session, char* buf, int cap)
{
    owlv_session* s = owlv__acquire(session);
    owlv_mock* m;
    int r;
    if (!s) return OWLV_ERR_SESSION;
    m = owlv_mock__of(s);
    if (!m) { owlv__release(s, OW_OK, NULL); return OWLV_ERR_STATE; }
    r = owlv_mock__str_out(m->last_cmd, buf, cap);
    owlv__release(s, OW_OK, NULL);
    return r;
}

/* Queue a spontaneous event. Real device events are full frames --
 * "[*<name> <hexTimestampNs> <seq> <payload> <ok>]", the same shape as a
 * command response -- and the line reassembler in the C package relies on
 * that trailing ok token to tell a complete frame from a payload that merely
 * happened to contain a newline. Synthesising the frame here rather than
 * taking a raw line keeps a test from accidentally asserting against a shape
 * the firmware never sends. */
int owlv_test_push_event(int session, const char* name, const char* payload)
{
    owlv_session* s = owlv__acquire(session);
    owlv_mock* m;
    char frame[OW_RESP_MAX + 128];
    if (!s) return OWLV_ERR_SESSION;
    m = owlv_mock__of(s);
    if (!m) { owlv__release(s, OW_OK, NULL); return OWLV_ERR_STATE; }
    if (name && name[0]) {
        snprintf(frame, sizeof frame, "[*%s 00000000DECAFBAD 7 %s 1]\n",
                 name, payload ? payload : "");
        owlv_mock__emit(m, frame);
    }
    owlv__release(s, OW_OK, NULL);
    return OWLV_OK;
}
