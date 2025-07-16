#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

typedef void (*plugin_func_t)(void);

typedef struct {
    const char *name;        // command name, e.g. "add"
    plugin_func_t func;      // pointer to plugin function
} Plugin;

void register_plugin(const char *name, plugin_func_t func);
void run_plugin(const char *name);
void list_plugins(void);

#endif

