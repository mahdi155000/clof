#ifndef PLUGIN_H
#define PLUGIN_H

#include <ncursesw/ncurses.h>  // for WINDOW

typedef void (*plugin_func_t)(WINDOW *win);

typedef struct {
    const char *name;
    plugin_func_t func;
} PluginEntry;

void register_plugin(const char *name, plugin_func_t func);

extern PluginEntry plugins[];
extern int plugin_count;

#endif