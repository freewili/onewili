/* Exercise the LabVIEW ABI from plain C.
 *
 * Run this before you open LabVIEW. It uses the DLL exactly the way a Call
 * Library Function Node does -- integer session handles, caller-sized string
 * buffers, status codes -- so if something fails here the problem is in the
 * wrapper or the wiring, not in LabVIEW.
 *
 *   owlv_selftest                 list ports and check the ABI, no device
 *   owlv_selftest auto            find a FreeWili, open it, run device checks
 *   owlv_selftest COM7            open that port and run device checks
 */

#include "onewili_lv.h"

#include <stdio.h>
#include <string.h>

static int failures;

static void check(int cond, const char* what)
{
    printf("  [%s] %s\n", cond ? "ok" : "FAIL", what);
    if (!cond) ++failures;
}

static void show_status(const char* what, int r)
{
    char msg[128] = {0};
    owlv_status_message(r, msg, (int)sizeof msg);
    printf("  %-34s -> %d (%s)\n", what, r, msg);
}

int main(int argc, char** argv)
{
    int major = 0, minor = 0, patch = 0;
    int count = 0, i, r, session = 0;
    char buf[256];

    printf("OneWili LabVIEW wrapper self-test\n");

    owlv_version(&major, &minor, &patch);
    printf("  wrapper version %d.%d.%d, %d-bit build\n",
           major, minor, patch, owlv_pointer_size() * 8);
    check(owlv_pointer_size() == 4 || owlv_pointer_size() == 8,
          "pointer size is 4 or 8");

    /* String-out contract: too small must truncate, NUL-terminate and report
     * OWLV_ERR_BUFFER; a zero cap must be accepted and ignored. */
    memset(buf, 'x', sizeof buf);
    r = owlv_status_message(OWLV_ERR_TIMEOUT, buf, 4);
    check(r == OWLV_ERR_BUFFER && strlen(buf) == 3,
          "short string buffer truncates and reports OWLV_ERR_BUFFER");
    check(owlv_status_message(OWLV_OK, NULL, 0) == OWLV_OK,
          "NULL string output is accepted");

    /* Session contract: 0 and unknown handles are rejected, never crash. */
    check(owlv_io_gpio_set_io_toggle(0, 25) == OWLV_ERR_SESSION,
          "session 0 is rejected");
    check(owlv_io_gpio_set_io_toggle(99999, 25) == OWLV_ERR_SESSION,
          "unknown session is rejected");
    check(owlv_close(12345) == OWLV_OK,
          "closing an unknown session is a no-op");

    printf("\nserial ports:\n");
    r = owlv_refresh_ports(&count);
    show_status("owlv_refresh_ports", r);
    for (i = 0; i < count; ++i) {
        char name[64] = {0}, desc[192] = {0};
        int kind = 0, vid = 0, pid = 0;
        static const char* kinds[] = {
            "unknown", "FreeWili main", "FreeWili display",
            "FreeWili binary (FTDI)", "FreeWili debug probe", "FreeWili ESP32"
        };
        owlv_port_name(i, name, (int)sizeof name);
        owlv_port_description(i, desc, (int)sizeof desc);
        owlv_port_kind(i, &kind);
        owlv_port_usb_ids(i, &vid, &pid);
        printf("  %-12s %04X:%04X  %-24s %s\n", name, vid, pid,
               (kind >= 0 && kind <= 5) ? kinds[kind] : "?", desc);
    }
    if (count == 0) printf("  (none)\n");

    if (argc < 2) {
        printf("\n%d failure(s). Pass a port name or 'auto' to test a device.\n",
               failures);
        return failures ? 1 : 0;
    }

    printf("\nopening device:\n");
    if (strcmp(argv[1], "auto") == 0) {
        r = owlv_open_auto(&session, buf, (int)sizeof buf);
        show_status("owlv_open_auto", r);
        if (r == OWLV_OK) printf("  found %s\n", buf);
    } else {
        r = owlv_open(argv[1], &session);
        show_status("owlv_open", r);
    }
    if (r != OWLV_OK) {
        owlv_last_error(0, buf, (int)sizeof buf);
        printf("  last error: %s\n", buf);
        return 1;
    }
    printf("  session %d\n", session);

    {
        int valid = 0;
        owlv_session_valid(session, &valid);
        check(valid == 1, "session reports valid");
        owlv_session_port(session, buf, (int)sizeof buf);
        printf("  port: %s\n", buf);
    }

    /* A no-argument read: proves the request/response round trip works. */
    {
        unsigned int gpio = 0;
        r = owlv_io_gpio_read_all(session, &gpio);
        show_status("owlv_io_gpio_read_all", r);
        if (r == OWLV_OK) printf("  GPIO bitfield: 0x%08X\n", gpio);
        else { owlv_last_error(session, buf, (int)sizeof buf);
               printf("  last error: %s\n", buf); }
    }

    /* A scalar-in command. */
    r = owlv_io_gpio_set_io_toggle(session, 25);
    show_status("owlv_io_gpio_set_io_toggle(25)", r);

    /* Mixed string-out / bool-out, to check the output marshalling. */
    {
        char sd[128] = {0}, mask[128] = {0};
        int hoststream = -1;
        r = owlv_hardware_system_device_state(session, sd, (int)sizeof sd,
                                              &hoststream, mask,
                                              (int)sizeof mask);
        show_status("owlv_hardware_system_device_state", r);
        if (r == OWLV_OK) {
            printf("  sd=%s hoststream=%d activemask=%s\n", sd, hoststream, mask);
            check(hoststream == 0 || hoststream == 1,
                  "bool output is normalised to 0/1");
        }
    }

    /* The raw escape hatch, against the same command as above. */
    r = owlv_send_raw(session, "i\\g\\u", buf, (int)sizeof buf);
    show_status("owlv_send_raw(i\\g\\u)", r);
    if (r == OWLV_OK) printf("  raw response: %s\n", buf);

    /* Text events: never blocks, and reports "nothing pending" as got == 0. */
    {
        int got = -1, seq = 0;
        unsigned int ts_lo = 0, ts_hi = 0;
        char id[64] = {0}, args[256] = {0};
        r = owlv_poll_text_event(session, &got, id, (int)sizeof id,
                                 args, (int)sizeof args,
                                 &ts_lo, &ts_hi, &seq);
        show_status("owlv_poll_text_event", r);
        check(got == 0 || got == 1, "poll reports 0 or 1");
        if (got)
            printf("  event %s seq %d at %llu ns: %s\n", id, seq,
                   ((unsigned long long)ts_hi << 32) | ts_lo, args);
    }

    r = owlv_close(session);
    show_status("owlv_close", r);
    {
        int valid = 1;
        owlv_session_valid(session, &valid);
        check(valid == 0, "session is invalid after close");
    }
    check(owlv_io_gpio_set_io_toggle(session, 25) == OWLV_ERR_SESSION,
          "closed session is rejected");

    owlv_close_all();
    printf("\n%d failure(s).\n", failures);
    return failures ? 1 : 0;
}
