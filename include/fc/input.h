#pragma once
#include <stdbool.h>

#define FC_KEY_UP        1001
#define FC_KEY_DOWN      1002
#define FC_KEY_LEFT      1003
#define FC_KEY_RIGHT     1004
#define FC_KEY_ESC       27
#define FC_KEY_ENTER     13
#define FC_KEY_BACKSPACE 127
#define FC_KEY_TAB       9
#define FC_KEY_F1        1101
#define FC_KEY_F2        1102
#define FC_KEY_F3        1103
#define FC_KEY_F4        1104
#define FC_KEY_F5        1105
#define FC_KEY_DELETE    1201
#define FC_KEY_HOME      1202
#define FC_KEY_END       1203
#define FC_KEY_PGUP      1204
#define FC_KEY_PGDN      1205

typedef enum {
    FC_EV_NONE = 0,
    FC_EV_KEY,
    FC_EV_MOUSE_PRESS,
    FC_EV_MOUSE_RELEASE,
} fc_ev_type_t;

typedef struct {
    fc_ev_type_t type;
    int          x, y;
    int          btn;
    int          key;
    char         ch;    // printable char wenn key < 256
} fc_event_t;

void fc_mouse_enable(bool on);
bool fc_input_read(fc_event_t *ev, int timeout_ms);
