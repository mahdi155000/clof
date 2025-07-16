#ifndef PLUGINS_MANAGER_H
#define PLUGINS_MANAGER_H

typedef void (*plugin_func_t)(void);

void register_plugin(const char *name, plugin_func_t func);
void run_plugin(const char *name);
void list_plugins(void);

#endif
