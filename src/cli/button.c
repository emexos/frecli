#include <string.h>
#include "fc/widgets.h"
#include "fc/colors.h"

fc_node_t *fc_button(fc_dom_t *dom, int x, int y, int w, const char *label) {
    fc_node_t *n = fc_node_new(dom, FC_NODE_BUTTON, x, y, w, 1);
    strncpy(n->d.button.label, label ? label : "", FC_STR - 1);
    n->d.button.fg = FC_BLACK;
    n->d.button.bg = FC_WHITE;
    n->d.button.hover_fg = FC_BLACK;
    n->d.button.hover_bg = FC_CYAN;
    return n;
}

void fc_button_colors(fc_node_t *n, fc_color_t fg, fc_color_t bg) {
    n->d.button.fg = fg;
    n->d.button.bg = bg;
    fc_node_dirty(n);
}

void fc_button_hover_colors(fc_node_t *n, fc_color_t fg, fc_color_t bg) {
    n->d.button.hover_fg = fg;
    n->d.button.hover_bg = bg;
    fc_node_dirty(n);
}

void fc_button_on_click(fc_node_t *n, void (*cb)(fc_node_t *n)) {
    n->d.button.on_click = cb;
}

void fc_button_set_userdata(fc_node_t *n, void *data) {
    n->d.button.userdata = data;
}

void *fc_button_get_userdata(fc_node_t *n) {
    return n->d.button.userdata;
}

void fc_button_set_label(fc_node_t *n, const char *label) {
    strncpy(n->d.button.label, label ? label : "", FC_STR - 1);
    fc_node_dirty(n);
}
