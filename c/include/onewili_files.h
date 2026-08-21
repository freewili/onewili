/* File transfer for the OneWili C API. Layered over the generated filesystem
 * menu commands; see ow_files.h for the protocol implementation. */
#ifndef ONEWILI_FILES_H
#define ONEWILI_FILES_H
#include "onewili.h"
#include "ow_files.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Upload `len` bytes to `dev_path` on the device. cb may be NULL. */
ow_status ow_file_put_mem(ow_device* dev, const char* dev_path,
                          const uint8_t* data, size_t len,
                          ow_progress_cb cb, void* ctx);

/* Download `dev_path` into buf. OW_ERR_BUFFER when it does not fit. */
ow_status ow_file_get_mem(ow_device* dev, const char* dev_path,
                          uint8_t* buf, size_t cap, size_t* out_len,
                          ow_progress_cb cb, void* ctx);

/* List `dir_path` ("" or "/" for the current directory). OW_ERR_BUFFER when
 * there are more entries than cap; *out_count entries are still valid. */
ow_status ow_file_list(ow_device* dev, const char* dir_path,
                       ow_dir_entry* out, size_t cap, size_t* out_count);

/* Connect the SD card to the USB reader / PC (true) or the main CPU (false). */
ow_status ow_sd_host_select(ow_device* dev, bool to_pc);

#ifndef OW_NO_STDIO
/* Host convenience: same transfers against real files. */
ow_status ow_file_put(ow_device* dev, const char* host_path,
                      const char* dev_path, ow_progress_cb cb, void* ctx);
ow_status ow_file_get(ow_device* dev, const char* dev_path,
                      const char* host_path, ow_progress_cb cb, void* ctx);
#endif

#ifdef __cplusplus
}
#endif
#endif
