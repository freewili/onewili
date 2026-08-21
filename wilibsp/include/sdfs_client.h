#ifndef SDFS_CLIENT_H
#define SDFS_CLIENT_H

#include "sdfs_wire.h"
#include "sdfs_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const sdfs_transport_t *tp;
    uint16_t next_req_id;
    uint32_t timeout_polls;   /* recv attempts before SDFS_ERR_TIMEOUT */
    uint8_t  buf[SDFS_MAX_FRAME];
} sdfs_client_t;

typedef struct {
    sdfs_status_t status;
    int           card_present;
    int           mounted;
    uint32_t      bytes;
} sdfs_status_info_t;

/* Read state machine context (Task 9). */
typedef struct {
    uint16_t      req_id;
    uint8_t      *dst;
    size_t        cap;
    size_t        got;
    uint16_t      next_seq;
    int           done;
    sdfs_status_t status;
    uint32_t      polls;
} sdfs_read_ctx_t;

void sdfs_client_init(sdfs_client_t *c, const sdfs_transport_t *tp);

/* Fire-and-forget whole-file write. Splits into chunks, returns immediately.
 * *out_req_id is always set (when non-NULL) to the assigned request id, even
 * if the call subsequently fails (use with sdfs_status to reconcile outcome).
 * flags may include SDFS_FLAG_APPEND.
 * Returns SDFS_ERR_BAD_REQUEST if path exceeds SDFS_MAX_PATH chars.
 * Returns SDFS_ERR_IO if a send fails; note chunks are sent incrementally, so
 * on a mid-stream failure some chunks may already have been transmitted (no
 * rollback) — the caller reconciles the outcome via sdfs_status. */
sdfs_status_t sdfs_write(sdfs_client_t *c, const char *path,
                         const uint8_t *data, size_t len, uint8_t flags,
                         uint16_t *out_req_id);

/* Note: the blocking sdfs_read/sdfs_status pump the shared client transport and
 * drop non-matching frames, so a single client object services ONE operation at
 * a time (do not interleave a status query with an in-flight read on the same
 * client). */

/* Begin a read: assigns a req_id, sends READ_REQ, initializes ctx. */
sdfs_status_t sdfs_read_begin(sdfs_client_t *c, sdfs_read_ctx_t *rc,
                              const char *path, uint8_t *dst, size_t cap);

/* Advance a read by draining at most one inbound frame.
 * Returns SDFS_PENDING while incomplete, or a final status. */
sdfs_status_t sdfs_read_poll(sdfs_client_t *c, sdfs_read_ctx_t *rc);

/* Blocking whole-file read. On SDFS_OK sets *out_len to bytes read.
 * Returns SDFS_ERR_TIMEOUT if the response never completes. */
sdfs_status_t sdfs_read(sdfs_client_t *c, const char *path,
                        uint8_t *dst, size_t cap, size_t *out_len);

/* Query write/media status. req_id==0 asks for the global/last result.
 * Synchronous: blocks (pumping recv) until STATUS_RESP or timeout.
 * Fills *out on SDFS_OK; returns SDFS_ERR_TIMEOUT if no response arrives. */
sdfs_status_t sdfs_status(sdfs_client_t *c, uint16_t req_id,
                          sdfs_status_info_t *out);

/* Query metadata for a path. On SDFS_OK sets *is_dir (0/1) and *size (bytes; 0 for dirs). */
sdfs_status_t sdfs_stat(sdfs_client_t *c, const char *path, int *is_dir, uint32_t *size);

/* Create a directory. Blocking; returns the server's status. */
sdfs_status_t sdfs_mkdir(sdfs_client_t *c, const char *path);
/* Delete a file or empty directory. Blocking. */
sdfs_status_t sdfs_remove(sdfs_client_t *c, const char *path);
/* Rename/move oldp -> newp. Blocking. newp must be <= SDFS_MAX_PAYLOAD chars. */
sdfs_status_t sdfs_rename(sdfs_client_t *c, const char *oldp, const char *newp);

/* Called once per directory entry during sdfs_list. name is NUL-terminated. */
typedef void (*sdfs_list_cb)(const char *name, int is_dir, uint32_t size, void *user);

/* Enumerate a directory. Blocking; invokes cb per entry as LIST_CHUNKs arrive.
 * Returns SDFS_OK when the final chunk is received, or an error status. */
sdfs_status_t sdfs_list(sdfs_client_t *c, const char *path,
                        sdfs_list_cb cb, void *user);

/* Stateful remote file handles (Phase 2-C). mode: 0=read, 1=write/truncate,
 * 2=write/append. On success *out_handle is the server-assigned handle id. */
sdfs_status_t sdfs_open(sdfs_client_t *c, const char *path, uint8_t mode, uint8_t *out_handle);
/* Read forward from the handle's current position, up to len bytes. Sets
 * *got to bytes actually read (< len at EOF). */
sdfs_status_t sdfs_hread(sdfs_client_t *c, uint8_t handle, uint8_t *dst, uint32_t len, uint32_t *got);
/* Write at the handle's current position. The final chunk of each call is
 * acknowledged, providing bounded transport backpressure; sdfs_hclose also
 * verifies the total byte count. */
sdfs_status_t sdfs_hwrite(sdfs_client_t *c, uint8_t handle, const uint8_t *data, uint32_t len);
/* Seek the handle's position (FatFs f_lseek semantics). Blocking. */
sdfs_status_t sdfs_seek(sdfs_client_t *c, uint8_t handle, uint32_t offset);
/* Close the handle. `expected_bytes` = total bytes the caller wrote via sdfs_hwrite
 * on this handle; the server compares it against bytes actually written and returns
 * SDFS_ERR_IO on mismatch (a dropped fire-and-forget chunk), turning silent write
 * truncation into a detectable failure. Pass SDFS_HCLOSE_NO_VERIFY to skip the check
 * (e.g. closing a stranded handle of unknown write count). Returns the first write
 * error seen (if any), else the integrity/close status. Blocking. */
#define SDFS_HCLOSE_NO_VERIFY 0xFFFFFFFFu
sdfs_status_t sdfs_hclose(sdfs_client_t *c, uint8_t handle, uint32_t expected_bytes);

#ifdef __cplusplus
}
#endif

#endif /* SDFS_CLIENT_H */
