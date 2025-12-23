#ifndef PLUGIN_H
#define PLUGIN_H

/*
 * Plugin function type
 * - takes no arguments
 * - returns nothing
 */
typedef void (*plugin_func_t)(void);

/*
 * One command = one plugin
 */
typedef struct {
    const char *name;   // command name (e.g. "add")
    const char *help;   // help text
    plugin_func_t run;  // function to execute
} Plugin;

/* =====================================================
 * Plugin entry points (function declarations)
 * ===================================================== */

void plugin_list(void);
void plugin_add(void);
void plugin_update(void);
void plugin_remove(void);
void plugin_help(void);
void plugin_tui(void);
void plugin_search(void);
void plugin_reid(void);

/* =====================================================
 * Plugin registry (defined in plugins.c)
 * ===================================================== */

extern Plugin plugins[];
extern int plugin_count;

#endif
