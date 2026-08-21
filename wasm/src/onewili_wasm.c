/* WASM ow_transport shim over the ow_exec host import - generated. Do not edit. */
#include "onewili_wasm.h"

typedef struct oww_ctx {
    char   cmd[OW_CMD_MAX];
    size_t cmd_len;
    char   resp[OW_RESP_MAX];
    size_t resp_len;   /* framed response length incl. trailing '\n', 0 if none */
    size_t resp_pos;   /* read cursor into resp */
} oww_ctx;

static oww_ctx g_oww;

static int oww_write(void* c, const uint8_t* data, size_t len) {
    oww_ctx* x = (oww_ctx*)c;
    size_t i;
    /* A new command begins once the previous response has been fully drained. */
    if (x->resp_len && x->resp_pos >= x->resp_len) {
        x->cmd_len = 0; x->resp_len = 0; x->resp_pos = 0;
    }
    for (i = 0; i < len; ++i) {
        uint8_t b = data[i];
        if (b == 0x02) continue;              /* drop the reset-nav prefix */
        if (b == '\n') {                      /* command complete -> run it now */
            int n;
            if (x->cmd_len == 0) {            /* bare reset (open/close): no exec */
                x->resp_len = 0; x->resp_pos = 0;
                return (int)len;
            }
            x->cmd[x->cmd_len] = 0;
            n = ow_exec(x->cmd, (int)x->cmd_len, x->resp, (int)(sizeof x->resp - 1));
            if (n < 0) n = 0;                 /* failure -> empty response */
            if ((size_t)n >= sizeof x->resp) n = (int)(sizeof x->resp - 1);
            x->resp[n] = '\n';                /* ow__read_line needs a terminator */
            x->resp_len = (size_t)n + 1;
            x->resp_pos = 0;
            x->cmd_len  = 0;
            return (int)len;
        }
        if (x->cmd_len + 1 < sizeof x->cmd) x->cmd[x->cmd_len++] = (char)b;
    }
    return (int)len;
}

static int oww_read(void* c, uint8_t* buf, size_t cap, uint32_t timeout_ms) {
    oww_ctx* x = (oww_ctx*)c;
    size_t avail, n, i;
    (void)timeout_ms;
    if (x->resp_len == 0 || x->resp_pos >= x->resp_len) return 0;
    avail = x->resp_len - x->resp_pos;
    n = avail < cap ? avail : cap;
    for (i = 0; i < n; ++i) buf[i] = (uint8_t)x->resp[x->resp_pos + i];
    x->resp_pos += n;
    return (int)n;
}

ow_status ow_open_wasm(ow_device* dev) {
    static ow_transport t;
    g_oww.cmd_len = 0; g_oww.resp_len = 0; g_oww.resp_pos = 0;
    t.ctx = &g_oww;
    t.write = oww_write;
    t.read = oww_read;
    return ow_open(dev, &t);
}
