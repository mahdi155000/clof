// plugins/show/show.c
#include <stdio.h>
#include <string.h>
#include "../../storage.h"

void plugin_register(void);
void plugin_show(int argc, char **argv, Item *M_L, int *item_count);

void plugin_register(void) {
    register_plugin("show", plugin_show);
}

void plugin_show(int argc, char **argv, Item *M_L, int *item_count) {
    if (argc > 1 && strcmp(argv[1], "item") == 0) {
        // Full display
        for (int i = 0; i < *item_count; i++) {
            printf("Item %d:\n", i + 1);
            printf("  ID       : %d\n", M_L[i].id);
            printf("  Title    : %s\n", M_L[i].title);
            printf("  Value    : %d\n", M_L[i].value);
            printf("  Constant : %s\n", M_L[i].constant);
            printf("  Comment  : %s\n", M_L[i].comment);
            printf("--------------------------------------------------------\n");
        }
    } else {
        // Minimal display
        for (int i = 0; i < *item_count; ++i) {
            printf("%3d) %-30s | %4d\n", i + 1, M_L[i].title, M_L[i].value);
        }
    }
}
