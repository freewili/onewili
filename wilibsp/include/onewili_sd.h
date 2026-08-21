/* SD-card access for the OneWili WiliBSP API.
 *
 * Requests travel to the FreeWili 2 MAIN CPU over the FwGUI display link and
 * are served from the SD card (FatFs drive "1:") by fw2main's SDFS server --
 * the same route the stock display firmware uses for every file operation.
 * ow_open_fwgui() arms this layer for you; see onewili_fwgui.h.
 *
 * Contract:
 *   - Paths are ABSOLUTE and '/'-rooted, at most SDFS_MAX_PATH characters.
 *     There is no internal-flash ("flsh:") equivalent -- SDFS reaches the SD
 *     card and nothing else.
 *   - At most OW_SD_MAX_HANDLES files may be open at once (fw2main's limit);
 *     a further open fails with OW_ERR_FAILED / SDFS_ERR_BUSY.
 *   - Calls BLOCK, with an idle timeout (default 2000 ms). While blocked the
 *     link keeps parsing OneWili text and binary frames into their queues.
 *   - Writes are fire-and-forget: ow_sd_write cannot report a write failure.
 *     ow_sd_close sends the byte count it expects and the server compares it,
 *     so ALWAYS check ow_sd_close's status -- that is where a dropped chunk
 *     surfaces, as OW_ERR_FAILED / SDFS_ERR_IO.
 */
#ifndef ONEWILI_SD_H
#define ONEWILI_SD_H
#include "onewili.h"
#include "sdfs_client.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Mirrors fw2main's SDFS_HOST_MAX_HANDLES (sdfslib/CMakeLists.txt). */
#ifndef OW_SD_MAX_HANDLES
#define OW_SD_MAX_HANDLES 2
#endif

/* Values match sdfslib's open modes; do not renumber. */
typedef enum { OW_SD_READ = 0, OW_SD_WRITE = 1, OW_SD_APPEND = 2 } ow_sd_mode;

typedef struct {
    uint8_t  h;         /* server-assigned handle id */
    uint8_t  mode;      /* ow_sd_mode */
    uint8_t  is_open;
    uint32_t written;   /* bytes passed to ow_sd_write; verified by ow_sd_close */
} ow_sd_file;

typedef void (*ow_sd_list_cb)(const char* name, bool is_dir, uint32_t size,
                              void* user);

/* Binds the SD client to `tp` and clears handles a previous run of this app
 * left open on the server (a display-CPU reset does not close them). Called
 * for you by ow_open_fwgui(); call it directly only with a custom transport.
 * `tp` is COPIED; it need not outlive this call, so an automatic
 * sdfs_transport_t is fine. (sdfslib's client stores a borrowed pointer, so
 * without that copy every later ow_sd_* call would read a dead frame.) */
ow_status ow_sd_arm(const sdfs_transport_t* tp);

/* `dev` is accepted for symmetry with the rest of the OneWili API and is
 * currently unused: one display CPU has one link, and the client is static. */
ow_status ow_sd_open (ow_device* dev, ow_sd_file* f, const char* path, ow_sd_mode mode);
/* Reads up to `len` bytes forward from the handle position. *got < len means
 * end of file, and is NOT an error. */
ow_status ow_sd_read (ow_sd_file* f, void* dst, size_t len, size_t* got);
ow_status ow_sd_write(ow_sd_file* f, const void* src, size_t len);
ow_status ow_sd_seek (ow_sd_file* f, uint32_t offset);
ow_status ow_sd_close(ow_sd_file* f);

/* Whole-file transfers. These use the stateless SDFS opcodes, so they neither
 * consume nor require a free file handle. */
ow_status ow_sd_get_mem(ow_device* dev, const char* path, void* dst, size_t cap,
                        size_t* out_len);
ow_status ow_sd_put_mem(ow_device* dev, const char* path, const void* src,
                        size_t len, bool append);

ow_status ow_sd_stat  (ow_device* dev, const char* path, bool* is_dir, uint32_t* size);
/* Invokes `cb` once per entry as chunks arrive. The server does not emit
 * "." or "..". */
ow_status ow_sd_list  (ow_device* dev, const char* path, ow_sd_list_cb cb, void* user);
ow_status ow_sd_mkdir (ow_device* dev, const char* path);
/* Deletes a file or an EMPTY directory. */
ow_status ow_sd_remove(ow_device* dev, const char* path);
/* Both paths must be on the SD card; the server cannot move across volumes.
 * `oldp` follows the usual SDFS_MAX_PATH bound; `newp` travels in the wire
 * payload field instead of the header's path field, so it is bounded by the
 * tighter SDFS_MAX_PAYLOAD -- a longer destination is rejected client-side
 * with OW_ERR_ARG. */
ow_status ow_sd_rename(ow_device* dev, const char* oldp, const char* newp);

/* Idle timeout for every blocking call, in milliseconds (default 2000).
 * For the chunk-streaming waits (reads and directory listings) the clock
 * restarts whenever a frame arrives, so a long transfer that keeps producing
 * chunks will not trip it. The single-response waits (open, stat, mkdir,
 * remove, rename, seek, close) never reset it, so there the budget is the
 * whole wait -- which is what you want, since exactly one reply is due. */
void ow_sd_set_timeout_ms(uint32_t ms);

/* The sdfslib status behind the last ow_sd_* result that actually reached the
 * SDFS client (SDFS_OK when it was OW_OK). Valid until the next such call.
 * NOT updated by client-side rejections: a call that returns OW_ERR_ARG (bad
 * path, bad handle, bad mode) or OW_ERR_IO (never armed) short-circuits before
 * the wire, and this still reports the PREVIOUS call's status. Read it only
 * after an OW_ERR_TIMEOUT / OW_ERR_PROTOCOL / OW_ERR_FAILED result. */
sdfs_status_t ow_sd_last_error(void);

#ifdef __cplusplus
}
#endif
#endif /* ONEWILI_SD_H */
