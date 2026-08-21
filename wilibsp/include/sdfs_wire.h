#ifndef SDFS_WIRE_H
#define SDFS_WIRE_H

#include <stdint.h>
#include <stddef.h>

/* ---- Tunables (overridable at compile time) ---- */
#ifndef SDFS_MAX_PAYLOAD
#define SDFS_MAX_PAYLOAD 512u   /* file-data bytes per frame */
#endif
#ifndef SDFS_MAX_PATH
#define SDFS_MAX_PATH 255u      /* matches FatFs FF_MAX_LFN */
#endif

#define SDFS_HEADER_SIZE 11u
#define SDFS_MAX_FRAME (SDFS_HEADER_SIZE + SDFS_MAX_PATH + SDFS_MAX_PAYLOAD)

/* ---- Opcodes ---- */
typedef enum {
    SDFS_OP_WRITE_CHUNK = 1,
    SDFS_OP_READ_REQ    = 2,
    SDFS_OP_READ_CHUNK  = 3,
    SDFS_OP_STATUS_REQ  = 4,
    SDFS_OP_STATUS_RESP = 5,
    SDFS_OP_NACK        = 6,
    SDFS_OP_STAT_REQ    = 7,
    SDFS_OP_STAT_RESP   = 8,
    SDFS_OP_LIST_REQ    = 9,
    SDFS_OP_LIST_CHUNK  = 10,
    SDFS_OP_MKDIR_REQ   = 11,
    SDFS_OP_REMOVE_REQ  = 12,
    SDFS_OP_RENAME_REQ  = 13,
    SDFS_OP_RESULT      = 14,
    SDFS_OP_OPEN_REQ     = 15,
    SDFS_OP_OPEN_RESP    = 16,
    SDFS_OP_HREAD_REQ    = 17,
    SDFS_OP_HWRITE_CHUNK = 18,
    SDFS_OP_SEEK_REQ     = 19,
    SDFS_OP_HCLOSE_REQ   = 20
} sdfs_opcode_t;

/* ---- Frame flags ---- */
#define SDFS_FLAG_APPEND 0x01u
#define SDFS_FLAG_LAST   0x02u

/* ---- Status codes ---- */
typedef enum {
    SDFS_OK              = 0,
    SDFS_ERR_NOT_FOUND   = 1,
    SDFS_ERR_IO          = 2,
    SDFS_ERR_NO_SPACE    = 3,
    SDFS_ERR_TOO_BIG     = 4,
    SDFS_ERR_TIMEOUT     = 5,
    SDFS_ERR_BUSY        = 6,
    SDFS_ERR_BAD_REQUEST = 7,
    SDFS_ERR_NO_CARD     = 8,
    SDFS_ERR_NOT_MOUNTED = 9,
    SDFS_PENDING         = 100   /* in-progress / unknown */
} sdfs_status_t;

/* ---- Decoded frame header ---- */
typedef struct {
    uint8_t  opcode;
    uint16_t req_id;
    uint8_t  flags;
    uint16_t seq;
    uint16_t total;
    uint8_t  path_len;
    uint16_t payload_len;
} sdfs_header_t;

#endif /* SDFS_WIRE_H */
