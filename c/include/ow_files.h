/* OneWili file-transfer runtime: CRC-32, wire-line builders, fdir parsing,
 * and the put/get/list transfer state machines. The transfer routines are
 * driven through the caller-supplied ow_files_io callback pair rather than
 * onewili.h's ow_transport directly; the emitted package wraps this file in
 * onewili_files.h/.c adapting ow_device to an ow_files_io.
 *
 * Standalone by design (no onewili.h dependency) so the unit tests can compile
 * it directly.
 */
#ifndef OW_FILES_H
#define OW_FILES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OW_FILES_NAME_MAX 64

/* Mirrors ow_status (onewili.h). Kept as its own enum so this header stays
 * standalone; the emitted wrapper casts between them. */
typedef enum {
    OW_FILES_OK = 0, OW_FILES_ERR_ARG, OW_FILES_ERR_IO, OW_FILES_ERR_TIMEOUT,
    OW_FILES_ERR_FAILED, OW_FILES_ERR_PROTOCOL, OW_FILES_ERR_BUFFER
} ow_files_status;

typedef struct {
    char     name[OW_FILES_NAME_MAX];
    bool     is_dir;
    uint32_t size;
} ow_dir_entry;

typedef void (*ow_progress_cb)(void* ctx, size_t done, size_t total);

/* ---- CRC-32 (IEEE 802.3, reflected 0xEDB88320) --------------------------- */
void     ow_files_crc32_begin(uint32_t* state);
void     ow_files_crc32_update(uint32_t* state, const uint8_t* data, size_t len);
uint32_t ow_files_crc32_final(uint32_t state);
uint32_t ow_files_crc32(const uint8_t* data, size_t len);

/* ---- Wire-line builders -------------------------------------------------- */
/* Both write the full navigation prefix plus the command line. Return the byte
 * count written, or 0 when cap is too small. On failure the buffer's contents
 * are unspecified -- snprintf still writes a truncated, NUL-terminated partial
 * string into it even as it reports overflow -- so callers must check the
 * return value rather than assume buf was left untouched. Neither builder
 * writes a NUL terminator beyond the returned length on success. */
size_t ow_files_build_put_header(char* buf, size_t cap, const char* dev_path,
                                 uint32_t size, uint32_t crc32);
size_t ow_files_build_get_header(char* buf, size_t cap, const char* dev_path);

/* ---- fdir event parsing -------------------------------------------------- */
/* `args` is exactly what ow_poll_text_line (onewili.c:221) yields: everything
 * after the event id with the trailing ']' removed. That INCLUDES the framing
 * rpConsole::printEventResponse wraps around every event payload:
 *
 *     wire  : [*fdir <hexTimestamp> <sequence> <payload...> <ok>]
 *     args  :        <hexTimestamp> <sequence> <payload...> <ok>
 *     payload:                      dir <name> <size>
 *                                   fil <name> <size>
 *                                   end <count>
 *
 * So skip two leading tokens and one trailing token to reach the payload.
 * Parse the payload's trailing numbers from the RIGHT, not the name from the
 * left — filenames may contain spaces, and only right-anchored parsing gets
 * "fil my notes.txt 4096" correct.
 *
 * Returns 1 when an entry was parsed into *out, 0 for the "end <count>" record
 * (with *count_out set), -1 for anything unrecognised — including an unframed
 * payload, which means the caller passed the wrong string.
 *
 * Names longer than OW_FILES_NAME_MAX-1 are truncated. A payload with no size
 * field yields size 0, tolerating firmware from before the listing fix. */
int ow_files_parse_fdir(const char* args, ow_dir_entry* out, uint32_t* count_out);

/* ---- Transfer ------------------------------------------------------------ */

/* Serial callbacks, structurally identical to ow_transport (onewili.h). Kept
 * separate so this header stays standalone.
 *   write: >=0 on success, <0 on error.
 *   read : bytes read (>0), 0 on timeout, <0 on error. */
typedef struct ow_files_io {
    void* ctx;
    int (*write)(void* ctx, const uint8_t* data, size_t len);
    int (*read)(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms);
} ow_files_io;

#ifndef OW_FILES_CHUNK
#define OW_FILES_CHUNK 64          /* payload bytes per write, as fwSerial uses */
#endif
#ifndef OW_FILES_IDLE_MS
#define OW_FILES_IDLE_MS 500       /* silence that ends a read phase */
#endif
#ifndef OW_FILES_LINE_MAX
#define OW_FILES_LINE_MAX 1024
#endif

ow_files_status ow_files_put(const ow_files_io* io, const char* dev_path,
                             const uint8_t* data, size_t len,
                             ow_progress_cb cb, void* cb_ctx);
ow_files_status ow_files_get(const ow_files_io* io, const char* dev_path,
                             uint8_t* buf, size_t cap, size_t* out_len,
                             ow_progress_cb cb, void* cb_ctx);
ow_files_status ow_files_list(const ow_files_io* io, const char* dir_path,
                              ow_dir_entry* out, size_t cap, size_t* out_count);

#ifdef __cplusplus
}
#endif

#ifdef OW_FILES_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OW_FILES_POLY 0xEDB88320u

void ow_files_crc32_begin(uint32_t* state) { *state = 0xFFFFFFFFu; }

void ow_files_crc32_update(uint32_t* state, const uint8_t* data, size_t len) {
    uint32_t crc = *state;
    for (size_t i = 0; i < len; ++i) {
        uint8_t octet = data[i];
        for (int bit = 8; --bit >= 0; octet >>= 1) {
            if ((crc ^ octet) & 1u) crc = (crc >> 1) ^ OW_FILES_POLY;
            else                    crc = (crc >> 1);
        }
    }
    *state = crc;
}

uint32_t ow_files_crc32_final(uint32_t state) { return ~state; }

uint32_t ow_files_crc32(const uint8_t* data, size_t len) {
    uint32_t st;
    ow_files_crc32_begin(&st);
    ow_files_crc32_update(&st, data, len);
    return ow_files_crc32_final(st);
}

size_t ow_files_build_put_header(char* buf, size_t cap, const char* dev_path,
                                 uint32_t size, uint32_t crc32) {
    if (!buf || !dev_path) return 0;
    int n = snprintf(buf, cap, "h\nx\nf\n%s %lu %lu\n", dev_path,
                     (unsigned long) size, (unsigned long) crc32);
    if (n < 0 || (size_t) n >= cap) return 0;
    return (size_t) n;
}

size_t ow_files_build_get_header(char* buf, size_t cap, const char* dev_path) {
    if (!buf || !dev_path) return 0;
    /* The device wants backslash-separated paths on this command, and the
     * proven implementation emits a space before the newline. */
    int n = snprintf(buf, cap, "h\nx\nu\n%s \n", dev_path);
    if (n < 0 || (size_t) n >= cap) return 0;
    for (size_t i = 0; i < (size_t) n; ++i)
        if (buf[i] == '/') buf[i] = '\\';
    return (size_t) n;
}

/* Advance past one whitespace-delimited token plus the spaces after it.
 * Returns NULL when there is no token left. */
static const char* ow_files_skip_token(const char* p) {
    if (!p) return NULL;
    while (*p == ' ') ++p;
    if (!*p) return NULL;
    while (*p && *p != ' ') ++p;
    while (*p == ' ') ++p;
    return p;
}

/* Start of the last whitespace-delimited token in [begin,end), or NULL. */
static const char* ow_files_last_token(const char* begin, const char* end) {
    while (end > begin && end[-1] == ' ') --end;
    if (end == begin) return NULL;
    const char* p = end;
    while (p > begin && p[-1] != ' ') --p;
    return p;
}

static bool ow_files_all_digits(const char* p, const char* end) {
    if (p >= end) return false;
    for (; p < end; ++p) if (*p < '0' || *p > '9') return false;
    return true;
}

int ow_files_parse_fdir(const char* args, ow_dir_entry* out, uint32_t* count_out) {
    if (!args || !out) return -1;

    /* Strip the event framing: <hexTimestamp> <sequence> ... <ok>. */
    const char* p = ow_files_skip_token(args);          /* past timestamp */
    p = ow_files_skip_token(p);                         /* past sequence  */
    if (!p || !*p) return -1;

    const char* end = p + strlen(p);
    while (end > p && (end[-1] == ' ' || end[-1] == ']')) --end;
    const char* ok = ow_files_last_token(p, end);       /* trailing success flag */
    if (!ok) return -1;
    end = ok;                                           /* payload ends before it */
    while (end > p && end[-1] == ' ') --end;
    if (end == p) return -1;

    /* payload is now [p,end): "dir <name> <size>" / "fil <name> <size>" /
     * "end <count>". */
    bool is_dir;
    if      (end - p > 4 && !strncmp(p, "dir ", 4)) is_dir = true;
    else if (end - p > 4 && !strncmp(p, "fil ", 4)) is_dir = false;
    else if (end - p > 4 && !strncmp(p, "end ", 4)) {
        if (count_out) *count_out = (uint32_t) strtoul(p + 4, NULL, 10);
        return 0;
    }
    else return -1;

    const char* name = p + 4;
    while (name < end && *name == ' ') ++name;
    if (name >= end) return -1;

    /* Size is the last token of the payload -- but only if it is numeric.
     * Firmware from before the listing fix omits it, and a filename is not
     * mistaken for a size because of the all-digits test. Parsing from the
     * right is what lets names contain spaces.
     *
     * KNOWN, UNRESOLVABLE AMBIGUITY: a legacy sizeless entry whose name has
     * two or more words and whose last word happens to be all-digits -- e.g.
     * payload "fil run 2024" with no size field -- is byte-identical on the
     * wire to a SIZED entry named "run" of size 2024. This function has no
     * way to tell them apart and will report the latter (name "run", size
     * 2024) for both. No parser can do better from this string alone; the
     * information needed to disambiguate was never put on the wire by the
     * legacy firmware. This only matters when talking to an unflashed,
     * pre-listing-fix device -- current firmware always emits the size field,
     * so any SIZED entry (the normal case) is always parsed correctly
     * regardless of how many words are in its name. A single-token numeric
     * legacy name (e.g. "fil 2024" with no size) is also unaffected: the
     * `last > name` check below requires at least one token before the
     * candidate size, so a lone numeric name is correctly left as the name
     * with size 0. */
    uint32_t size = 0;
    const char* nameEnd = end;
    const char* last = ow_files_last_token(name, end);
    if (last && last > name && ow_files_all_digits(last, end)) {
        size = (uint32_t) strtoul(last, NULL, 10);
        nameEnd = last;
        while (nameEnd > name && nameEnd[-1] == ' ') --nameEnd;
    }
    if (nameEnd <= name) return -1;

    size_t nameLen = (size_t)(nameEnd - name);
    if (nameLen > OW_FILES_NAME_MAX - 1) nameLen = OW_FILES_NAME_MAX - 1;
    memcpy(out->name, name, nameLen);
    out->name[nameLen] = '\0';
    out->is_dir = is_dir;
    out->size   = size;
    return 1;
}

#define OW_FILES_EMPTY_READS 2      /* consecutive zero-byte reads == silence */

/* Shared byte-level reader. Buffers whatever arrives; callers pull lines or
 * raw bytes out of it. */
typedef struct {
    const ow_files_io* io;
    uint8_t  buf[OW_FILES_LINE_MAX];
    size_t   len;
    size_t   pos;
    int      empties;
} ow_files_rx;

static void ow_files_rx_init(ow_files_rx* rx, const ow_files_io* io) {
    rx->io = io; rx->len = 0; rx->pos = 0; rx->empties = 0;
}

/* Refill when drained. Returns 1 = bytes available, 0 = silence, -1 = io error. */
static int ow_files_rx_fill(ow_files_rx* rx) {
    if (rx->pos < rx->len) return 1;
    while (rx->empties < OW_FILES_EMPTY_READS) {
        int n = rx->io->read(rx->io->ctx, rx->buf, sizeof rx->buf, OW_FILES_IDLE_MS);
        if (n < 0) return -1;
        if (n > 0) { rx->len = (size_t) n; rx->pos = 0; rx->empties = 0; return 1; }
        rx->empties++;
    }
    return 0;
}

static int ow_files_rx_byte(ow_files_rx* rx, uint8_t* out) {
    int r = ow_files_rx_fill(rx);
    if (r <= 0) return r;
    *out = rx->buf[rx->pos++];
    return 1;
}

/* Read one '\n'-terminated line (terminator dropped, NUL added).
 * 1 = line in `line`, 0 = silence, -1 = io error. */
static int ow_files_read_line(ow_files_rx* rx, char* line, size_t cap) {
    size_t n = 0;
    for (;;) {
        uint8_t c;
        int r = ow_files_rx_byte(rx, &c);
        if (r < 0) return r;                            /* io error: never a bogus success */
        if (r == 0) { if (n == 0) return r; break; }    /* silence ends a partial line */
        if (c == '\n') break;
        if (c != '\r' && n + 1 < cap) line[n++] = (char) c;
    }
    line[n] = '\0';
    return 1;
}

/* If `line` is "[*<id> <args>]", return a pointer to <args> and NUL-terminate
 * it in place. NULL when the line is not an event for `id`. */
static char* ow_files_event_args(char* line, const char* id) {
    if (line[0] != '[' || line[1] != '*') return NULL;
    size_t idLen = strlen(id);
    if (strncmp(line + 2, id, idLen) != 0) return NULL;
    if (line[2 + idLen] != ' ') return NULL;
    char* args = line + 3 + idLen;
    size_t n = strlen(args);
    while (n > 0 && (args[n - 1] == ']' || args[n - 1] == ' ')) args[--n] = '\0';
    return args;
}

/* Parse ANY framed (non-event) response line "[<tag> <ts> <seq> <body...>
 * <ok>]" into its tag and body, both NUL-terminated in place. Deliberately
 * does not interpret or validate the tag: a handshake frame (opened by
 * fwMenuX::callSubFunction) is tagged with the full navigation prefix to
 * that menu (e.g. "h\x\f"), while the later completion frame (emitted
 * directly by fwMenuFileSystem via rpConsole::printMenuResponse with a
 * literal string) uses a different, fixed tag (e.g. "x\f") -- see the put()/
 * get() doc comments below for the concrete shapes. Callers that need to
 * distinguish frames by tag (the completion loops) do their own tokenizing
 * via ow_files_tokenize instead of this helper; this one is for the
 * handshake loops, which must match on body content only and tolerate
 * whatever tag arrives.
 *
 * Returns false for anything not shaped like a framed response: an event
 * line ("[*..."), chatter with no leading '[', or a line with no trailing
 * 0/1 flag token. */
static bool ow_files_parse_frame(char* line, char** tag_out, char** body_out) {
    if (line[0] != '[' || line[1] == '*') return false;
    char* p = line + 1;
    char* tag = p;
    while (*p && *p != ' ') ++p;
    if (!*p) return false;
    *p++ = '\0';                                    /* NUL-terminate the tag */

    p = (char*) ow_files_skip_token(p);              /* past timestamp */
    p = (char*) ow_files_skip_token(p);              /* past sequence  */
    if (!p || !*p) return false;

    char* end = p + strlen(p);
    while (end > p && (end[-1] == ' ' || end[-1] == ']')) --end;
    char* ok = (char*) ow_files_last_token(p, end);
    if (!ok) return false;
    if (!((ok[0] == '0' || ok[0] == '1') && ok + 1 == end)) return false;

    char* bodyEnd = ok;
    while (bodyEnd > p && bodyEnd[-1] == ' ') --bodyEnd;
    *bodyEnd = '\0';                                 /* NUL-terminate the body */

    *tag_out = tag;
    *body_out = p;
    return true;
}

static ow_files_status ow_files_reset(const ow_files_io* io) {
    static const uint8_t kReset[2] = { 0x03, '\n' };
    return io->write(io->ctx, kReset, 2) < 0 ? OW_FILES_ERR_IO : OW_FILES_OK;
}

#define OW_FILES_RESPONSE_TOKENS_MAX 16   /* generous for any real x\f/x\u line */

/* Split a NUL-terminated line into whitespace-delimited tokens, destructively
 * (each token gets its own terminating NUL written in place of the delimiter
 * that followed it), storing up to `maxTok` of them in `tok`. A leading '['
 * and the first ']' encountered are treated as delimiters, not token
 * characters, so a framed response line's tokens come out the same way an
 * unframed one's would.
 *
 * Returns the TRUE token count, which may be greater than `maxTok` when the
 * line has more tokens than fit -- tokens past the cap are scanned (so
 * parsing still reaches the closing ']') but NOT stored, so `tok[maxTok]`
 * and beyond do not hold real data. Callers MUST treat a returned count
 * greater than `maxTok` as "too many fields to trust" and fail closed
 * rather than index `tok[n-1]` as though it were the line's real last
 * token -- that slot would actually be `tok[maxTok-1]`, some interior field,
 * not the last one. */
static int ow_files_tokenize(char* line, char** tok, int maxTok) {
    int n = 0;
    char* p = line;
    if (*p == '[') ++p;
    while (*p) {
        while (*p == ' ') ++p;
        if (!*p) break;
        char* start = p;
        while (*p && *p != ' ' && *p != ']') ++p;
        bool hitBracket = (*p == ']');
        if (n < maxTok) tok[n] = start;
        ++n;
        if (*p) { *p = '\0'; ++p; }
        if (hitBracket) break;
    }
    return n;
}

/* A framed command response is "[<tag> <ts> <seq> <payload...> <ok>]" --
 * whatever the payload's own shape, the LAST token is always the trailing
 * ok flag (rpConsole::printEventResponse). Unlike ow_files_parse_frame (used
 * for the tag-tolerant handshake loops), this DOES require an exact tag
 * match: the completion frame's tag is a fixed literal the firmware source
 * hardcodes at the printMenuResponse call site (e.g. "x\f"), not something
 * that varies with menu navigation, so pinning to it here is safe and lets
 * an unrelated framed response (a different in-flight command's answer, for
 * instance) be skipped rather than misread as this transfer's result.
 *
 * Returns 1 (ok) or 0 (not ok) on a tag match; -1 if `line` isn't a framed
 * response at all OR its tag doesn't match `tag` (caller should keep
 * waiting); -2 if the tag matches but the trailing flag isn't a
 * recognisable single digit (this includes the line having MORE than
 * OW_FILES_RESPONSE_TOKENS_MAX tokens -- `ow_files_tokenize` did not store
 * the real last one, so guessing from `tok[n-1]` would risk reading an
 * unrelated interior field; caller should treat this as a protocol error,
 * not keep waiting). */
static int ow_files_response_ok(char* line, const char* tag) {
    char* tok[OW_FILES_RESPONSE_TOKENS_MAX];
    int n = ow_files_tokenize(line, tok, OW_FILES_RESPONSE_TOKENS_MAX);
    if (n < 1 || strcmp(tok[0], tag) != 0) return -1;
    if (n > OW_FILES_RESPONSE_TOKENS_MAX) return -2;
    const char* ok = tok[n - 1];
    if (ok[0] == '1' && ok[1] == '\0') return 1;
    if (ok[0] == '0' && ok[1] == '\0') return 0;
    return -2;
}

/* A `get` trailer's payload is "success <N> bytes <CRC> crc" (fwMenuFileSystem
 * .cpp's doUpload), framed the usual way, so the full line reads
 * "[x\u <ts> <seq> success <N> bytes <CRC> crc <ok>]" -- note the fixed
 * "x\u" tag, not the handshake's "h\x\u" (see ow_files_parse_frame). As with
 * ow_files_response_ok, the tag is checked exactly: it's a literal the
 * firmware source hardcodes at the printMenuResponse call site, not derived
 * from navigation. Right-anchored beyond that: the last token is the ok
 * flag, the one before it the literal word "crc", and the one before THAT
 * the crc value -- checking the literal guards against silently reading the
 * wrong field if the payload shape is off.
 *
 * Sets *tag_matched to whether `tag` matched at all (false: caller should
 * keep waiting for the real trailer; true but return false: this WAS the
 * trailer frame but its payload didn't parse -- caller should treat that as
 * a protocol error, not keep waiting). Returns true and sets *out only on a
 * full match (caller must not invent a crc from a false-shaped line,
 * including one with more than OW_FILES_RESPONSE_TOKENS_MAX tokens -- see
 * ow_files_tokenize). */
static bool ow_files_extract_trailer_crc(char* line, const char* tag, uint32_t* out,
                                         bool* tag_matched) {
    *tag_matched = false;
    char* tok[OW_FILES_RESPONSE_TOKENS_MAX];
    int n = ow_files_tokenize(line, tok, OW_FILES_RESPONSE_TOKENS_MAX);
    if (n < 1 || strcmp(tok[0], tag) != 0) return false;
    *tag_matched = true;
    if (n < 3 || n > OW_FILES_RESPONSE_TOKENS_MAX) return false;
    if (strcmp(tok[n - 2], "crc") != 0) return false;
    char* end;
    unsigned long v = strtoul(tok[n - 3], &end, 10);
    if (end == tok[n - 3] || *end != '\0') return false;
    *out = (uint32_t) v;
    return true;
}

/* Ground truth for the handshakes below is firmware MenuX/fwMenuFileSystem.cpp:
 * getFileFromPC/doDownload (put, line 156/798) and sendFileToPC/doUpload
 * (get, line 183/653), by way of fwMenuX::callSubFunction (rmpLib/fwMenuX.cpp),
 * which opens a normal menu RESPONSE FRAME before calling the handler and
 * closes it right after (neither handler defers m_bFinishRepsonse). So
 * "Send File Now" / "Invalid" / "RxFile <size>" / "CantOpenFile" are each the
 * BODY of a framed response, not a bare line on their own:
 *
 *     [h\x\f <hexTimestampNs> <seq> Send File Now 1]
 *     [h\x\u <hexTimestampNs> <seq> RxFile 4096 1]
 *
 * and the frame's tag is the full navigation prefix to that menu ("h\x\f" for
 * put, "h\x\u" for get) -- a DIFFERENT tag from the completion frame below
 * ("x\f"/"x\u"), which comes from a direct printMenuResponse call with a
 * fixed literal, not through callSubFunction. That's why the handshake
 * parsing below (ow_files_parse_frame) is tag-tolerant -- matching on body
 * content only -- while the completion parsing (ow_files_response_ok /
 * ow_files_extract_trailer_crc) pins to the exact literal tag. Both
 * handshake lines are emitted with printOutAlways so quiet mode can't
 * suppress them. */

ow_files_status ow_files_put(const ow_files_io* io, const char* dev_path,
                             const uint8_t* data, size_t len,
                             ow_progress_cb cb, void* cb_ctx) {
    if (!io || !io->write || !io->read || !dev_path || (len && !data)) return OW_FILES_ERR_ARG;

    ow_files_status st = ow_files_reset(io);
    if (st != OW_FILES_OK) return st;

    char hdr[OW_FILES_LINE_MAX];
    size_t hn = ow_files_build_put_header(hdr, sizeof hdr, dev_path,
                                          (uint32_t) len, ow_files_crc32(data, len));
    if (hn == 0) return OW_FILES_ERR_BUFFER;
    if (io->write(io->ctx, (const uint8_t*) hdr, hn) < 0) return OW_FILES_ERR_IO;

    ow_files_rx rx;
    ow_files_rx_init(&rx, io);
    char line[OW_FILES_LINE_MAX];

    /* Framed handshake (tag-tolerant, body-matched -- see the ground-truth
     * comment above): "Send File Now" clears us to stream; "Invalid" (a
     * rejected path/size/crc line) means not one payload byte should go
     * out. */
    for (;;) {
        int r = ow_files_read_line(&rx, line, sizeof line);
        if (r < 0) return OW_FILES_ERR_IO;
        if (r == 0) return OW_FILES_ERR_TIMEOUT;
        char* tag; char* body;
        if (!ow_files_parse_frame(line, &tag, &body)) continue;   /* event/chatter */
        (void) tag;   /* tag-tolerant on purpose -- see the ground-truth comment above */
        if (strcmp(body, "Send File Now") == 0) break;
        if (strcmp(body, "Invalid") == 0) return OW_FILES_ERR_FAILED;
        /* some other framed response: unrelated, keep waiting for the handshake */
    }

    size_t sent = 0;
    while (sent < len) {
        size_t chunk = len - sent;
        if (chunk > OW_FILES_CHUNK) chunk = OW_FILES_CHUNK;
        if (io->write(io->ctx, data + sent, chunk) < 0) return OW_FILES_ERR_IO;
        sent += chunk;
        if (cb) cb(cb_ctx, sent, len);
    }

    /* The device verifies its own CRC after the payload lands and reports
     * the result as a framed "x\f" response: ok 1 on "success <N> bytes", ok
     * 0 on "Failed checksum" (and it deletes the file in that case). */
    for (;;) {
        int r = ow_files_read_line(&rx, line, sizeof line);
        if (r < 0) return OW_FILES_ERR_IO;
        if (r == 0) return OW_FILES_ERR_TIMEOUT;
        int ok = ow_files_response_ok(line, "x\\f");
        if (ok == -1) continue;                     /* not our completion frame yet */
        if (ok == -2) return OW_FILES_ERR_PROTOCOL;  /* our tag, but malformed */
        return ok ? OW_FILES_OK : OW_FILES_ERR_FAILED;
    }
}

ow_files_status ow_files_get(const ow_files_io* io, const char* dev_path,
                             uint8_t* buf, size_t cap, size_t* out_len,
                             ow_progress_cb cb, void* cb_ctx) {
    if (!io || !io->write || !io->read || !dev_path || !buf) return OW_FILES_ERR_ARG;
    if (out_len) *out_len = 0;

    ow_files_status st = ow_files_reset(io);
    if (st != OW_FILES_OK) return st;

    char hdr[OW_FILES_LINE_MAX];
    size_t hn = ow_files_build_get_header(hdr, sizeof hdr, dev_path);
    if (hn == 0) return OW_FILES_ERR_BUFFER;
    if (io->write(io->ctx, (const uint8_t*) hdr, hn) < 0) return OW_FILES_ERR_IO;

    ow_files_rx rx;
    ow_files_rx_init(&rx, io);
    char line[OW_FILES_LINE_MAX];

    /* Framed handshake (tag-tolerant, body-matched -- see the ground-truth
     * comment above): "RxFile <size>" clears us to read the payload -- size
     * ONLY, no crc yet; the crc arrives in the trailer, AFTER the payload.
     * "Invalid" (bad path syntax) or "CantOpenFile" (no such file) both mean
     * there is no payload to read. */
    unsigned long size = 0;
    for (;;) {
        int r = ow_files_read_line(&rx, line, sizeof line);
        if (r < 0) return OW_FILES_ERR_IO;
        if (r == 0) return OW_FILES_ERR_TIMEOUT;
        char* tag; char* body;
        if (!ow_files_parse_frame(line, &tag, &body)) continue;   /* event/chatter */
        (void) tag;   /* tag-tolerant on purpose -- see the ground-truth comment above */
        if (strcmp(body, "Invalid") == 0 || strcmp(body, "CantOpenFile") == 0)
            return OW_FILES_ERR_FAILED;
        if (strncmp(body, "RxFile ", 7) == 0) {
            char* end;
            size = strtoul(body + 7, &end, 10);
            if (end == body + 7) return OW_FILES_ERR_PROTOCOL;   /* no number there */
            break;
        }
        /* some other framed response: unrelated, keep waiting for the handshake */
    }

    if (size > cap) {
        /* The device is about to stream `size` payload bytes regardless of
         * whether we can hold them (it already committed to that count in
         * the handshake above). Leaving them unread on the wire would bleed
         * into whatever the next caller reads next, so drain and discard
         * them before reporting the error -- best effort: if the drain
         * itself times out or hits an IO error, report ERR_BUFFER anyway
         * rather than masking the real problem behind a drain failure. */
        size_t toDrain = size;
        while (toDrain > 0) {
            uint8_t c;
            int r = ow_files_rx_byte(&rx, &c);
            if (r <= 0) break;
            --toDrain;
        }
        return OW_FILES_ERR_BUFFER;
    }

    size_t got = 0;
    while (got < size) {
        uint8_t c;
        int r = ow_files_rx_byte(&rx, &c);
        if (r < 0) return OW_FILES_ERR_IO;
        if (r == 0) return OW_FILES_ERR_TIMEOUT;
        buf[got++] = c;
        if (cb && (got % OW_FILES_CHUNK) == 0) cb(cb_ctx, got, size);
    }
    if (cb) cb(cb_ctx, got, size);

    /* The crc lives ONLY in this trailer, a framed "x\u" response with body
     * "success <N> bytes <CRC> crc". No trailer, or one we can't parse,
     * means we do not have a crc to trust -- report a protocol error rather
     * than silently accepting the payload unverified. */
    for (;;) {
        int r = ow_files_read_line(&rx, line, sizeof line);
        if (r < 0) return OW_FILES_ERR_IO;
        if (r == 0) return OW_FILES_ERR_PROTOCOL;          /* no trailer: don't invent a crc */

        uint32_t crc;
        bool tagMatched = false;
        if (!ow_files_extract_trailer_crc(line, "x\\u", &crc, &tagMatched)) {
            if (!tagMatched) continue;          /* not our trailer yet: keep waiting */
            return OW_FILES_ERR_PROTOCOL;       /* our tag, but malformed payload */
        }
        if (ow_files_crc32(buf, got) != crc) return OW_FILES_ERR_PROTOCOL;
        break;
    }

    if (out_len) *out_len = got;
    return OW_FILES_OK;
}

ow_files_status ow_files_list(const ow_files_io* io, const char* dir_path,
                              ow_dir_entry* out, size_t cap, size_t* out_count) {
    if (!io || !io->write || !io->read || !out) return OW_FILES_ERR_ARG;
    if (out_count) *out_count = 0;

    ow_files_status st = ow_files_reset(io);
    if (st != OW_FILES_OK) return st;

    char cmd[OW_FILES_LINE_MAX];
    int cn = snprintf(cmd, sizeof cmd, "h\nx\nl\n%s\n", dir_path ? dir_path : "");
    if (cn < 0 || (size_t) cn >= sizeof cmd) return OW_FILES_ERR_BUFFER;
    if (io->write(io->ctx, (const uint8_t*) cmd, (size_t) cn) < 0) return OW_FILES_ERR_IO;

    ow_files_rx rx;
    ow_files_rx_init(&rx, io);

    size_t n = 0;
    bool overflow = false;
    char line[OW_FILES_LINE_MAX];
    for (;;) {
        int r = ow_files_read_line(&rx, line, sizeof line);
        if (r < 0) return OW_FILES_ERR_IO;
        if (r == 0) break;                       /* silence: legacy firmware, no end record */

        char* args = ow_files_event_args(line, "fdir");
        if (!args) continue;                     /* the command's own response, etc. */

        ow_dir_entry e;
        uint32_t count = 0;
        int k = ow_files_parse_fdir(args, &e, &count);
        if (k < 0) continue;
        if (k == 0) break;                       /* end record */
        if (n < cap) out[n++] = e;
        else overflow = true;
    }

    if (out_count) *out_count = n;
    return overflow ? OW_FILES_ERR_BUFFER : OW_FILES_OK;
}

#endif /* OW_FILES_IMPLEMENTATION */
#endif /* OW_FILES_H */
