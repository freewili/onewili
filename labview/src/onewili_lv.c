/* OneWili LabVIEW ABI -- core implementation.
 *
 * Hand-written (the command forwarders in onewili_lv_api.c are generated).
 * Everything here exists to turn the C package's pointer-and-callback API
 * into something a Call Library Function Node can drive: integer session
 * handles instead of pointers, caller-sized string buffers instead of
 * ow_device*, polled latches instead of callbacks.
 */

#include "onewili_lv_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <setupapi.h>
#else
#include <dirent.h>
#endif

#define OWLV_VERSION_MAJOR 1
#define OWLV_VERSION_MINOR 0
#define OWLV_VERSION_PATCH 0

/* ── locking ───────────────────────────────────────────────────────────── */

#ifdef _WIN32
static void owlv__lock_init(owlv_mutex* m)    { InitializeCriticalSection(m); }
static void owlv__lock(owlv_mutex* m)         { EnterCriticalSection(m); }
static void owlv__unlock(owlv_mutex* m)       { LeaveCriticalSection(m); }
#else
static void owlv__lock_init(owlv_mutex* m)    { pthread_mutex_init(m, NULL); }
static void owlv__lock(owlv_mutex* m)         { pthread_mutex_lock(m); }
static void owlv__unlock(owlv_mutex* m)       { pthread_mutex_unlock(m); }
#endif

/* ── globals ───────────────────────────────────────────────────────────── */

static owlv_session g_sessions[OWLV_MAX_SESSIONS];
static owlv_mutex   g_table_lock;
static int          g_table_ready = 0;
static int          g_next_id = 1;
static char         g_global_err[OWLV_ERRLEN];

typedef struct {
    char name[64];
    char desc[192];
    int  vid, pid;
    int  kind;
} owlv_port_info;

static owlv_port_info g_ports[OWLV_MAX_PORTS];
static int            g_port_count;

/* Every session's lock is created here and never destroyed: owlv__acquire
 * takes it after dropping the table lock, so a concurrent owlv_close must not
 * be able to pull the lock out from under it. Closing clears the session's
 * fields (leaving the lock, the last member, alone) and zeroes its id, which
 * owlv__acquire re-checks once it holds the lock. */
static void owlv__init_once(void)
{
    int i;
    owlv__lock_init(&g_table_lock);
    for (i = 0; i < OWLV_MAX_SESSIONS; ++i)
        owlv__lock_init(&g_sessions[i].lock);
    g_table_ready = 1;
}

/* LabVIEW does not unload the DLL between VI runs, so initialising once per
 * process is enough -- but two Call Library Function Nodes in parallel loops
 * can reach here simultaneously on the very first call, so it is guarded. */
static void owlv__init(void)
{
#ifdef _WIN32
    static LONG once = 0;
    if (InterlockedCompareExchange(&once, 1, 0) == 0)
        owlv__init_once();
    while (!g_table_ready) Sleep(0);
#else
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once(&once, owlv__init_once);
#endif
}

/* Zero a session without touching its lock, which must outlive the session. */
static void owlv__reset(owlv_session* s)
{
    memset(s, 0, offsetof(owlv_session, lock));
    s->last_kind = OWLV_EV_NONE;
}

/* ── small helpers ─────────────────────────────────────────────────────── */

/* Copy a NUL-terminated string into a caller-sized buffer. A NULL buffer or a
 * cap of 0 means "the caller does not want this value", which is not an
 * error -- LabVIEW users routinely leave optional string outputs unwired. */
static int owlv__str_out(const char* src, char* buf, int cap)
{
    size_t n;
    if (!buf || cap <= 0) return OWLV_OK;
    if (!src) src = "";
    n = strlen(src);
    if (n + 1 > (size_t)cap) {
        memcpy(buf, src, (size_t)cap - 1);
        buf[cap - 1] = 0;
        return OWLV_ERR_BUFFER;
    }
    memcpy(buf, src, n + 1);
    return OWLV_OK;
}

static void owlv__set_err(char* dst, const char* what, ow_status r)
{
    static const char* names[] = {
        "ok", "bad argument", "serial I/O error", "timed out",
        "device reported failure", "protocol error", "buffer too small"
    };
    const char* msg = ((int)r >= 0 && (int)r < 7) ? names[(int)r] : "unknown error";
    if (r == OW_OK) { dst[0] = 0; return; }
    snprintf(dst, OWLV_ERRLEN, "%s: %s", what ? what : "onewili", msg);
}

/* Defined with the binary-event accessors below; both event paths split the
 * device's 64-bit timestamps into two U32 halves for LabVIEW. */
static void owlv__split64(unsigned long long v, unsigned int* lo, unsigned int* hi);

owlv_session* owlv__acquire(int session)
{
    int i;
    owlv_session* s = NULL;
    owlv__init();
    if (session <= 0) return NULL;
    owlv__lock(&g_table_lock);
    for (i = 0; i < OWLV_MAX_SESSIONS; ++i) {
        if (g_sessions[i].id == session) { s = &g_sessions[i]; break; }
    }
    owlv__unlock(&g_table_lock);
    if (!s) return NULL;
    owlv__lock(&s->lock);
    /* Re-check: owlv_close could have won the race for the table lock. */
    if (s->id != session) { owlv__unlock(&s->lock); return NULL; }
    return s;
}

void owlv__release(owlv_session* s, ow_status r, const char* what)
{
    if (!s) return;
    owlv__set_err(s->err, what, r);
    owlv__unlock(&s->lock);
}

/* ── library information ───────────────────────────────────────────────── */

int owlv_version(int* major, int* minor, int* patch)
{
    if (major) *major = OWLV_VERSION_MAJOR;
    if (minor) *minor = OWLV_VERSION_MINOR;
    if (patch) *patch = OWLV_VERSION_PATCH;
    return OWLV_OK;
}

int owlv_pointer_size(void)
{
    return (int)sizeof(void*);
}

int owlv_status_message(int status, char* buf, int cap)
{
    const char* m;
    switch (status) {
    case OWLV_OK:              m = "ok"; break;
    case OWLV_ERR_ARG:         m = "bad argument"; break;
    case OWLV_ERR_IO:          m = "serial I/O error"; break;
    case OWLV_ERR_TIMEOUT:     m = "timed out waiting for the device"; break;
    case OWLV_ERR_FAILED:      m = "the device rejected the command"; break;
    case OWLV_ERR_PROTOCOL:    m = "malformed response frame"; break;
    case OWLV_ERR_BUFFER:      m = "output buffer too small"; break;
    case OWLV_ERR_SESSION:     m = "invalid or closed session handle"; break;
    case OWLV_ERR_NO_DEVICE:   m = "no matching FreeWili port found"; break;
    case OWLV_ERR_OPEN:        m = "could not open the serial port"; break;
    case OWLV_ERR_LIMIT:       m = "too many open sessions"; break;
    case OWLV_ERR_STATE:       m = "not valid in this state"; break;
    case OWLV_ERR_UNSUPPORTED: m = "not supported on this platform"; break;
    default:                   m = "unknown status"; break;
    }
    return owlv__str_out(m, buf, cap);
}

int owlv_last_error(int session, char* buf, int cap)
{
    owlv_session* s;
    int r;
    owlv__init();
    if (session == 0) return owlv__str_out(g_global_err, buf, cap);
    s = owlv__acquire(session);
    if (!s) return OWLV_ERR_SESSION;
    r = owlv__str_out(s->err, buf, cap);
    owlv__unlock(&s->lock);
    return r;
}

/* ── port discovery ────────────────────────────────────────────────────── */

/* From freewili-finder's usbdef.hpp. */
static int owlv__kind_for(int vid, int pid)
{
    if (vid == 0x093C && (pid == 0x205A || pid == 0x2054)) return OWLV_PORT_FW_MAIN;
    if (vid == 0x093C && (pid == 0x2060 || pid == 0x2055)) return OWLV_PORT_FW_DISPLAY;
    if (vid == 0x0403 &&  pid == 0x6014)                   return OWLV_PORT_FW_BINARY;
    if (vid == 0x2E8A &&  pid == 0x000C)                   return OWLV_PORT_FW_DEBUG;
    if (vid == 0x303A &&  pid == 0x1001)                   return OWLV_PORT_FW_ESP32;
    return OWLV_PORT_UNKNOWN;
}

#ifdef _WIN32
/* {4D36E978-E325-11CE-BFC1-08002BE10318} -- GUID_DEVCLASS_PORTS. Spelled out
 * so we do not need INITGUID or a link against uuid.lib. */
static const GUID owlv__guid_ports = {
    0x4D36E978, 0xE325, 0x11CE,
    {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}
};

static int owlv__com_number(const char* name)
{
    /* "COM12" -> 12; anything else sorts last. */
    if (strncmp(name, "COM", 3) != 0) return 1 << 20;
    return atoi(name + 3);
}

static void owlv__scan_ports(void)
{
    HDEVINFO di;
    SP_DEVINFO_DATA dd;
    DWORD i;

    g_port_count = 0;
    di = SetupDiGetClassDevsA(&owlv__guid_ports, NULL, NULL, DIGCF_PRESENT);
    if (di == INVALID_HANDLE_VALUE) return;

    memset(&dd, 0, sizeof dd);
    dd.cbSize = sizeof dd;
    for (i = 0; SetupDiEnumDeviceInfo(di, i, &dd); ++i) {
        char port[64] = {0}, desc[192] = {0}, hwid[512] = {0};
        DWORD cb, type = 0;
        HKEY key;
        const char* p;
        owlv_port_info* e;

        if (g_port_count >= OWLV_MAX_PORTS) break;

        key = SetupDiOpenDevRegKey(di, &dd, DICS_FLAG_GLOBAL, 0,
                                   DIREG_DEV, KEY_READ);
        if (key == INVALID_HANDLE_VALUE) continue;
        cb = sizeof port;
        if (RegQueryValueExA(key, "PortName", NULL, &type,
                             (LPBYTE)port, &cb) != ERROR_SUCCESS)
            port[0] = 0;
        RegCloseKey(key);
        if (!port[0]) continue;
        /* LPT ports live in the same class. */
        if (strncmp(port, "COM", 3) != 0) continue;

        SetupDiGetDeviceRegistryPropertyA(di, &dd, SPDRP_FRIENDLYNAME, NULL,
                                          (PBYTE)desc, sizeof desc, NULL);
        SetupDiGetDeviceRegistryPropertyA(di, &dd, SPDRP_HARDWAREID, NULL,
                                          (PBYTE)hwid, sizeof hwid, NULL);

        e = &g_ports[g_port_count++];
        memset(e, 0, sizeof *e);
        snprintf(e->name, sizeof e->name, "%s", port);
        snprintf(e->desc, sizeof e->desc, "%s", desc);
        p = strstr(hwid, "VID_");
        if (p) e->vid = (int)strtol(p + 4, NULL, 16);
        p = strstr(hwid, "PID_");
        if (p) e->pid = (int)strtol(p + 4, NULL, 16);
        e->kind = owlv__kind_for(e->vid, e->pid);
    }
    SetupDiDestroyDeviceInfoList(di);

    /* COM3 before COM10: insertion sort, the list is tiny. */
    {
        int a, b;
        for (a = 1; a < g_port_count; ++a) {
            owlv_port_info tmp = g_ports[a];
            int key = owlv__com_number(tmp.name);
            for (b = a - 1;
                 b >= 0 && owlv__com_number(g_ports[b].name) > key; --b)
                g_ports[b + 1] = g_ports[b];
            g_ports[b + 1] = tmp;
        }
    }
}

#else /* POSIX */

/* No VID/PID here: identifying a FreeWili on Linux/macOS needs libudev or
 * IOKit, which this wrapper deliberately does not link. Kinds come back as
 * OWLV_PORT_UNKNOWN and owlv_open_auto falls back to "the only candidate". */
static void owlv__scan_ports(void)
{
    static const char* dirs[] = {"/dev"};
    DIR* d;
    struct dirent* de;
    size_t k;

    g_port_count = 0;
    for (k = 0; k < sizeof dirs / sizeof dirs[0]; ++k) {
        d = opendir(dirs[k]);
        if (!d) continue;
        while ((de = readdir(d)) != NULL) {
            const char* n = de->d_name;
            int match = strncmp(n, "ttyACM", 6) == 0 ||
                        strncmp(n, "ttyUSB", 6) == 0 ||
                        strncmp(n, "cu.usbmodem", 11) == 0 ||
                        strncmp(n, "cu.usbserial", 12) == 0;
            owlv_port_info* e;
            if (!match || g_port_count >= OWLV_MAX_PORTS) continue;
            e = &g_ports[g_port_count++];
            memset(e, 0, sizeof *e);
            snprintf(e->name, sizeof e->name, "%s/%s", dirs[k], n);
            snprintf(e->desc, sizeof e->desc, "%s", n);
            e->kind = OWLV_PORT_UNKNOWN;
        }
        closedir(d);
    }
}
#endif

int owlv_refresh_ports(int* count)
{
    owlv__init();
    owlv__lock(&g_table_lock);
    owlv__scan_ports();
    if (count) *count = g_port_count;
    owlv__unlock(&g_table_lock);
    return OWLV_OK;
}

static int owlv__port_field(int index, int which, char* buf, int cap,
                            int* a, int* b)
{
    int r = OWLV_OK;
    owlv__init();
    owlv__lock(&g_table_lock);
    if (index < 0 || index >= g_port_count) {
        r = OWLV_ERR_ARG;
    } else {
        owlv_port_info* e = &g_ports[index];
        switch (which) {
        case 0: r = owlv__str_out(e->name, buf, cap); break;
        case 1: r = owlv__str_out(e->desc, buf, cap); break;
        case 2: if (a) *a = e->kind; break;
        default:
            if (a) *a = e->vid;
            if (b) *b = e->pid;
            break;
        }
    }
    owlv__unlock(&g_table_lock);
    return r;
}

int owlv_port_name(int index, char* buf, int cap)
{ return owlv__port_field(index, 0, buf, cap, NULL, NULL); }

int owlv_port_description(int index, char* buf, int cap)
{ return owlv__port_field(index, 1, buf, cap, NULL, NULL); }

int owlv_port_kind(int index, int* kind)
{ return owlv__port_field(index, 2, NULL, 0, kind, NULL); }

int owlv_port_usb_ids(int index, int* vid, int* pid)
{ return owlv__port_field(index, 3, NULL, 0, vid, pid); }

int owlv_find_port(int kind, char* buf, int cap)
{
    int i, r = OWLV_ERR_NO_DEVICE;
    owlv__init();
    owlv__lock(&g_table_lock);
    owlv__scan_ports();
    for (i = 0; i < g_port_count; ++i) {
        if (g_ports[i].kind == kind) {
            r = owlv__str_out(g_ports[i].name, buf, cap);
            break;
        }
    }
    if (r == OWLV_ERR_NO_DEVICE)
        snprintf(g_global_err, sizeof g_global_err,
                 "no port of kind %d among %d serial port(s)", kind, g_port_count);
    owlv__unlock(&g_table_lock);
    return r;
}

/* ── sessions ──────────────────────────────────────────────────────────── */

static int owlv__transport_open(const char* port, serial_pc** out)
{
    serial_pc* sp;
    if (!port || !port[0]) return OWLV_ERR_ARG;
    sp = serial_pc_open(port);
    if (!sp) return OWLV_ERR_OPEN;
    *out = sp;
    return OWLV_OK;
}

/* Claim a slot, run the OneWili handshake over an already-opened transport
 * and hand back the session handle. `sp` is the serial port the transport
 * belongs to, or NULL when it is not backed by one (the loopback used by the
 * tests); the session closes whatever it is given. On failure the slot is
 * released and `sp` is left for the caller to close. */
int owlv__open_with(const ow_transport* t, const char* label,
                    struct serial_pc* sp, int* session)
{
    int i, slot = -1;
    owlv_session* s;
    ow_status st;

    owlv__init();
    if (session) *session = 0;
    if (!t || !t->read || !t->write) return OWLV_ERR_ARG;

    owlv__lock(&g_table_lock);
    for (i = 0; i < OWLV_MAX_SESSIONS; ++i)
        if (g_sessions[i].id == 0) { slot = i; break; }
    if (slot < 0) {
        owlv__unlock(&g_table_lock);
        snprintf(g_global_err, sizeof g_global_err,
                 "owlv_open: all %d session slots are in use", OWLV_MAX_SESSIONS);
        return OWLV_ERR_LIMIT;
    }
    s = &g_sessions[slot];
    owlv__reset(s);
    s->sp = sp;
    snprintf(s->port, sizeof s->port, "%s", label ? label : "");

    st = ow_open(&s->dev, t);
    if (st != OW_OK) {
        owlv__reset(s);
        owlv__unlock(&g_table_lock);
        snprintf(g_global_err, sizeof g_global_err,
                 "owlv_open: ow_open failed on %s (status %d)",
                 label ? label : "?", (int)st);
        return (int)st;
    }
    s->id = g_next_id++;
    if (g_next_id <= 0) g_next_id = 1;
    if (session) *session = s->id;
    owlv__unlock(&g_table_lock);
    g_global_err[0] = 0;
    return OWLV_OK;
}

int owlv_open(const char* port, int* session)
{
    serial_pc* sp = NULL;
    ow_transport t;
    int r;

    owlv__init();
    if (session) *session = 0;
    if (!port || !port[0]) {
        snprintf(g_global_err, sizeof g_global_err, "owlv_open: empty port name");
        return OWLV_ERR_ARG;
    }

    r = owlv__transport_open(port, &sp);
    if (r != OWLV_OK) {
        snprintf(g_global_err, sizeof g_global_err,
                 "owlv_open: could not open %s", port);
        return r;
    }

    t = serial_pc_transport(sp);
    r = owlv__open_with(&t, port, sp, session);
    if (r != OWLV_OK) serial_pc_close(sp);
    return r;
}

int owlv_open_auto(int* session, char* port_used, int cap)
{
    char port[OWLV_PORTLEN];
    int r, i, found = -1, candidates = 0;

    owlv__init();
    if (session) *session = 0;

    owlv__lock(&g_table_lock);
    owlv__scan_ports();
    for (i = 0; i < g_port_count; ++i)
        if (g_ports[i].kind == OWLV_PORT_FW_MAIN) { found = i; break; }
    if (found < 0) {
        /* Platforms without USB identification: accept a single candidate. */
        for (i = 0; i < g_port_count; ++i)
            if (g_ports[i].kind == OWLV_PORT_UNKNOWN) { ++candidates; found = i; }
        if (candidates != 1) found = -1;
    }
    if (found >= 0)
        snprintf(port, sizeof port, "%s", g_ports[found].name);
    else
        snprintf(g_global_err, sizeof g_global_err,
                 "owlv_open_auto: no FreeWili main port among %d serial port(s)",
                 g_port_count);
    owlv__unlock(&g_table_lock);

    if (found < 0) return OWLV_ERR_NO_DEVICE;

    r = owlv_open(port, session);
    if (r == OWLV_OK) {
        int sr = owlv__str_out(port, port_used, cap);
        if (sr != OWLV_OK) return sr;
    }
    return r;
}

int owlv_close(int session)
{
    int i;
    owlv_session* s = NULL;

    owlv__init();
    owlv__lock(&g_table_lock);
    for (i = 0; i < OWLV_MAX_SESSIONS; ++i)
        if (g_sessions[i].id == session && session != 0) {
            s = &g_sessions[i];
            break;
        }
    if (!s) { owlv__unlock(&g_table_lock); return OWLV_OK; }

    /* Take the session lock so a call in flight on another thread finishes
     * before the transport goes away, then clear the id under the table lock
     * so no new call can acquire it. */
    owlv__lock(&s->lock);
    s->id = 0;
    if (s->bin_open) {
        ow_binary_close(&s->bdev);
        serial_pc_close(s->bin_sp);
        s->bin_open = 0;
        s->bin_sp = NULL;
    }
    ow_close(&s->dev);
    if (s->sp) serial_pc_close(s->sp);
    s->sp = NULL;
    free(s->dir);
    s->dir = NULL;
    s->dir_count = 0;
    owlv__reset(s);
    owlv__unlock(&s->lock);
    owlv__unlock(&g_table_lock);
    return OWLV_OK;
}

int owlv_close_all(void)
{
    int i, ids[OWLV_MAX_SESSIONS], n = 0;
    owlv__init();
    owlv__lock(&g_table_lock);
    for (i = 0; i < OWLV_MAX_SESSIONS; ++i)
        if (g_sessions[i].id) ids[n++] = g_sessions[i].id;
    owlv__unlock(&g_table_lock);
    for (i = 0; i < n; ++i) owlv_close(ids[i]);
    return OWLV_OK;
}

int owlv_session_valid(int session, int* valid)
{
    owlv_session* s = owlv__acquire(session);
    if (valid) *valid = s ? 1 : 0;
    if (s) owlv__unlock(&s->lock);
    return OWLV_OK;
}

int owlv_session_port(int session, char* buf, int cap)
{
    owlv_session* s = owlv__acquire(session);
    int r;
    if (!s) return OWLV_ERR_SESSION;
    r = owlv__str_out(s->port, buf, cap);
    owlv__unlock(&s->lock);
    return r;
}

/* ── raw command escape hatch ──────────────────────────────────────────── */

/* Deliberately simpler than the generated path: it does not reassemble a
 * response frame that the firmware split across physical lines, and text
 * events seen while waiting are discarded rather than queued. Use the
 * generated owlv_* commands for anything the generator already covers. */
static int owlv__raw(owlv_session* s, const char* cmd, char* resp, int cap)
{
    uint8_t out[OW_CMD_MAX + 2];
    char buf[OW_RESP_MAX];
    size_t clen, len = 0;

    clen = strlen(cmd);
    if (clen + 2 > sizeof out) return OWLV_ERR_ARG;
    out[0] = 0x02;                       /* reset to root + quiet, as ow__call */
    memcpy(out + 1, cmd, clen);
    out[1 + clen] = '\n';

    s->dev.line_len = 0;
    if (s->dev.t.write(s->dev.t.ctx, out, clen + 2) < 0) return OWLV_ERR_IO;

    for (;;) {
        size_t i;
        int n;
        for (i = 0; i < len; ++i) {
            if (buf[i] != '\n') continue;
            {
                size_t llen = i;
                char line[OW_RESP_MAX];
                if (llen && buf[llen - 1] == '\r') --llen;
                memcpy(line, buf, llen);
                line[llen] = 0;
                memmove(buf, buf + i + 1, len - i - 1);
                len -= i + 1;
                /* Skip spontaneous events "[*id ...]"; take the first real
                 * response frame "[<path> <ts> <seq> <body> <ok>]". */
                if (llen >= 5 && line[0] == '[' && line[1] != '*' &&
                    line[llen - 1] == ']') {
                    const char* body = NULL;
                    const char* end = line + llen - 1;
                    const char* q;
                    const char* last;
                    int spaces = 0;
                    for (q = line + 1; q < end; ++q)
                        if (*q == ' ' && ++spaces == 3) { body = q + 1; break; }
                    if (!body) return OWLV_ERR_PROTOCOL;
                    last = end;
                    while (last > body && last[-1] != ' ') --last;
                    if (last <= body) return OWLV_ERR_PROTOCOL;
                    {
                        size_t rlen = (size_t)((last - 1) - body);
                        char tmp[OW_RESP_MAX];
                        int ok = (last[0] == '1');
                        if (rlen >= sizeof tmp) return OWLV_ERR_BUFFER;
                        memcpy(tmp, body, rlen);
                        tmp[rlen] = 0;
                        {
                            int sr = owlv__str_out(tmp, resp, cap);
                            if (sr != OWLV_OK) return sr;
                        }
                        return ok ? OWLV_OK : OWLV_ERR_FAILED;
                    }
                }
                i = (size_t)-1;          /* rescan the shifted buffer */
            }
        }
        if (len + 1 >= sizeof buf) return OWLV_ERR_BUFFER;
        n = s->dev.t.read(s->dev.t.ctx, (uint8_t*)buf + len,
                          sizeof buf - len - 1, OW_DEFAULT_TIMEOUT_MS);
        if (n == 0) return OWLV_ERR_TIMEOUT;
        if (n < 0) return OWLV_ERR_IO;
        len += (size_t)n;
    }
}

int owlv_send_raw(int session, const char* command, char* response, int cap)
{
    owlv_session* s = owlv__acquire(session);
    int r;
    if (!s) return OWLV_ERR_SESSION;
    if (!command || !command[0]) { owlv__unlock(&s->lock); return OWLV_ERR_ARG; }
    r = owlv__raw(s, command, response, cap);
    if (r == OWLV_OK) s->err[0] = 0;
    else snprintf(s->err, sizeof s->err, "owlv_send_raw(\"%s\"): status %d",
                  command, r);
    owlv__unlock(&s->lock);
    return r;
}

/* ── text events ───────────────────────────────────────────────────────── */

/* ow_poll_text_line hands back everything after the event name, which for a
 * real event line is "<hexTimestampNs> <seq> <payload...> <ok>" -- the same
 * frame shape a command response uses. Split it so a LabVIEW diagram gets the
 * payload on its own, with the timestamp and sequence as separate outputs.
 * That matches what the Python package's event objects carry. A line that
 * does not fit the shape is passed through verbatim. */
static void owlv__split_event(const char* raw, char* payload, size_t cap,
                              unsigned long long* ts, long* seq)
{
    const char *p = raw, *tok1, *tok2, *body, *end;
    char* endp;
    unsigned long long v_ts;
    long v_seq;
    size_t n;

    if (ts)  *ts  = 0;
    if (seq) *seq = 0;

    while (*p == ' ') ++p;
    tok1 = p;
    while (*p && *p != ' ') ++p;
    if (!*p) goto verbatim;
    v_ts = strtoull(tok1, &endp, 16);
    if (endp != p) goto verbatim;

    while (*p == ' ') ++p;
    tok2 = p;
    while (*p && *p != ' ') ++p;
    if (tok2 == p) goto verbatim;
    v_seq = strtol(tok2, &endp, 10);
    if (endp != p) goto verbatim;

    while (*p == ' ') ++p;
    body = p;
    end = body + strlen(body);
    /* Drop the trailing " <ok>" token the firmware appends to events too. */
    while (end > body && end[-1] == ' ') --end;
    {
        const char* last = end;
        while (last > body && last[-1] != ' ') --last;
        /* Every event frame ends with the ok flag, so a body of exactly one
         * "0"/"1" token is an empty payload, not a payload of "1". */
        if ((end - last) == 1 && (*last == '0' || *last == '1')) {
            end = last;
            while (end > body && end[-1] == ' ') --end;
        }
    }
    if (ts)  *ts  = v_ts;
    if (seq) *seq = v_seq;
    n = (size_t)(end - body);
    if (!payload || cap == 0) return;
    if (n >= cap) n = cap - 1;
    memcpy(payload, body, n);
    payload[n] = 0;
    return;

verbatim:
    if (!payload || cap == 0) return;
    n = strlen(raw);
    if (n >= cap) n = cap - 1;
    memcpy(payload, raw, n);
    payload[n] = 0;
}

int owlv_poll_text_event(int session, int* got,
                         char* id, int id_cap, char* args, int args_cap,
                         unsigned int* time_stamp_ns_lo,
                         unsigned int* time_stamp_ns_hi,
                         int* sequence)
{
    owlv_session* s = owlv__acquire(session);
    ow_event ev;
    int n, r = OWLV_OK;

    if (!s) return OWLV_ERR_SESSION;
    if (got) *got = 0;

    memset(&ev, 0, sizeof ev);
    n = ow_poll_text_event(&s->dev, &ev);
    if (n < 0) {
        r = -n;
        owlv__set_err(s->err, "ow_poll_text_event", (ow_status)r);
    } else if (n > 0) {
        char payload[OW_RESP_MAX];
        unsigned long long ts = 0;
        long seq = 0;
        owlv__split_event(ev.u.text.args, payload, sizeof payload, &ts, &seq);
        owlv__split64(ts, time_stamp_ns_lo, time_stamp_ns_hi);
        if (sequence) *sequence = (int)seq;
        {
            int a = owlv__str_out(ev.u.text.id, id, id_cap);
            int b = owlv__str_out(payload, args, args_cap);
            r = (a != OWLV_OK) ? a : b;
        }
        if (got) *got = 1;
        s->err[0] = 0;
    } else {
        s->err[0] = 0;
    }
    owlv__unlock(&s->lock);
    return r;
}

int owlv_dropped_text_events(int session, int* dropped)
{
    owlv_session* s = owlv__acquire(session);
    if (!s) return OWLV_ERR_SESSION;
    if (dropped) *dropped = (int)s->dev.dropped_text_events;
    owlv__unlock(&s->lock);
    return OWLV_OK;
}

/* ── binary event port ─────────────────────────────────────────────────── */

int owlv_binary_open(int session, const char* port)
{
    owlv_session* s = owlv__acquire(session);
    serial_pc* sp = NULL;
    ow_transport t;
    ow_status st;
    int r;

    if (!s) return OWLV_ERR_SESSION;
    if (s->bin_open) { owlv__unlock(&s->lock); return OWLV_ERR_STATE; }

    r = owlv__transport_open(port, &sp);
    if (r != OWLV_OK) {
        snprintf(s->err, sizeof s->err,
                 "owlv_binary_open: could not open %s", port ? port : "(null)");
        owlv__unlock(&s->lock);
        return r;
    }
    t = serial_pc_transport(sp);
    st = ow_binary_open(&s->bdev, &t);
    if (st != OW_OK) {
        serial_pc_close(sp);
        owlv__release(s, st, "ow_binary_open");
        return (int)st;
    }
    s->bin_sp = sp;
    s->bin_open = 1;
    snprintf(s->bin_port, sizeof s->bin_port, "%s", port);
    owlv__release(s, OW_OK, "owlv_binary_open");
    return OWLV_OK;
}

int owlv_binary_open_auto(int session, char* port_used, int cap)
{
    char port[OWLV_PORTLEN];
    int r = owlv_find_port(OWLV_PORT_FW_BINARY, port, (int)sizeof port);
    if (r != OWLV_OK) return r;
    r = owlv_binary_open(session, port);
    if (r == OWLV_OK) r = owlv__str_out(port, port_used, cap);
    return r;
}

int owlv_binary_close(int session)
{
    owlv_session* s = owlv__acquire(session);
    if (!s) return OWLV_ERR_SESSION;
    if (s->bin_open) {
        ow_binary_close(&s->bdev);
        serial_pc_close(s->bin_sp);
        s->bin_sp = NULL;
        s->bin_open = 0;
        s->bin_port[0] = 0;
    }
    owlv__unlock(&s->lock);
    return OWLV_OK;
}

int owlv_binary_poll(int session, int* kind)
{
    owlv_session* s = owlv__acquire(session);
    int n, r = OWLV_OK;

    if (!s) return OWLV_ERR_SESSION;
    if (kind) *kind = OWLV_EV_NONE;
    if (!s->bin_open) { owlv__unlock(&s->lock); return OWLV_ERR_STATE; }

    n = ow_binary_poll(&s->bdev, &s->last_event);
    if (n < 0) {
        r = -n;
        s->last_kind = OWLV_EV_NONE;
        owlv__set_err(s->err, "ow_binary_poll", (ow_status)r);
    } else if (n > 0) {
        s->last_kind = (int)s->last_event.kind;
        if (kind) *kind = s->last_kind;
        s->err[0] = 0;
    } else {
        s->last_kind = OWLV_EV_NONE;
        s->err[0] = 0;
    }
    owlv__unlock(&s->lock);
    return r;
}

int owlv_binary_stats(int session, int* unknown_frames, int* size_mismatches)
{
    owlv_session* s = owlv__acquire(session);
    if (!s) return OWLV_ERR_SESSION;
    if (unknown_frames)  *unknown_frames  = (int)s->bdev.unknown_frames;
    if (size_mismatches) *size_mismatches = (int)s->bdev.size_mismatches;
    owlv__unlock(&s->lock);
    return OWLV_OK;
}

/* The C package keeps device timestamps as uint64_t. LabVIEW's Call Library
 * Function Node does carry a 64-bit integer type, but the Import Shared
 * Library Wizard's handling of `unsigned long long` varies by version, so the
 * ABI splits them into two U32 halves instead. */
static void owlv__split64(unsigned long long v, unsigned int* lo, unsigned int* hi)
{
    if (lo) *lo = (unsigned int)(v & 0xFFFFFFFFull);
    if (hi) *hi = (unsigned int)(v >> 32);
}

int owlv_last_gpio_report(int session,
                          unsigned int* time_stamp_ns_lo,
                          unsigned int* time_stamp_ns_hi,
                          unsigned int* gpio_bitfield,
                          int* error)
{
    owlv_session* s = owlv__acquire(session);
    ow_evt_gpio_report* e;
    if (!s) return OWLV_ERR_SESSION;
    if (s->last_kind != OWLV_EV_GPIO_REPORT) {
        owlv__unlock(&s->lock);
        return OWLV_ERR_STATE;
    }
    e = &s->last_event.u.gpio_report;
    owlv__split64((unsigned long long)e->time_stamp_ns,
                  time_stamp_ns_lo, time_stamp_ns_hi);
    if (gpio_bitfield) *gpio_bitfield = (unsigned int)e->gpio_bitfield;
    if (error) *error = e->error ? 1 : 0;
    owlv__unlock(&s->lock);
    return OWLV_OK;
}

int owlv_last_can_rx_report(int session,
                            unsigned int* time_stamp_ns_lo,
                            unsigned int* time_stamp_ns_hi,
                            unsigned int* gpio_bitfield,
                            unsigned int* canid,
                            unsigned int* filter_header_bits,
                            unsigned int* data_words, int data_words_cap,
                            int* error)
{
    owlv_session* s = owlv__acquire(session);
    ow_evt_can_rx_report* e;
    int i, n;
    if (!s) return OWLV_ERR_SESSION;
    if (s->last_kind != OWLV_EV_CAN_RX_REPORT) {
        owlv__unlock(&s->lock);
        return OWLV_ERR_STATE;
    }
    e = &s->last_event.u.can_rx_report;
    owlv__split64((unsigned long long)e->time_stamp_ns,
                  time_stamp_ns_lo, time_stamp_ns_hi);
    if (gpio_bitfield)      *gpio_bitfield      = (unsigned int)e->gpio_bitfield;
    if (canid)              *canid              = (unsigned int)e->r0_canid;
    if (filter_header_bits) *filter_header_bits = (unsigned int)e->r1_filter_header_bits;
    if (error)              *error              = e->error ? 1 : 0;
    n = (int)(sizeof e->data_words / sizeof e->data_words[0]);
    if (data_words && data_words_cap > 0)
        for (i = 0; i < n && i < data_words_cap; ++i)
            data_words[i] = (unsigned int)e->data_words[i];
    owlv__unlock(&s->lock);
    return (data_words && data_words_cap > 0 && data_words_cap < n)
           ? OWLV_ERR_BUFFER : OWLV_OK;
}

int owlv_last_logic_analyzer_report(int session,
                                    unsigned int* trigger_time_stamp_ns_lo,
                                    unsigned int* trigger_time_stamp_ns_hi,
                                    unsigned int* sample_rate_ns,
                                    int* gpio_start_pin,
                                    int* bits_per_sample,
                                    int* trigger_type,
                                    unsigned int* trigger_location,
                                    unsigned int* buffer_head,
                                    int* analog_channel_mask,
                                    int* analog_resolution,
                                    int* analog_channel_count,
                                    unsigned int* analog_sample_rate_ns,
                                    unsigned int* analog_sample_count,
                                    unsigned int* analog_buffer_head,
                                    unsigned int* analog_trigger_location,
                                    int* error)
{
    owlv_session* s = owlv__acquire(session);
    ow_evt_logic_analyzer_report* e;
    if (!s) return OWLV_ERR_SESSION;
    if (s->last_kind != OWLV_EV_LOGIC_ANALYZER_REPORT) {
        owlv__unlock(&s->lock);
        return OWLV_ERR_STATE;
    }
    e = &s->last_event.u.logic_analyzer_report;
    owlv__split64((unsigned long long)e->trigger_time_stamp_ns,
                  trigger_time_stamp_ns_lo, trigger_time_stamp_ns_hi);
    if (sample_rate_ns)          *sample_rate_ns          = (unsigned int)e->sample_rate_ns;
    if (gpio_start_pin)          *gpio_start_pin          = (int)e->gpio_start_pin;
    if (bits_per_sample)         *bits_per_sample         = (int)e->bits_per_sample;
    if (trigger_type)            *trigger_type            = (int)e->trigger_type;
    if (trigger_location)        *trigger_location        = (unsigned int)e->trigger_location;
    if (buffer_head)             *buffer_head             = (unsigned int)e->buffer_head;
    if (analog_channel_mask)     *analog_channel_mask     = (int)e->analog_channel_mask;
    if (analog_resolution)       *analog_resolution       = (int)e->analog_resolution;
    if (analog_channel_count)    *analog_channel_count    = (int)e->analog_channel_count;
    if (analog_sample_rate_ns)   *analog_sample_rate_ns   = (unsigned int)e->analog_sample_rate_ns;
    if (analog_sample_count)     *analog_sample_count     = (unsigned int)e->analog_sample_count;
    if (analog_buffer_head)      *analog_buffer_head      = (unsigned int)e->analog_buffer_head;
    if (analog_trigger_location) *analog_trigger_location = (unsigned int)e->analog_trigger_location;
    if (error)                   *error                   = e->error ? 1 : 0;
    owlv__unlock(&s->lock);
    return OWLV_OK;
}

int owlv_last_text_event(int session, char* id, int id_cap,
                         char* args, int args_cap,
                         unsigned int* time_stamp_ns_lo,
                         unsigned int* time_stamp_ns_hi,
                         int* sequence)
{
    owlv_session* s = owlv__acquire(session);
    char payload[OW_RESP_MAX];
    unsigned long long ts = 0;
    long seq = 0;
    int a, b;
    if (!s) return OWLV_ERR_SESSION;
    if (s->last_kind != OWLV_EV_TEXT) {
        owlv__unlock(&s->lock);
        return OWLV_ERR_STATE;
    }
    owlv__split_event(s->last_event.u.text.args, payload, sizeof payload,
                      &ts, &seq);
    owlv__split64(ts, time_stamp_ns_lo, time_stamp_ns_hi);
    if (sequence) *sequence = (int)seq;
    a = owlv__str_out(s->last_event.u.text.id, id, id_cap);
    b = owlv__str_out(payload, args, args_cap);
    owlv__unlock(&s->lock);
    return (a != OWLV_OK) ? a : b;
}

/* ── file transfer ─────────────────────────────────────────────────────── */

static void owlv__progress(void* ctx, size_t done, size_t total)
{
    owlv_session* s = (owlv_session*)ctx;
    if (!s) return;
    s->prog_done  = (unsigned int)done;
    s->prog_total = (unsigned int)total;
}

int owlv_file_put(int session, const char* host_path, const char* device_path)
{
    owlv_session* s = owlv__acquire(session);
    ow_status r;
    if (!s) return OWLV_ERR_SESSION;
    if (!host_path || !device_path) { owlv__unlock(&s->lock); return OWLV_ERR_ARG; }
    s->prog_done = s->prog_total = 0;
    r = ow_file_put(&s->dev, host_path, device_path, owlv__progress, s);
    owlv__release(s, r, "ow_file_put");
    return (int)r;
}

int owlv_file_get(int session, const char* device_path, const char* host_path)
{
    owlv_session* s = owlv__acquire(session);
    ow_status r;
    if (!s) return OWLV_ERR_SESSION;
    if (!host_path || !device_path) { owlv__unlock(&s->lock); return OWLV_ERR_ARG; }
    s->prog_done = s->prog_total = 0;
    r = ow_file_get(&s->dev, device_path, host_path, owlv__progress, s);
    owlv__release(s, r, "ow_file_get");
    return (int)r;
}

int owlv_file_put_mem(int session, const char* device_path,
                      const unsigned char* data, int len)
{
    owlv_session* s = owlv__acquire(session);
    ow_status r;
    if (!s) return OWLV_ERR_SESSION;
    if (!device_path || len < 0) { owlv__unlock(&s->lock); return OWLV_ERR_ARG; }
    s->prog_done = s->prog_total = 0;
    r = ow_file_put_mem(&s->dev, device_path, (const uint8_t*)data,
                        (size_t)len, owlv__progress, s);
    owlv__release(s, r, "ow_file_put_mem");
    return (int)r;
}

int owlv_file_get_mem(int session, const char* device_path,
                      unsigned char* buf, int cap, int* len)
{
    owlv_session* s = owlv__acquire(session);
    ow_status r;
    size_t n = 0;
    if (!s) return OWLV_ERR_SESSION;
    if (len) *len = 0;
    if (!device_path || cap < 0) { owlv__unlock(&s->lock); return OWLV_ERR_ARG; }
    s->prog_done = s->prog_total = 0;
    r = ow_file_get_mem(&s->dev, device_path, (uint8_t*)buf, (size_t)cap, &n,
                        owlv__progress, s);
    if (len) *len = (int)n;
    owlv__release(s, r, "ow_file_get_mem");
    return (int)r;
}

int owlv_file_progress(int session, unsigned int* done, unsigned int* total)
{
    int i;
    owlv_session* s = NULL;
    owlv__init();
    /* Deliberately does NOT take the session lock: a transfer holds it for the
     * whole transfer, and the point of this call is to be readable meanwhile. */
    owlv__lock(&g_table_lock);
    for (i = 0; i < OWLV_MAX_SESSIONS; ++i)
        if (g_sessions[i].id == session && session != 0) { s = &g_sessions[i]; break; }
    if (s) {
        if (done)  *done  = s->prog_done;
        if (total) *total = s->prog_total;
    }
    owlv__unlock(&g_table_lock);
    return s ? OWLV_OK : OWLV_ERR_SESSION;
}

int owlv_file_list(int session, const char* dir_path, int* count)
{
    owlv_session* s = owlv__acquire(session);
    ow_status r;
    size_t n = 0;

    if (!s) return OWLV_ERR_SESSION;
    if (count) *count = 0;
    if (!s->dir) {
        s->dir = (ow_dir_entry*)calloc(OWLV_DIR_MAX, sizeof(ow_dir_entry));
        if (!s->dir) { owlv__unlock(&s->lock); return OWLV_ERR_BUFFER; }
    }
    s->dir_count = 0;
    r = ow_file_list(&s->dev, dir_path ? dir_path : "", s->dir,
                     (size_t)OWLV_DIR_MAX, &n);
    /* OW_ERR_BUFFER still leaves *n valid entries behind; report them. */
    s->dir_count = (int)n;
    if (count) *count = (int)n;
    owlv__release(s, r, "ow_file_list");
    return (int)r;
}

int owlv_file_entry(int session, int index, char* name, int name_cap,
                    int* is_dir, unsigned int* size)
{
    owlv_session* s = owlv__acquire(session);
    ow_dir_entry* e;
    int r;
    if (!s) return OWLV_ERR_SESSION;
    if (!s->dir || index < 0 || index >= s->dir_count) {
        owlv__unlock(&s->lock);
        return OWLV_ERR_ARG;
    }
    e = &s->dir[index];
    r = owlv__str_out(e->name, name, name_cap);
    if (is_dir) *is_dir = e->is_dir ? 1 : 0;
    if (size)   *size   = (unsigned int)e->size;
    owlv__unlock(&s->lock);
    return r;
}

int owlv_sd_host_select(int session, int to_pc)
{
    owlv_session* s = owlv__acquire(session);
    ow_status r;
    if (!s) return OWLV_ERR_SESSION;
    r = ow_sd_host_select(&s->dev, to_pc ? true : false);
    owlv__release(s, r, "ow_sd_host_select");
    return (int)r;
}
