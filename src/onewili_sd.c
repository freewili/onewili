#include "onewili_sd.h"
#include <string.h>

/* These MUST match fw2main's sdfslib build (sdfslib/CMakeLists.txt). A
 * mismatch does not fail to link -- it silently truncates paths and desyncs
 * chunking against a server that disagrees -- so make it a build error. */
#if SDFS_MAX_PAYLOAD != 96 || SDFS_MAX_PATH != 128
#error "SDFS tunables must match fw2main: SDFS_MAX_PAYLOAD=96, SDFS_MAX_PATH=128"
#endif

/* Milliseconds the self-heal closes in ow_sd_arm are allowed to wait. Kept
 * short so booting against a MAIN CPU that is not answering costs a fraction
 * of a second rather than OW_SD_MAX_HANDLES full timeouts. */
#define OW_SD_ARM_TIMEOUT_MS 200

/* sdfs_client_init BORROWS the transport (sdfs_client.c: c->tp = tp), it does
 * not copy it, and every later ow_sd_* call dereferences that pointer. Callers
 * naturally build an sdfs_transport_t on the stack -- ow_open_fwgui does -- so
 * owning the storage here is what stops the client from outliving it. */
static sdfs_transport_t g_tp;
static sdfs_client_t g_cli;
static int           g_armed;
static sdfs_status_t g_last = SDFS_OK;
static uint32_t      g_timeout_ms = 2000;

/* One transport poll costs ~1 ms (the transport paces an empty poll), so the
 * client's poll budget IS the millisecond budget. */
static void apply_timeout(uint32_t ms) {
    g_timeout_ms = ms;
    g_cli.timeout_polls = ms ? ms : 1;
}

static ow_status map(sdfs_status_t s) {
    g_last = s;
    switch (s) {
    case SDFS_OK:              return OW_OK;
    case SDFS_ERR_TIMEOUT:     return OW_ERR_TIMEOUT;
    case SDFS_ERR_BAD_REQUEST: return OW_ERR_PROTOCOL;   /* also: lost chunk */
    default:                   return OW_ERR_FAILED;
    }
}

/* Absolute, '/'-rooted, at most SDFS_MAX_PATH characters. */
static bool path_ok(const char* p) {
    size_t n;
    if (!p || p[0] != '/') return false;
    for (n = 0; p[n]; n++)
        if (n >= SDFS_MAX_PATH) return false;
    return true;
}

ow_status ow_sd_arm(const sdfs_transport_t* tp) {
    uint32_t saved = g_timeout_ms;
    uint8_t h;
    if (!tp) return OW_ERR_ARG;
    g_tp = *tp;                       /* copy: the caller's object may be automatic */
    sdfs_client_init(&g_cli, &g_tp);
    g_armed = 1;
    /* Resetting this CPU leaves the server's handle slots marked used, so
     * every later open would fail BUSY until MAIN reboots. Close them all
     * under a short budget -- failures here are expected (that is the normal
     * case on a clean boot) and deliberately ignored, but they must not cost
     * OW_SD_MAX_HANDLES full timeouts when MAIN is not answering. */
    apply_timeout(OW_SD_ARM_TIMEOUT_MS);
    for (h = 0; h < OW_SD_MAX_HANDLES; h++)
        (void)sdfs_hclose(&g_cli, h, SDFS_HCLOSE_NO_VERIFY);
    apply_timeout(saved);
    g_last = SDFS_OK;
    return OW_OK;
}

void ow_sd_set_timeout_ms(uint32_t ms) { apply_timeout(ms); }

sdfs_status_t ow_sd_last_error(void) { return g_last; }

ow_status ow_sd_open(ow_device* dev, ow_sd_file* f, const char* path, ow_sd_mode mode) {
    uint8_t h = 0xFF;
    sdfs_status_t s;
    (void)dev;
    if (!f) return OW_ERR_ARG;
    /* Leave *f safe on EVERY failure path. `ow_sd_file f;` is stack-declared in
     * the shipped example, so an app that skips the status check would
     * otherwise act on an uninitialised handle byte and send HWRITE/HCLOSE
     * against whatever the frame happened to hold. */
    f->is_open = 0;
    if (!g_armed) return OW_ERR_IO;
    if (!path_ok(path)) return OW_ERR_ARG;
    if (mode != OW_SD_READ && mode != OW_SD_WRITE && mode != OW_SD_APPEND)
        return OW_ERR_ARG;
    s = sdfs_open(&g_cli, path, (uint8_t)mode, &h);
    if (s != SDFS_OK) return map(s);
    f->h = h; f->mode = (uint8_t)mode; f->is_open = 1; f->written = 0;
    return map(SDFS_OK);
}

ow_status ow_sd_read(ow_sd_file* f, void* dst, size_t len, size_t* got) {
    uint32_t n = 0;
    sdfs_status_t s;
    if (got) *got = 0;
    if (!g_armed) return OW_ERR_IO;
    if (!f || !f->is_open || !dst) return OW_ERR_ARG;
    if (f->mode != OW_SD_READ) return OW_ERR_ARG;
    if (len == 0) return map(SDFS_OK);
    s = sdfs_hread(&g_cli, f->h, (uint8_t*)dst, (uint32_t)len, &n);
    if (got) *got = (size_t)n;
    return map(s);
}

ow_status ow_sd_write(ow_sd_file* f, const void* src, size_t len) {
    sdfs_status_t s;
    if (!g_armed) return OW_ERR_IO;
    if (!f || !f->is_open) return OW_ERR_ARG;
    if (f->mode == OW_SD_READ) return OW_ERR_ARG;
    if (len == 0) return map(SDFS_OK);
    if (!src) return OW_ERR_ARG;
    s = sdfs_hwrite(&g_cli, f->h, (const uint8_t*)src, (uint32_t)len);
    if (s == SDFS_OK) f->written += (uint32_t)len;
    return map(s);
}

ow_status ow_sd_seek(ow_sd_file* f, uint32_t offset) {
    if (!g_armed) return OW_ERR_IO;
    if (!f || !f->is_open) return OW_ERR_ARG;
    /* Safe on write handles too: the server's integrity counter tracks bytes
     * written (sdfs_server.c: h->wr_bytes), not the file position. */
    return map(sdfs_seek(&g_cli, f->h, offset));
}

ow_status ow_sd_close(ow_sd_file* f) {
    sdfs_status_t s;
    if (!g_armed) return OW_ERR_IO;
    if (!f || !f->is_open) return OW_ERR_ARG;
    s = sdfs_hclose(&g_cli, f->h,
                    f->mode == OW_SD_READ ? SDFS_HCLOSE_NO_VERIFY : f->written);
    f->is_open = 0;
    return map(s);
}

ow_status ow_sd_get_mem(ow_device* dev, const char* path, void* dst, size_t cap,
                        size_t* out_len) {
    size_t n = 0;
    sdfs_status_t s;
    (void)dev;
    if (out_len) *out_len = 0;
    if (!g_armed) return OW_ERR_IO;
    if (!dst || !path_ok(path)) return OW_ERR_ARG;
    s = sdfs_read(&g_cli, path, (uint8_t*)dst, cap, &n);
    if (out_len) *out_len = n;
    return map(s);
}

ow_status ow_sd_put_mem(ow_device* dev, const char* path, const void* src,
                        size_t len, bool append) {
    uint16_t req = 0;
    sdfs_status_info_t info;
    sdfs_status_t s;
    (void)dev;
    if (!g_armed) return OW_ERR_IO;
    if (!path_ok(path) || (!src && len)) return OW_ERR_ARG;
    s = sdfs_write(&g_cli, path, (const uint8_t*)src, len,
                   append ? SDFS_FLAG_APPEND : 0u, &req);
    if (s != SDFS_OK) return map(s);
    /* sdfs_write is fire-and-forget; ask the server how it actually went so
     * the caller gets one synchronous answer. */
    memset(&info, 0, sizeof info);
    s = sdfs_status(&g_cli, req, &info);
    if (s != SDFS_OK) return map(s);
    return map(info.status);
}

ow_status ow_sd_stat(ow_device* dev, const char* path, bool* is_dir, uint32_t* size) {
    int dir = 0;
    uint32_t sz = 0;
    sdfs_status_t s;
    (void)dev;
    if (!g_armed) return OW_ERR_IO;
    if (!path_ok(path)) return OW_ERR_ARG;
    s = sdfs_stat(&g_cli, path, &dir, &sz);
    if (s == SDFS_OK) {
        if (is_dir) *is_dir = dir ? true : false;
        if (size)   *size   = sz;
    }
    return map(s);
}

/* sdfs_list_cb takes int is_dir; ow_sd_list_cb takes bool. Bridge them. */
typedef struct { ow_sd_list_cb cb; void* user; } ow_sd_list_shim;

static void ow_sd_list_thunk(const char* name, int is_dir, uint32_t size, void* user) {
    ow_sd_list_shim* sh = (ow_sd_list_shim*)user;
    if (sh->cb) sh->cb(name, is_dir ? true : false, size, sh->user);
}

ow_status ow_sd_list(ow_device* dev, const char* path, ow_sd_list_cb cb, void* user) {
    ow_sd_list_shim sh;
    (void)dev;
    if (!g_armed) return OW_ERR_IO;
    if (!path_ok(path) || !cb) return OW_ERR_ARG;
    sh.cb = cb; sh.user = user;
    return map(sdfs_list(&g_cli, path, ow_sd_list_thunk, &sh));
}

ow_status ow_sd_mkdir(ow_device* dev, const char* path) {
    (void)dev;
    if (!g_armed) return OW_ERR_IO;
    if (!path_ok(path)) return OW_ERR_ARG;
    return map(sdfs_mkdir(&g_cli, path));
}

ow_status ow_sd_remove(ow_device* dev, const char* path) {
    (void)dev;
    if (!g_armed) return OW_ERR_IO;
    if (!path_ok(path)) return OW_ERR_ARG;
    return map(sdfs_remove(&g_cli, path));
}

ow_status ow_sd_rename(ow_device* dev, const char* oldp, const char* newp) {
    (void)dev;
    if (!g_armed) return OW_ERR_IO;
    if (!path_ok(oldp) || !path_ok(newp)) return OW_ERR_ARG;
    /* The destination travels in the payload field, not the header's path
     * field, so it is bounded by SDFS_MAX_PAYLOAD rather than SDFS_MAX_PATH
     * (sdfs_client.h). Reject it here so an over-long destination reports
     * OW_ERR_ARG like every other client-side rejection, instead of coming
     * back as OW_ERR_PROTOCOL from the client's own length check. */
    if (strlen(newp) > SDFS_MAX_PAYLOAD) return OW_ERR_ARG;
    return map(sdfs_rename(&g_cli, oldp, newp));
}
