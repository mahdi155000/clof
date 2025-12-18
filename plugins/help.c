#include <stdio.h>
#include "../plugin.h"

extern Plugin plugins[];
extern int plugin_count;

void plugin_help(void) {
    for (int i = 0; i < plugin_count; i++){ 
        printf("%-10s - %s\n",
            plugins[i].name,
            plugins[i].help);
    }
}