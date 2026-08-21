/* WASM OneWili glue - generated. Do not edit. */
#ifndef ONEWILI_WASM_H
#define ONEWILI_WASM_H
#include "onewili.h"
#include "onewili_cmdids.h"
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OW_WASM_IMPORT(name) \
    __attribute__((import_module("wiliwasm"))) __attribute__((import_name(name)))

/* The single generic device call: cmd_index into the firmware command tree;
 * args = little-endian packed arguments; ret receives packed return values.
 * Returns 0 on success, negative on error. */
OW_WASM_IMPORT("ow_call")
int ow_call(int cmd_index, const uint8_t* args, int args_len, uint8_t* ret, int ret_cap);

/* No transport to set up anymore; kept for API parity. */
static inline ow_status ow_open_wasm(ow_device* dev) { (void)dev; return OW_OK; }

/* ---- little-endian arg packers (return new offset) ---- */
static inline int ow__pack_i32(uint8_t* b, int off, int32_t v) {
    memcpy(b + off, &v, 4); return off + 4; }
static inline int ow__pack_u32(uint8_t* b, int off, uint32_t v) {
    memcpy(b + off, &v, 4); return off + 4; }
static inline int ow__pack_u8(uint8_t* b, int off, uint8_t v) {
    b[off] = v; return off + 1; }
static inline int ow__pack_f32(uint8_t* b, int off, float v) {
    memcpy(b + off, &v, 4); return off + 4; }
static inline int ow__pack_bytes(uint8_t* b, int off, const uint8_t* p, uint32_t n) {
    memcpy(b + off, &n, 4); off += 4; memcpy(b + off, p, n); return off + (int)n; }
static inline int ow__pack_str(uint8_t* b, int off, const char* s) {
    return ow__pack_bytes(b, off, (const uint8_t*)s, (uint32_t)strlen(s)); }

/* ---- little-endian return unpackers (advance *off) ---- */
static inline int32_t  ow__take_i32(const uint8_t* b, int* off) {
    int32_t v; memcpy(&v, b + *off, 4); *off += 4; return v; }
static inline uint32_t ow__take_u32(const uint8_t* b, int* off) {
    uint32_t v; memcpy(&v, b + *off, 4); *off += 4; return v; }
static inline uint8_t  ow__take_u8(const uint8_t* b, int* off) {
    uint8_t v = b[*off]; *off += 1; return v; }
static inline float    ow__take_f32(const uint8_t* b, int* off) {
    float v; memcpy(&v, b + *off, 4); *off += 4; return v; }
/* Copies up to cap-1 bytes of a length-prefixed value, NUL-terminates. */
static inline void ow__take_str(const uint8_t* b, int* off, char* dst, int cap) {
    uint32_t n; memcpy(&n, b + *off, 4); *off += 4;
    uint32_t k = ((int)n < cap - 1) ? n : (uint32_t)(cap - 1);
    memcpy(dst, b + *off, k); dst[k] = 0; *off += (int)n; }
static inline void ow__take_bytes(const uint8_t* b, int* off, uint8_t* dst,
                                  int cap, int* out_len) {
    uint32_t n; memcpy(&n, b + *off, 4); *off += 4;
    int k = ((int)n < cap) ? (int)n : cap; memcpy(dst, b + *off, k);
    if (out_len) *out_len = (int)n; *off += (int)n; }

/* ---- Retained runtime primitives (unchanged) ---- */
OW_WASM_IMPORT("waitms")   void waitms(int milliseconds);
OW_WASM_IMPORT("millis")   unsigned int millis(void);
OW_WASM_IMPORT("wilirand") int wilirand(void);
OW_WASM_IMPORT("log")      void log(int v);
OW_WASM_IMPORT("log64")    void log64(long long v);
OW_WASM_IMPORT("logf")     void logf(float v);
OW_WASM_IMPORT("logf64")   void logf64(double v);
OW_WASM_IMPORT("logtext")  void logtext(char* v);

/* ---- Retained file I/O ---- */
OW_WASM_IMPORT("openFile")               int openFile(const char* file_name, int mode);
OW_WASM_IMPORT("closeFile")              int closeFile(int handle);
OW_WASM_IMPORT("writeFile")              int writeFile(int handle, unsigned char* data, int data_bytes);
OW_WASM_IMPORT("preAllocateSpaceForFile") int preAllocateSpaceForFile(int handle, int size_in_bytes);
OW_WASM_IMPORT("readFile")               int readFile(int handle, unsigned char* data, int data_bytes);
OW_WASM_IMPORT("readFileLine")           int readFileLine(int handle, char* data, int data_bytes);
OW_WASM_IMPORT("setFilePosition")        int setFilePosition(int handle, int position);
OW_WASM_IMPORT("getFilePosition")        int getFilePosition(int handle);
OW_WASM_IMPORT("getFileSize")            int getFileSize(int handle);

/* ---- Retained file system ---- */
OW_WASM_IMPORT("renameFileOrDirectory")  int renameFileOrDirectory(const char* name, const char* new_name);
OW_WASM_IMPORT("fileExists")             int fileExists(const char* file_name);
OW_WASM_IMPORT("makeDirectory")          int makeDirectory(const char* file_name);
OW_WASM_IMPORT("changeDirectory")        int changeDirectory(const char* file_name);
OW_WASM_IMPORT("removeFileOrDirectory")  int removeFileOrDirectory(const char* file_name);
OW_WASM_IMPORT("getDirectoryItemByIndex") int getDirectoryItemByIndex(const char* directory, const char* file_name, int include_extension, int index);
OW_WASM_IMPORT("getVolumeInfo")          void getVolumeInfo(int* free_space, int* total);

#ifdef __cplusplus
}
#endif
#endif /* ONEWILI_WASM_H */
