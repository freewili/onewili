#ifndef SDFS_CODEC_H
#define SDFS_CODEC_H

#include <stddef.h>
#include <stdint.h>

#include "sdfs_wire.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Serialize header + optional path + optional payload into `out`.
 * `h->path_len` / `h->payload_len` describe the sizes; `path`/`payload`
 * may be NULL when the corresponding length is 0.
 * Returns total frame length on success (always > 0, at most SDFS_MAX_FRAME),
 * or -1 on invalid input / out too small. -1 is the only error sentinel. */
int sdfs_encode(const sdfs_header_t *h, const char *path,
                const uint8_t *payload, uint8_t *out, size_t out_cap);

/* Parse `frame` of `len` bytes. Fills `*h` and points `*path`/`*payload`
 * into `frame` (no copy); either may be set NULL when its length is 0.
 * Returns 0 on success, -1 on malformed/oversize frame. */
int sdfs_decode(const uint8_t *frame, size_t len, sdfs_header_t *h,
                const char **path, const uint8_t **payload);

#ifdef __cplusplus
}
#endif

#endif /* SDFS_CODEC_H */
