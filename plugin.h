// plugin.h
#ifndef PLUGIN_H
#define PLUGIN_H

typedef void (*plugin_func_t)(void);

typedef struct {
    const char *name;
    plugin_func_t func;
} PluginEntry;

#define MAX_PLUGINS 128

extern PluginEntry plugins[];
extern int plugin_count;

// Add this line:
void plugin_tui(void);

// Function to register plugins
void register_plugin(const char *name, plugin_func_t func);

#endif
