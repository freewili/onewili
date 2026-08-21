#include "sdfs_codec.h"
#include <string.h>

static void put_u16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)(v & 0xFF);
    p[1] = (uint8_t)((v >> 8) & 0xFF);
}
static uint16_t get_u16(const uint8_t *p) {
    return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
}

int sdfs_encode(const sdfs_header_t *h, const char *path,
                const uint8_t *payload, uint8_t *out, size_t out_cap) {
    if (!h || !out) return -1;
    if (h->path_len > SDFS_MAX_PATH) return -1;
    if (h->payload_len > SDFS_MAX_PAYLOAD) return -1;

    size_t total = (size_t)SDFS_HEADER_SIZE + h->path_len + h->payload_len;
    if (total > out_cap) return -1;
    if (h->path_len && !path) return -1;
    if (h->payload_len && !payload) return -1;

    out[0] = h->opcode;
    put_u16(&out[1], h->req_id);
    out[3] = h->flags;
    put_u16(&out[4], h->seq);
    put_u16(&out[6], h->total);
    out[8] = h->path_len;
    put_u16(&out[9], h->payload_len);

    if (h->path_len)    memcpy(&out[SDFS_HEADER_SIZE], path, h->path_len);
    if (h->payload_len) memcpy(&out[SDFS_HEADER_SIZE + h->path_len], payload, h->payload_len);

    return (int)total;
}

int sdfs_decode(const uint8_t *frame, size_t len, sdfs_header_t *h,
                const char **path, const uint8_t **payload) {
    if (!frame || !h) return -1;
    if (len < SDFS_HEADER_SIZE) return -1;

    /* Opcode value is not validated here: dispatch policy (rejecting unknown
     * opcodes) belongs to the server/client layers, not the codec. */
    h->opcode      = frame[0];
    h->req_id      = get_u16(&frame[1]);
    h->flags       = frame[3];
    h->seq         = get_u16(&frame[4]);
    h->total       = get_u16(&frame[6]);
    h->path_len    = frame[8];
    h->payload_len = get_u16(&frame[9]);

    if (h->path_len > SDFS_MAX_PATH) return -1;
    if (h->payload_len > SDFS_MAX_PAYLOAD) return -1;
    if ((size_t)SDFS_HEADER_SIZE + h->path_len + h->payload_len != len) return -1;

    if (path)    *path    = h->path_len ? (const char *)&frame[SDFS_HEADER_SIZE] : NULL;
    if (payload) *payload = h->payload_len ? &frame[SDFS_HEADER_SIZE + h->path_len] : NULL;
    return 0;
}
