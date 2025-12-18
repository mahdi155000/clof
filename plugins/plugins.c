#include "../plugin.h"

// Forward declarations
void plugin_list(void);
void plugin_add(void);
void plugin_update(void);
void plugin_remove(void);
void plugin_help(void);

Plugin plugins[] = {
    { "list",   "Show all entries", plugin_list },
    { "add",    "Add new entry",    plugin_add },
    { "update", "Update progress",  plugin_update },
    { "remove", "Remove entry",     plugin_remove},
    {"help",    "Show help",        plugin_help},

};


int plugin_count = sizeof(plugins) / sizeof(plugins[0]);