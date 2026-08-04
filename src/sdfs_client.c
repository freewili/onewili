#include "sdfs_client.h"
#include "sdfs_codec.h"
#include <string.h>

void sdfs_client_init(sdfs_client_t *c, const sdfs_transport_t *tp) {
    memset(c, 0, sizeof(*c));
    c->tp = tp;
    c->next_req_id = 1;       /* 0 is reserved for "global" status */
    c->timeout_polls = 100000;
}

static uint16_t next_id(sdfs_client_t *c) {
    uint16_t id = c->next_req_id++;
    if (c->next_req_id == 0) c->next_req_id = 1;
    return id;
}

sdfs_status_t sdfs_write(sdfs_client_t *c, const char *path,
                         const uint8_t *data, size_t len, uint8_t flags,
                         uint16_t *out_req_id) {
    uint16_t rid = next_id(c);
    if (out_req_id) *out_req_id = rid;

    size_t path_len = strlen(path);
    if (path_len > SDFS_MAX_PATH) return SDFS_ERR_BAD_REQUEST;

    /* Guard before the rounding division so it cannot overflow size_t for a
     * pathological len near SIZE_MAX; also bounds nchunks to the 16-bit total. */
    if (len > (size_t)0xFFFF * SDFS_MAX_PAYLOAD) return SDFS_ERR_TOO_BIG;

    size_t nchunks = (len + SDFS_MAX_PAYLOAD - 1) / SDFS_MAX_PAYLOAD;
    if (nchunks == 0) nchunks = 1;   /* empty file => one empty LAST chunk */

    size_t off = 0;
    for (size_t seq = 0; seq < nchunks; seq++) {
        size_t remain = len - off;
        uint16_t plen = (uint16_t)(remain < SDFS_MAX_PAYLOAD ? remain : SDFS_MAX_PAYLOAD);

        sdfs_header_t h = {0};
        h.opcode = SDFS_OP_WRITE_CHUNK;
        h.req_id = rid;
        h.seq = (uint16_t)seq;
        h.total = (uint16_t)nchunks;
        h.flags = (seq + 1 == nchunks) ? SDFS_FLAG_LAST : 0;
        if (seq == 0) h.flags |= (flags & SDFS_FLAG_APPEND);
        h.path_len = (seq == 0) ? (uint8_t)path_len : 0;
        h.payload_len = plen;

        int n = sdfs_encode(&h, (seq == 0) ? path : NULL,
                            plen ? &data[off] : NULL, c->buf, sizeof(c->buf));
        if (n <= 0) return SDFS_ERR_BAD_REQUEST;
        if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;
        off += plen;
    }
    return SDFS_OK;
}

sdfs_status_t sdfs_read_begin(sdfs_client_t *c, sdfs_read_ctx_t *rc,
                              const char *path, uint8_t *dst, size_t cap) {
    memset(rc, 0, sizeof(*rc));
    rc->req_id = next_id(c);
    rc->dst = dst;
    rc->cap = cap;
    rc->status = SDFS_PENDING;

    size_t path_len = strlen(path);
    if (path_len > SDFS_MAX_PATH) return SDFS_ERR_BAD_REQUEST;

    sdfs_header_t h = {0};
    h.opcode = SDFS_OP_READ_REQ;
    h.req_id = rc->req_id;
    h.path_len = (uint8_t)path_len;
    int n = sdfs_encode(&h, path, NULL, c->buf, sizeof(c->buf));
    if (n <= 0) return SDFS_ERR_BAD_REQUEST;
    if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;
    return SDFS_OK;
}

sdfs_status_t sdfs_read_poll(sdfs_client_t *c, sdfs_read_ctx_t *rc) {
    if (rc->done) return rc->status;

    size_t len = 0;
    int r = c->tp->recv(c->tp->ctx, c->buf, sizeof(c->buf), &len);
    if (r < 0) { rc->done = 1; rc->status = SDFS_ERR_IO; return rc->status; }
    if (r == 0) {
        if (++rc->polls >= c->timeout_polls) {
            rc->done = 1; rc->status = SDFS_ERR_TIMEOUT;
        }
        return rc->done ? rc->status : SDFS_PENDING;
    }
    rc->polls = 0;

    sdfs_header_t h; const char *p; const uint8_t *pay;
    if (sdfs_decode(c->buf, len, &h, &p, &pay) != 0) return SDFS_PENDING;
    if (h.req_id != rc->req_id) return SDFS_PENDING;   /* not ours */

    if (h.opcode == SDFS_OP_NACK) {
        rc->done = 1;
        rc->status = (sdfs_status_t)(h.payload_len ? pay[0] : SDFS_ERR_IO);
        return rc->status;
    }
    if (h.opcode == SDFS_OP_READ_CHUNK) {
        if (h.seq != rc->next_seq) {
            rc->done = 1; rc->status = SDFS_ERR_BAD_REQUEST; return rc->status;
        }
        if (rc->got + h.payload_len > rc->cap) {
            rc->done = 1; rc->status = SDFS_ERR_TOO_BIG; return rc->status;
        }
        if (h.payload_len) memcpy(&rc->dst[rc->got], pay, h.payload_len);
        rc->got += h.payload_len;
        rc->next_seq++;
        if ((h.flags & SDFS_FLAG_LAST) || (h.total && rc->next_seq >= h.total)) {
            rc->done = 1; rc->status = SDFS_OK;
        }
        return rc->done ? rc->status : SDFS_PENDING;
    }
    return SDFS_PENDING;   /* ignore unrelated opcodes */
}

sdfs_status_t sdfs_read(sdfs_client_t *c, const char *path,
                        uint8_t *dst, size_t cap, size_t *out_len) {
    sdfs_read_ctx_t rc;
    sdfs_status_t s = sdfs_read_begin(c, &rc, path, dst, cap);
    if (s != SDFS_OK) return s;
    do { s = sdfs_read_poll(c, &rc); } while (s == SDFS_PENDING);
    if (s == SDFS_OK && out_len) *out_len = rc.got;
    return s;
}

sdfs_status_t sdfs_status(sdfs_client_t *c, uint16_t req_id,
                          sdfs_status_info_t *out) {
    sdfs_header_t h = {0};
    h.opcode = SDFS_OP_STATUS_REQ;
    h.req_id = req_id;
    int n = sdfs_encode(&h, NULL, NULL, c->buf, sizeof(c->buf));
    if (n <= 0) return SDFS_ERR_BAD_REQUEST;
    if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;

    uint32_t polls = 0;
    for (;;) {
        size_t len = 0;
        int r = c->tp->recv(c->tp->ctx, c->buf, sizeof(c->buf), &len);
        if (r < 0) return SDFS_ERR_IO;
        if (r == 0) {
            if (++polls >= c->timeout_polls) return SDFS_ERR_TIMEOUT;
            continue;
        }
        sdfs_header_t rh; const char *p; const uint8_t *pay;
        if (sdfs_decode(c->buf, len, &rh, &p, &pay) != 0) continue;
        /* Only the r==0 (no-frame) path advances polls; stray frames are
         * silently skipped so timeout is driven by quiet-line idle (mirrors
         * sdfs_read_poll). */
        if (rh.opcode != SDFS_OP_STATUS_RESP || rh.req_id != req_id) continue;
        if (rh.payload_len != 7) return SDFS_ERR_IO;
        if (out) {
            out->status       = (sdfs_status_t)pay[0];
            out->card_present = pay[1];
            out->mounted      = pay[2];
            out->bytes        = (uint32_t)pay[3] | ((uint32_t)pay[4] << 8) |
                                ((uint32_t)pay[5] << 16) | ((uint32_t)pay[6] << 24);
        }
        return SDFS_OK;
    }
}

sdfs_status_t sdfs_stat(sdfs_client_t *c, const char *path,
                        int *is_dir, uint32_t *size) {
    size_t path_len = strlen(path);
    if (path_len > SDFS_MAX_PATH) return SDFS_ERR_BAD_REQUEST;

    sdfs_header_t h = {0};
    h.opcode = SDFS_OP_STAT_REQ;
    h.req_id = next_id(c);
    h.path_len = (uint8_t)path_len;
    int n = sdfs_encode(&h, path, NULL, c->buf, sizeof(c->buf));
    if (n <= 0) return SDFS_ERR_BAD_REQUEST;
    if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;

    uint32_t polls = 0;
    for (;;) {
        size_t len = 0;
        int r = c->tp->recv(c->tp->ctx, c->buf, sizeof(c->buf), &len);
        if (r < 0) return SDFS_ERR_IO;
        if (r == 0) { if (++polls >= c->timeout_polls) return SDFS_ERR_TIMEOUT; continue; }
        sdfs_header_t rh; const char *p; const uint8_t *pay;
        if (sdfs_decode(c->buf, len, &rh, &p, &pay) != 0) continue;
        if (rh.req_id != h.req_id) continue;
        if (rh.opcode == SDFS_OP_NACK)
            return (sdfs_status_t)(rh.payload_len ? pay[0] : SDFS_ERR_IO);
        if (rh.opcode != SDFS_OP_STAT_RESP || rh.payload_len != 6) continue;
        if (is_dir) *is_dir = pay[1];
        if (size)   *size   = (uint32_t)pay[2] | ((uint32_t)pay[3] << 8) |
                              ((uint32_t)pay[4] << 16) | ((uint32_t)pay[5] << 24);
        return (sdfs_status_t)pay[0];
    }
}

/* Send one request (opcode/path/payload) and block for its RESULT (or NACK). */
static sdfs_status_t await_result(sdfs_client_t *c, uint8_t opcode,
                                  const char *path, uint8_t path_len,
                                  const uint8_t *payload, uint16_t payload_len) {
    sdfs_header_t h = {0};
    h.opcode = opcode;
    h.req_id = next_id(c);
    h.path_len = path_len;
    h.payload_len = payload_len;
    int n = sdfs_encode(&h, path_len ? path : NULL,
                        payload_len ? payload : NULL, c->buf, sizeof(c->buf));
    if (n <= 0) return SDFS_ERR_BAD_REQUEST;
    if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;

    uint32_t polls = 0;
    for (;;) {
        size_t len = 0;
        int r = c->tp->recv(c->tp->ctx, c->buf, sizeof(c->buf), &len);
        if (r < 0) return SDFS_ERR_IO;
        if (r == 0) { if (++polls >= c->timeout_polls) return SDFS_ERR_TIMEOUT; continue; }
        sdfs_header_t rh; const char *p; const uint8_t *pay;
        if (sdfs_decode(c->buf, len, &rh, &p, &pay) != 0) continue;
        if (rh.req_id != h.req_id) continue;
        if (rh.opcode == SDFS_OP_NACK)
            return (sdfs_status_t)(rh.payload_len ? pay[0] : SDFS_ERR_IO);
        if (rh.opcode == SDFS_OP_RESULT && rh.payload_len == 1)
            return (sdfs_status_t)pay[0];
        /* other opcodes: ignore, keep waiting */
    }
}

sdfs_status_t sdfs_mkdir(sdfs_client_t *c, const char *path) {
    size_t pl = strlen(path);
    if (pl > SDFS_MAX_PATH) return SDFS_ERR_BAD_REQUEST;
    return await_result(c, SDFS_OP_MKDIR_REQ, path, (uint8_t)pl, NULL, 0);
}

sdfs_status_t sdfs_remove(sdfs_client_t *c, const char *path) {
    size_t pl = strlen(path);
    if (pl > SDFS_MAX_PATH) return SDFS_ERR_BAD_REQUEST;
    return await_result(c, SDFS_OP_REMOVE_REQ, path, (uint8_t)pl, NULL, 0);
}

sdfs_status_t sdfs_rename(sdfs_client_t *c, const char *oldp, const char *newp) {
    size_t ol = strlen(oldp), nl = strlen(newp);
    if (ol > SDFS_MAX_PATH || nl > SDFS_MAX_PAYLOAD) return SDFS_ERR_BAD_REQUEST;
    return await_result(c, SDFS_OP_RENAME_REQ, oldp, (uint8_t)ol,
                        (const uint8_t *)newp, (uint16_t)nl);
}

sdfs_status_t sdfs_list(sdfs_client_t *c, const char *path,
                        sdfs_list_cb cb, void *user) {
    size_t path_len = strlen(path);
    if (path_len > SDFS_MAX_PATH) return SDFS_ERR_BAD_REQUEST;

    sdfs_header_t h = {0};
    h.opcode = SDFS_OP_LIST_REQ;
    h.req_id = next_id(c);
    h.path_len = (uint8_t)path_len;
    int n = sdfs_encode(&h, path, NULL, c->buf, sizeof(c->buf));
    if (n <= 0) return SDFS_ERR_BAD_REQUEST;
    if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;

    uint16_t next_seq = 0;
    uint32_t polls = 0;
    for (;;) {
        size_t len = 0;
        int r = c->tp->recv(c->tp->ctx, c->buf, sizeof(c->buf), &len);
        if (r < 0) return SDFS_ERR_IO;
        if (r == 0) { if (++polls >= c->timeout_polls) return SDFS_ERR_TIMEOUT; continue; }
        polls = 0;
        sdfs_header_t rh; const char *p; const uint8_t *pay;
        if (sdfs_decode(c->buf, len, &rh, &p, &pay) != 0) continue;
        if (rh.req_id != h.req_id) continue;
        if (rh.opcode == SDFS_OP_NACK)
            return (sdfs_status_t)(rh.payload_len ? pay[0] : SDFS_ERR_IO);
        if (rh.opcode != SDFS_OP_LIST_CHUNK) continue;
        if (rh.seq != next_seq) return SDFS_ERR_BAD_REQUEST;   /* lost/reordered chunk */
        next_seq++;

        /* Unpack packed entries: [flags:1][size:4][name_len:1][name] */
        uint16_t off = 0;
        while (off + 6u <= rh.payload_len) {
            uint8_t  flags = pay[off];
            uint32_t size  = (uint32_t)pay[off+1] | ((uint32_t)pay[off+2] << 8) |
                             ((uint32_t)pay[off+3] << 16) | ((uint32_t)pay[off+4] << 24);
            uint8_t  nlen  = pay[off+5];
            if (off + 6u + nlen > rh.payload_len) break;        /* malformed; stop */
            char name[SDFS_MAX_PATH + 1];
            uint8_t cp = nlen < SDFS_MAX_PATH ? nlen : (uint8_t)SDFS_MAX_PATH;
            memcpy(name, &pay[off+6], cp);
            name[cp] = '\0';
            if (cb) cb(name, (flags & 1) ? 1 : 0, size, user);
            off = (uint16_t)(off + 6 + nlen);
        }
        if (rh.flags & SDFS_FLAG_LAST) return SDFS_OK;
    }
}

sdfs_status_t sdfs_open(sdfs_client_t *c, const char *path, uint8_t mode, uint8_t *out_handle) {
    size_t pl = strlen(path);
    if (pl > SDFS_MAX_PATH) return SDFS_ERR_BAD_REQUEST;
    if (out_handle) *out_handle = 0xFF;
    sdfs_header_t h = {0};
    h.opcode = SDFS_OP_OPEN_REQ; h.req_id = next_id(c);
    h.path_len = (uint8_t)pl; h.payload_len = 1;
    uint8_t m = mode;
    int n = sdfs_encode(&h, path, &m, c->buf, sizeof(c->buf));
    if (n <= 0) return SDFS_ERR_BAD_REQUEST;
    if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;
    uint32_t polls = 0;
    for (;;) {
        size_t len = 0; int r = c->tp->recv(c->tp->ctx, c->buf, sizeof(c->buf), &len);
        if (r < 0) return SDFS_ERR_IO;
        if (r == 0) { if (++polls >= c->timeout_polls) return SDFS_ERR_TIMEOUT; continue; }
        sdfs_header_t rh; const char *p; const uint8_t *pay;
        if (sdfs_decode(c->buf, len, &rh, &p, &pay) != 0) continue;
        if (rh.req_id != h.req_id) continue;
        if (rh.opcode == SDFS_OP_NACK) return (sdfs_status_t)(rh.payload_len ? pay[0] : SDFS_ERR_IO);
        if (rh.opcode != SDFS_OP_OPEN_RESP || rh.payload_len != 2) continue;
        if (out_handle) *out_handle = pay[1];
        return (sdfs_status_t)pay[0];
    }
}

sdfs_status_t sdfs_hread(sdfs_client_t *c, uint8_t handle, uint8_t *dst,
                         uint32_t len, uint32_t *got) {
    if (got) *got = 0;
    uint8_t req[5] = { handle, (uint8_t)len, (uint8_t)(len>>8), (uint8_t)(len>>16), (uint8_t)(len>>24) };
    sdfs_header_t h = {0};
    h.opcode = SDFS_OP_HREAD_REQ; h.req_id = next_id(c); h.payload_len = 5;
    int n = sdfs_encode(&h, NULL, req, c->buf, sizeof(c->buf));
    if (n <= 0) return SDFS_ERR_BAD_REQUEST;
    if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;
    uint32_t polls = 0; uint16_t next_seq = 0; uint32_t total = 0;
    for (;;) {
        size_t rl = 0; int r = c->tp->recv(c->tp->ctx, c->buf, sizeof(c->buf), &rl);
        if (r < 0) return SDFS_ERR_IO;
        if (r == 0) { if (++polls >= c->timeout_polls) return SDFS_ERR_TIMEOUT; continue; }
        polls = 0;
        sdfs_header_t rh; const char *p; const uint8_t *pay;
        if (sdfs_decode(c->buf, rl, &rh, &p, &pay) != 0) continue;
        if (rh.req_id != h.req_id) continue;
        if (rh.opcode == SDFS_OP_NACK) return (sdfs_status_t)(rh.payload_len ? pay[0] : SDFS_ERR_IO);
        if (rh.opcode != SDFS_OP_READ_CHUNK) continue;
        if (rh.seq != next_seq) return SDFS_ERR_BAD_REQUEST;
        next_seq++;
        if (total + rh.payload_len > len) return SDFS_ERR_TOO_BIG;
        if (rh.payload_len) memcpy(&dst[total], pay, rh.payload_len);
        total += rh.payload_len;
        if (rh.flags & SDFS_FLAG_LAST) { if (got) *got = total; return SDFS_OK; }
    }
}

sdfs_status_t sdfs_hwrite(sdfs_client_t *c, uint8_t handle, const uint8_t *data, uint32_t len) {
    /* One or more HWRITE_CHUNKs, each [handle][<=MAX_PAYLOAD-1 data]. The
     * final chunk is flagged LAST and acknowledged by the server, providing
     * one backpressure point per caller batch. */
    uint32_t off = 0;
    const uint32_t cap = (uint32_t)SDFS_MAX_PAYLOAD - 1u;
    uint16_t final_rid = 0;
    do {
        uint32_t take = (len - off) < cap ? (len - off) : cap;
        uint8_t body[SDFS_MAX_PAYLOAD];
        body[0] = handle;
        if (take) memcpy(&body[1], &data[off], take);
        sdfs_header_t h = {0};
        h.opcode = SDFS_OP_HWRITE_CHUNK; h.req_id = next_id(c);
        if (off + take >= len) {
            h.flags = SDFS_FLAG_LAST;
            final_rid = h.req_id;
        }
        h.payload_len = (uint16_t)(1 + take);
        int n = sdfs_encode(&h, NULL, body, c->buf, sizeof(c->buf));
        if (n <= 0) return SDFS_ERR_BAD_REQUEST;
        if (c->tp->send(c->tp->ctx, c->buf, (size_t)n) < 0) return SDFS_ERR_IO;
        off += take;
    } while (off < len);

    uint32_t polls = 0;
    for (;;) {
        size_t rl = 0;
        int r = c->tp->recv(c->tp->ctx, c->buf, sizeof(c->buf), &rl);
        if (r < 0) return SDFS_ERR_IO;
        if (r == 0) {
            if (++polls >= c->timeout_polls) return SDFS_ERR_TIMEOUT;
            continue;
        }
        sdfs_header_t rh; const char *p; const uint8_t *pay;
        if (sdfs_decode(c->buf, rl, &rh, &p, &pay) != 0) continue;
        if (rh.req_id != final_rid) continue;
        if (rh.opcode == SDFS_OP_NACK)
            return (sdfs_status_t)(rh.payload_len ? pay[0] : SDFS_ERR_IO);
        if (rh.opcode == SDFS_OP_RESULT && rh.payload_len == 1)
            return (sdfs_status_t)pay[0];
    }
}

sdfs_status_t sdfs_seek(sdfs_client_t *c, uint8_t handle, uint32_t offset) {
    uint8_t req[5] = { handle, (uint8_t)offset, (uint8_t)(offset>>8), (uint8_t)(offset>>16), (uint8_t)(offset>>24) };
    return await_result(c, SDFS_OP_SEEK_REQ, NULL, 0, req, 5);
}

sdfs_status_t sdfs_hclose(sdfs_client_t *c, uint8_t handle, uint32_t expected_bytes) {
    /* payload = [handle:1][expected_bytes:4 LE]; expected 0xFFFFFFFF skips the
     * server's HWRITE byte-count integrity check (use for stranded-handle closes). */
    uint8_t req[5] = { handle,
                       (uint8_t)(expected_bytes & 0xFF),
                       (uint8_t)((expected_bytes >> 8) & 0xFF),
                       (uint8_t)((expected_bytes >> 16) & 0xFF),
                       (uint8_t)((expected_bytes >> 24) & 0xFF) };
    return await_result(c, SDFS_OP_HCLOSE_REQ, NULL, 0, req, sizeof(req));
}
