/* Pipelined write_canfd over OneWili; see onewili_fast.h. */
#include "onewili_fast.h"
#include <stdio.h>
#include <string.h>

#define OWF_CHUNK 56u   /* text bytes per link frame (onewili_fwgui.c OWFW_CHUNK_MAX) */
#define OWF_CHUNK_OVERHEAD 9u

static const char HEX[] = "0123456789ABCDEF";

void ow_fast_tx_init(ow_fast_tx* t, ow_device* dev, uint32_t max_in_flight,
                     ow_fast_result_fn on_result, void* user) {
    memset(t, 0, sizeof *t);
    t->dev = dev;
    if (max_in_flight < 1) max_in_flight = 1;
    if (max_in_flight > OW_FAST_MAX_IN_FLIGHT) max_in_flight = OW_FAST_MAX_IN_FLIGHT;
    t->max_in_flight = max_in_flight;
    t->max_bytes = OW_FAST_DEFAULT_MAX_BYTES;
    t->on_result = on_result;
    t->user = user;
}

static uint32_t owf_wire_bytes(uint32_t text_len) {
    uint32_t total = text_len + 2;                       /* 0x02 prefix + newline */
    uint32_t chunks = (total + OWF_CHUNK - 1) / OWF_CHUNK;
    return total + chunks * OWF_CHUNK_OVERHEAD;
}

int ow_fast_canfd_write(ow_fast_tx* t, int32_t channel, uint32_t arb_id, int can_fd,
                        int xtd_id, const uint8_t* data, size_t len, uint32_t token) {
    char cmd[32 + 64 * 3 + 4];
    int n;
    uint32_t wire;
    if (!t || !t->dev || len > 64) return -(int)OW_ERR_ARG;
    if (t->in_flight >= t->max_in_flight) return 0;
    n = snprintf(cmd, sizeof cmd, "i\\c\\w %ld %08lX %d %d",
                 (long)channel, (unsigned long)arb_id, can_fd ? 1 : 0, xtd_id ? 1 : 0);
    if (n < 0) return -(int)OW_ERR_ARG;
    for (size_t i = 0; i < len; i++) {
        cmd[n++] = ' ';
        cmd[n++] = HEX[data[i] >> 4];
        cmd[n++] = HEX[data[i] & 15];
    }
    cmd[n] = 0;
    wire = owf_wire_bytes((uint32_t)n);
    if (t->in_flight && t->bytes_in_flight + wire > t->max_bytes) return 0;
    {
        int w = ow_raw_send(t->dev, cmd);
        if (w < 0) { t->io_errors++; return w; }
    }
    t->q_bytes[t->q_w] = wire;
    t->q_token[t->q_w] = token;
    t->q_w = (t->q_w + 1) % OW_FAST_MAX_IN_FLIGHT;
    t->in_flight++;
    t->bytes_in_flight += wire;
    t->sent++;
    return 1;
}

int ow_fast_tx_reap(ow_fast_tx* t, uint32_t timeout_ms) {
    static char resp[OW_RESP_MAX];
    int reaped = 0;
    if (!t || !t->dev) return -(int)OW_ERR_ARG;
    while (t->in_flight) {
        int ok = 0;
        ow_status r = ow_raw_next_response(t->dev, resp, sizeof resp, &ok, reaped ? 0 : timeout_ms);
        if (r == OW_ERR_TIMEOUT) break;
        if (r == OW_ERR_IO) { t->io_errors++; return -(int)OW_ERR_IO; }
        if (r != OW_OK) { t->proto_errors++; ok = 0; }   /* a frame arrived but did not parse: still an answer */
        {
            uint32_t token = t->q_token[t->q_r];
            t->bytes_in_flight -= t->q_bytes[t->q_r];
            t->q_r = (t->q_r + 1) % OW_FAST_MAX_IN_FLIGHT;
            t->in_flight--;
            if (ok) t->ok++; else t->failed++;
            reaped++;
            if (t->on_result) t->on_result(t->user, token, ok);
        }
    }
    return reaped;
}

void ow_fast_tx_reset(ow_fast_tx* t) {
    if (!t) return;
    t->in_flight = t->bytes_in_flight = 0;
    t->q_r = t->q_w = 0;
    ow_raw_stash_clear();
}

ow_status ow_fast_tx_drain(ow_fast_tx* t, uint32_t timeout_ms) {
    if (!t) return OW_ERR_ARG;
    while (t->in_flight) {
        int r = ow_fast_tx_reap(t, timeout_ms);
        if (r < 0) return (ow_status)(-r);
        if (r == 0) return OW_ERR_TIMEOUT;
    }
    return OW_OK;
}
