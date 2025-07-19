#include <stdio.h>
#include <string.h>
#include "../../storage.h"
#include "../../plugins_manager.h"

// Plugin functionality
void plugin_update(int argc, char **argv, Item *M_L, int *item_count) {
    (void)argc; (void)argv;
    if (*item_count == 0) {
        printf("No items to update.\n");
        return;
    }
    int idx;
    printf("Enter item number to update (1-%d): ", *item_count);
    if (scanf("%d", &idx) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // clear input buffer
        return;
    }
    while (getchar() != '\n'); // clear input buffer
    idx--;
    if (idx < 0 || idx >= *item_count) {
        printf("Out of range.\n");
        return;
    }
    Item *it = &M_L[idx];
    printf("Enter title: ");
    if (!fgets(it->title, MAX_STR, stdin)) return;
    it->title[strcspn(it->title, "\n")] = 0;
    printf("Enter value: ");
    if (scanf("%d", &it->value) != 1) {
        printf("Invalid value.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    printf("Enter constant: ");
    if (!fgets(it->constant, MAX_STR, stdin)) return;
    it->constant[strcspn(it->constant, "\n")] = 0;
    printf("Enter comment: ");
    if (!fgets(it->comment, MAX_STR, stdin)) return;
    it->comment[strcspn(it->comment, "\n")] = 0;
    printf("Item updated.\n");
}
	

// The plugin_register function called by dlopen in main program
void plugin_register(void) {
    register_plugin("update", plugin_update);
}
