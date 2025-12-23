#include <stdio.h>
#include "../plugin.h"

void plugin_help(void) {
    printf("Available plugins:\n");
    for (int i = 0; i < plugin_count; i++) {
        printf("%2d) %s\n", i + 1, plugins[i].name);
    }
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("help", plugin_help);
}