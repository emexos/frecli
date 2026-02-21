#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include "fc/input.h"
#include "fc/term.h"

void fc_mouse_enable(bool on) {
    fc_term_writes(on ? "\033[?1000h\033[?1006h" : "\033[?1000l\033[?1006l");
    fc_term_flush();
}

static bool wait_stdin(int ms) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    struct timeval tv;
    tv.tv_sec  = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, ms < 0 ? NULL : &tv) > 0;
}
static int read_all(char *buf, int max)
{
    int n = (int)read(STDIN_FILENO, buf, max - 1);
    if (n <= 0) return 0;
    buf[n] = 0;


    if (buf[0] == 0x1B && n == 1) {
        if (wait_stdin(20)) {
            int more = (int)read(STDIN_FILENO, buf + 1, max - 2);
            if (more > 0) n += more;
            buf[n] = 0;
        }
    }
    return n;
}

bool fc_input_read(fc_event_t *ev, int timeout_ms)
{
    memset(ev, 0, sizeof(*ev));
    if (!wait_stdin(timeout_ms)) return false;

    char buf[64];
    int n = read_all(buf, sizeof(buf));
    if (n <= 0) return false;

    // mouse
    if (n > 3 && buf[0] == 0x1B && buf[1] == '[' && buf[2] == '<') {
        int btn = 0, x = 0, y = 0;
        char *p = buf + 3;
        while (*p && *p != ';') btn = btn * 10 + (*p++ - '0');
        if (*p == ';') p++;
        while (*p && *p != ';') x = x * 10 + (*p++ - '0');
        if (*p == ';') p++;
        while (*p && *p != 'M' && *p != 'm') y = y * 10 + (*p++ - '0');
        ev->type = (*p == 'M') ? FC_EV_MOUSE_PRESS : FC_EV_MOUSE_RELEASE;
        ev->x = x - 1;
        ev->y = y - 1;
        ev->btn  = (btn & 3) + 1;
        return true;
    }

    // escape sequences
    if (buf[0] == 0x1B && n >= 3 && buf[1] == '[') {
        ev->type = FC_EV_KEY;
        // arrows
        if (n == 3) {
            switch (buf[2]) {
                case 'A': ev->key = FC_KEY_UP;    return true;
                case 'B': ev->key = FC_KEY_DOWN;  return true;
                case 'C': ev->key = FC_KEY_RIGHT; return true;
                case 'D': ev->key = FC_KEY_LEFT;  return true;
                case 'H': ev->key = FC_KEY_HOME;  return true;
                case 'F': ev->key = FC_KEY_END;   return true;
            }
        }
        // ESC [ n ~
        if (buf[n-1] == '~') {
            int code = 0;
            for (int i = 2; i < n-1; i++) code = code * 10 + (buf[i] - '0');
            switch (code) {
                case 1:  ev->key = FC_KEY_HOME;   return true;
                case 3:  ev->key = FC_KEY_DELETE; return true;
                case 4:  ev->key = FC_KEY_END;    return true;
                case 5:  ev->key = FC_KEY_PGUP;   return true;
                case 6:  ev->key = FC_KEY_PGDN;   return true;
                case 11: ev->key = FC_KEY_F1;     return true;
                case 12: ev->key = FC_KEY_F2;     return true;
                case 13: ev->key = FC_KEY_F3;     return true;
                case 14: ev->key = FC_KEY_F4;     return true;
                case 15: ev->key = FC_KEY_F5;     return true;
            }
        }
        // ESC O X
        if (buf[1] == 'O') {
            switch (buf[2]) {
                case 'P': ev->key = FC_KEY_F1; return true;
                case 'Q': ev->key = FC_KEY_F2; return true;
                case 'R': ev->key = FC_KEY_F3; return true;
                case 'S': ev->key = FC_KEY_F4; return true;
            }
        }
        return false;
    }
    if (buf[0] == 0x1B && n == 1) {
        ev->type = FC_EV_KEY;
        ev->key  = FC_KEY_ESC;
        return true;
    }


    ev->type = FC_EV_KEY;
    ev->key  = (unsigned char)buf[0];
    ev->ch   = buf[0];
    return true;
}
