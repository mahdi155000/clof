#include <stdio.h>
#include <string.h>
#include "plugins_manager.h"

#define MAX_PLUGINS 100


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
/*
void run_plugin(const char *name, Item *M_L, int *item_count) {
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(plugins[i].name, name) == 0) {
            plugins[i].func(M_L, item_count);  // ✅ fixed call
            return;
        }
    }
    printf("Plugin '%s' not found.\n", name);
}*/
void run_plugin(const char *input, Item *M_L, int *item_count) {
    // Make a mutable copy of input
    char buf[256];
    strncpy(buf, input, sizeof(buf));
    buf[sizeof(buf) - 1] = '\0';

    // Split input into arguments
    char *argv[10];
    int argc = 0;
    char *token = strtok(buf, " ");
    while (token && argc < 10) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    if (argc == 0) return;

    // Match by first word
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(plugins[i].name, argv[0]) == 0) {
            plugins[i].func(argc, argv, M_L, item_count);
            return;
        }
    }

    printf("Plugin '%s' not found.\n", argv[0]);
}



void list_plugins(void) {
    printf("Available plugins:\n");
    for (int i = 0; i < plugin_count; i++) {
        printf(" - %s\n", plugins[i].name);
    }
}
