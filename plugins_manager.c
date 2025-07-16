#include <stdio.h>
#include <string.h>
#include "plugins_manager.h"

#define MAX_PLUGINS 100

typedef struct {
    const char *name;
    plugin_func_t func;
} Plugin;

static Plugin plugins[MAX_PLUGINS];
static int plugin_count = 0;

void register_plugin(const char *name, plugin_func_t func) {
    if (plugin_count < MAX_PLUGINS) {
        plugins[plugin_count].name = name;
        plugins[plugin_count].func = func;
        plugin_count++;
        printf("Registered plugin: %s\n", name); // debug print
    }
}

void run_plugin(const char *name) {
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(plugins[i].name, name) == 0) {
            plugins[i].func();
            return;
        }
    }
    printf("Plugin '%s' not found.\n", name);
}

void list_plugins(void) {
    printf("Available plugins:\n");
    for (int i = 0; i < plugin_count; i++) {
        printf(" - %s\n", plugins[i].name);
    }
}
