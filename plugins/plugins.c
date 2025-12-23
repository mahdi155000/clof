#include "../plugin.h"
#define MAX_PLUGINS 128


// just for test auto plugin resitration

PluginEntry plugins[MAX_PLUGINS];
int plugin_count = 0;

void register_plugin(const char *name, plugin_func_t func) {
    if (plugin_count < MAX_PLUGINS) {
        plugins[plugin_count].name = name;
        plugins[plugin_count].func = func;
        plugin_count++;
    }
}