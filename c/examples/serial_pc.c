#include "serial_pc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
/* ── Windows (Win32 COM) ─────────────────────────────────────────────── */
#include <windows.h>

struct serial_pc { HANDLE h; };

serial_pc* serial_pc_open(const char* port_name) {
    char path[64];
    snprintf(path, sizeof path, "\\\\.\\%s", port_name);
    HANDLE h = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                           OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return NULL;
    DCB dcb;
    memset(&dcb, 0, sizeof dcb);
    dcb.DCBlength = sizeof dcb;
    if (!GetCommState(h, &dcb)) { CloseHandle(h); return NULL; }
    dcb.BaudRate = 1000000;
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fBinary  = TRUE;
    dcb.fOutxCtsFlow = dcb.fOutxDsrFlow = dcb.fOutX = dcb.fInX = FALSE;
    dcb.fDsrSensitivity = dcb.fErrorChar = dcb.fNull = dcb.fAbortOnError = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    if (!SetCommState(h, &dcb)) { CloseHandle(h); return NULL; }
    /* FTDI delivers bursts faster than a polling caller may be scheduled.
     * Reserve room for a complete maximum capture, including its headers. */
    if (!SetupComm(h, 2 * 1024 * 1024, 4096)) { CloseHandle(h); return NULL; }
    if (!PurgeComm(h, PURGE_RXCLEAR | PURGE_TXCLEAR)) { CloseHandle(h); return NULL; }
    serial_pc* sp = (serial_pc*)malloc(sizeof *sp);
    if (!sp) { CloseHandle(h); return NULL; }
    sp->h = h;
    return sp;
}

void serial_pc_close(serial_pc* sp) {
    if (!sp) return;
    CloseHandle(sp->h);
    free(sp);
}

static int serial_pc_write(void* ctx, const uint8_t* data, size_t len) {
    serial_pc* sp = (serial_pc*)ctx;
    DWORD written = 0;
    if (!WriteFile(sp->h, data, (DWORD)len, &written, NULL)) return -1;
    return (int)written;
}

static int serial_pc_read(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms) {
    serial_pc* sp = (serial_pc*)ctx;
    DWORD errors = 0;
    COMSTAT status;
    if (!ClearCommError(sp->h, &errors, &status) || errors) return -1;
    COMMTIMEOUTS to;
    memset(&to, 0, sizeof to);
    to.ReadIntervalTimeout        = MAXDWORD;
    to.ReadTotalTimeoutMultiplier = timeout_ms ? MAXDWORD : 0;
    to.ReadTotalTimeoutConstant   = timeout_ms;
    if (!SetCommTimeouts(sp->h, &to)) return -1;
    DWORD got = 0;
    if (!ReadFile(sp->h, buf, (DWORD)cap, &got, NULL)) return -1;
    return (int)got;    /* 0 == timeout */
}

#else
/* ── POSIX (termios) ─────────────────────────────────────────────────── */
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#ifdef __APPLE__
#include <sys/ioctl.h>
#include <IOKit/serial/ioss.h>
#endif

struct serial_pc { int fd; };

serial_pc* serial_pc_open(const char* port_name) {
    int fd = open(port_name, O_RDWR | O_NOCTTY);
    if (fd < 0) return NULL;
    struct termios tio;
    if (tcgetattr(fd, &tio) != 0) { close(fd); return NULL; }
    cfmakeraw(&tio);
    tio.c_cflag |= CLOCAL | CREAD;
#ifdef CRTSCTS
    tio.c_cflag &= ~CRTSCTS;
#endif
#ifdef __APPLE__
    if (cfsetispeed(&tio, B9600) || cfsetospeed(&tio, B9600)) { close(fd); return NULL; }
#else
#ifdef B1000000
    if (cfsetispeed(&tio, B1000000) || cfsetospeed(&tio, B1000000)) { close(fd); return NULL; }
#else
    close(fd); errno = EINVAL; return NULL;
#endif
#endif
    tio.c_cc[VMIN]  = 0;
    tio.c_cc[VTIME] = 0;
    if (tcsetattr(fd, TCSANOW, &tio) != 0) { close(fd); return NULL; }
#ifdef __APPLE__
    { speed_t speed = 1000000;
      if (ioctl(fd, IOSSIOSPEED, &speed) < 0) { close(fd); return NULL; } }
#endif
    serial_pc* sp = (serial_pc*)malloc(sizeof *sp);
    if (!sp) { close(fd); return NULL; }
    sp->fd = fd;
    return sp;
}

void serial_pc_close(serial_pc* sp) {
    if (!sp) return;
    close(sp->fd);
    free(sp);
}

static int serial_pc_write(void* ctx, const uint8_t* data, size_t len) {
    serial_pc* sp = (serial_pc*)ctx;
    ssize_t n = write(sp->fd, data, len);
    return n < 0 ? -1 : (int)n;
}

static int serial_pc_read(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms) {
    serial_pc* sp = (serial_pc*)ctx;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sp->fd, &rfds);
    struct timeval tv;
    tv.tv_sec  = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    int rv = select(sp->fd + 1, &rfds, NULL, NULL, &tv);
    if (rv == 0) return 0;      /* timeout */
    if (rv < 0) return -1;
    ssize_t n = read(sp->fd, buf, cap);
    return n < 0 ? -1 : (int)n;
}
#endif

ow_transport serial_pc_transport(serial_pc* sp) {
    ow_transport t;
    t.ctx   = sp;
    t.write = serial_pc_write;
    t.read  = serial_pc_read;
    return t;
}
