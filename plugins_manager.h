#ifndef PLUGINS_MANAGER_H
#define PLUGINS_MANAGER_H

#include "storage.h"  // ✅ this makes Item and MAX_ITEMS known

//typedef void (*plugin_func_t)(Item *M_L, int *item_count);
typedef void (*plugin_func_t)(int argc, char **argv, Item *M_L, int *item_count);

typedef struct {
    const char *name;
    plugin_func_t func;
} Plugin;

void register_plugin(const char *name, plugin_func_t func);
void run_plugin(const char *name, Item *M_L, int *item_count);

#endif

