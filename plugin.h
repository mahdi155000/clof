#ifndef PLUGIN_H
#define PLUGIN_H

#include <ncursesw/ncurses.h>  // for WINDOW

typedef void (*plugin_func_t)(WINDOW *win);

void register_plugin(const char *name, plugin_func_t func);

extern struct plugin {
    const char *name;
    plugin_func_t func;
} plugins[];
extern int plugin_count;

#endif