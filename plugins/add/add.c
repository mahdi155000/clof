#include <stdio.h>
#include "../../plugins_manager.h"

// Plugin functionality
void plugin_add(void) {
    printf("Add plugin executed!\n");
}

// The plugin_register function called by dlopen in main program
void plugin_register(void) {
    register_plugin("add", plugin_add);
}
