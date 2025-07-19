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

    char buf[MAX_STR];

    // Title
    printf("Enter title [%s]: ", it->title);
    if (fgets(buf, MAX_STR, stdin) && buf[0] != '\n')
        strncpy(it->title, buf, MAX_STR);
    it->title[strcspn(it->title, "\n")] = 0;

    // Value
    printf("Enter value [%d]: ", it->value);
    if (fgets(buf, MAX_STR, stdin) && buf[0] != '\n')
        it->value = atoi(buf);

    // Constant
    printf("Enter constant [%s]: ", it->constant);
    if (fgets(buf, MAX_STR, stdin) && buf[0] != '\n')
        strncpy(it->constant, buf, MAX_STR);
    it->constant[strcspn(it->constant, "\n")] = 0;

    // Comment
    printf("Enter comment [%s]: ", it->comment);
    if (fgets(buf, MAX_STR, stdin) && buf[0] != '\n')
        strncpy(it->comment, buf, MAX_STR);
    it->comment[strcspn(it->comment, "\n")] = 0;

    printf("Item updated.\n");
}

// The plugin_register function called by dlopen in main program
void plugin_register(void) {
    register_plugin("update", plugin_update);
}
