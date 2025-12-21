#include "../plugin.h"

// Forward declarations
void plugin_lof(void);
void plugin_tui(void);
void plugin_list(void);
void plugin_add(void);
void plugin_update(void);
void plugin_remove(void);
void plugin_help(void);
void plugin_search(void);

Plugin plugins[] = {
    { "lof",    "Numeric LOF mode", plugin_lof },
    { "tui",    "Ncurses interface",    plugin_tui },
    { "list",   "Show all entries", plugin_list },
    { "add",    "Add new entry",    plugin_add },
    { "update", "Update progress",  plugin_update },
    { "remove", "Remove entry",     plugin_remove},
    {"help",    "Show help",        plugin_help},
    {"search",  "Search movies by title", plugin_search},

};


int plugin_count = sizeof(plugins) / sizeof(plugins[0]);