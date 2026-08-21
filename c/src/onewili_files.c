#define OW_FILES_IMPLEMENTATION
#include "onewili_files.h"
#include <stdio.h>
#include <stdlib.h>

/* ow_files_status (ow_files.h) and ow_status (onewili.h) declare the same
 * seven enumerators in the same order -- OK=0, ERR_ARG, ERR_IO, ERR_TIMEOUT,
 * ERR_FAILED, ERR_PROTOCOL, ERR_BUFFER (verified when ow_files_status was
 * introduced: see the menutool ledger's Task 5 entry recording the
 * enumerator-for-enumerator comparison) -- so a direct cast between them is
 * sound and does not need a switch over every value. */
static ow_status ow_files_status_cast(ow_files_status st) {
    return (ow_status) st;
}

/* ow_files_io is structurally identical to ow_transport (void* ctx plus the
 * same write/read signatures), so the device's transport is reused directly
 * rather than re-wrapped through an adapter callback. */
static ow_files_io ow_files_io_from_device(ow_device* dev) {
    ow_files_io io;
    io.ctx   = dev->t.ctx;
    io.write = dev->t.write;
    io.read  = dev->t.read;
    return io;
}

ow_status ow_file_put_mem(ow_device* dev, const char* dev_path,
                          const uint8_t* data, size_t len,
                          ow_progress_cb cb, void* ctx) {
    if (!dev) return OW_ERR_ARG;
    ow_files_io io = ow_files_io_from_device(dev);
    return ow_files_status_cast(ow_files_put(&io, dev_path, data, len, cb, ctx));
}

ow_status ow_file_get_mem(ow_device* dev, const char* dev_path,
                          uint8_t* buf, size_t cap, size_t* out_len,
                          ow_progress_cb cb, void* ctx) {
    if (!dev) return OW_ERR_ARG;
    ow_files_io io = ow_files_io_from_device(dev);
    return ow_files_status_cast(ow_files_get(&io, dev_path, buf, cap, out_len, cb, ctx));
}

ow_status ow_file_list(ow_device* dev, const char* dir_path,
                       ow_dir_entry* out, size_t cap, size_t* out_count) {
    if (!dev) return OW_ERR_ARG;
    ow_files_io io = ow_files_io_from_device(dev);
    return ow_files_status_cast(ow_files_list(&io, dir_path, out, cap, out_count));
}

ow_status ow_sd_host_select(ow_device* dev, bool to_pc) {
    /* Wire h\x\k. host=0 connects the SD card to the main CPU, host=1 to the
     * USB reader / PC (fwMenuFileSystem::setSDCardHostSelection). */
    return ow_hardware_file_system_set_sd_card_host(dev, to_pc ? 1 : 0);
}

#ifndef OW_NO_STDIO

#ifndef OW_FILE_GET_MAX
/* ow_file_get_mem needs its whole destination buffer up front -- there is no
 * device-side "stat" call to learn a file's size before reading it -- so this
 * host convenience wrapper allocates a generous fixed ceiling rather than
 * guessing small and retrying. Define OW_FILE_GET_MAX before including this
 * file (or as a compile definition) to raise it for a device that serves
 * larger files. */
#define OW_FILE_GET_MAX (16u * 1024u * 1024u)
#endif

ow_status ow_file_put(ow_device* dev, const char* host_path,
                      const char* dev_path, ow_progress_cb cb, void* ctx) {
    if (!dev || !host_path || !dev_path) return OW_ERR_ARG;

    FILE* f = fopen(host_path, "rb");
    if (!f) return OW_ERR_IO;

    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return OW_ERR_IO; }
    long sz = ftell(f);
    if (sz < 0) { fclose(f); return OW_ERR_IO; }
    if (fseek(f, 0, SEEK_SET) != 0) { fclose(f); return OW_ERR_IO; }

    uint8_t* buf = NULL;
    if (sz > 0) {
        buf = (uint8_t*) malloc((size_t) sz);
        if (!buf) { fclose(f); return OW_ERR_IO; }
        if (fread(buf, 1, (size_t) sz, f) != (size_t) sz) {
            free(buf);
            fclose(f);
            return OW_ERR_IO;
        }
    }
    if (fclose(f) != 0) { free(buf); return OW_ERR_IO; }

    ow_status st = ow_file_put_mem(dev, dev_path, buf, (size_t) sz, cb, ctx);
    free(buf);
    return st;
}

ow_status ow_file_get(ow_device* dev, const char* dev_path,
                      const char* host_path, ow_progress_cb cb, void* ctx) {
    if (!dev || !dev_path || !host_path) return OW_ERR_ARG;

    uint8_t* buf = (uint8_t*) malloc(OW_FILE_GET_MAX);
    if (!buf) return OW_ERR_IO;

    size_t got = 0;
    ow_status st = ow_file_get_mem(dev, dev_path, buf, OW_FILE_GET_MAX, &got, cb, ctx);
    if (st != OW_OK) { free(buf); return st; }

    FILE* f = fopen(host_path, "wb");
    if (!f) { free(buf); return OW_ERR_IO; }
    size_t wrote = fwrite(buf, 1, got, f);
    int closeRc = fclose(f);
    free(buf);
    if (wrote != got || closeRc != 0) return OW_ERR_IO;
    return OW_OK;
}

#endif /* OW_NO_STDIO */
