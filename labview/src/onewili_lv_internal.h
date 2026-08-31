/* Shared between the hand-written core (onewili_lv.c) and the generated
 * command forwarders (onewili_lv_api.c). Not part of the LabVIEW ABI --
 * never point the Import Shared Library Wizard at this file. */
#ifndef ONEWILI_LV_INTERNAL_H
#define ONEWILI_LV_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "onewili.h"
#include "onewili_binary.h"
#include "onewili_events.h"
#include "onewili_files.h"
#include "serial_pc.h"

#include "onewili_lv.h"

#ifdef _WIN32
#include <windows.h>
typedef CRITICAL_SECTION owlv_mutex;
#else
#include <pthread.h>
typedef pthread_mutex_t owlv_mutex;
#endif

#define OWLV_MAX_SESSIONS 32
#define OWLV_MAX_PORTS 64
#define OWLV_DIR_MAX 512
#define OWLV_ERRLEN 256
#define OWLV_PORTLEN 128

typedef struct owlv_session {
    int              id;              /* 0 = free slot */
    char             port[OWLV_PORTLEN];
    serial_pc*       sp;
    ow_device        dev;

    int              bin_open;
    char             bin_port[OWLV_PORTLEN];
    serial_pc*       bin_sp;
    ow_binary_device bdev;

    ow_event         last_event;      /* latched by owlv_binary_poll */
    int              last_kind;

    ow_dir_entry*    dir;
    int              dir_count;

    /* Written without the session lock so a parallel LabVIEW loop can read
     * them while a transfer is running. */
    volatile unsigned int prog_done, prog_total;

    char             err[OWLV_ERRLEN];
    owlv_mutex       lock;
} owlv_session;

/* Validate the handle and take the session lock. NULL when the handle is
 * not an open session -- callers return OWLV_ERR_SESSION. */
owlv_session* owlv__acquire(int session);

/* Record the outcome (for owlv_last_error) and release the session lock. */
void owlv__release(owlv_session* s, ow_status r, const char* what);

/* Start a session on an already-opened transport. `sp` is the serial port
 * backing it, or NULL when there is none (the loopback used by the tests);
 * the session closes whatever it is handed. Not part of the LabVIEW ABI. */
int owlv__open_with(const ow_transport* t, const char* label,
                    struct serial_pc* sp, int* session);

#endif /* ONEWILI_LV_INTERNAL_H */
