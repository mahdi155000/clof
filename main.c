#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "storage.h"
#include "plugins_manager.h"

#define MAX_ITEMS 1000
#define MAX_STR   100

//Item M_L[MAX_ITEMS];
//int item_count = 0;
/*
typedef struct {
    int id;
    char title[MAX_STR];
    int value;
    char constant[MAX_STR];
    char comment[MAX_STR];
} Item;

Item M_L[MAX_ITEMS];
int item_count = 0;
*/

// Sample data loader if no saved data
void load_default(void) {
    item_count = 3;
    M_L[0] = (Item){1, "Alpha", 5, "C_A", "First"};
    M_L[1] = (Item){2, "Bravo", 10, "C_B", "Second"};
    M_L[2] = (Item){3, "Charlie", 15, "C_C", "Third"};
}

// Display items
void show_items() {
    printf("ID  Title     Val  Const  Comment\n");
    for (int i = 0; i < item_count; i++) {
        Item *it = &M_L[i];
        printf("%2d) %-8s %4d  %-6s  %s\n",
               it->id, it->title, it->value, it->constant, it->comment);
    }
    printf("Enter N to +1, -N to -1, or type plugin command, Q to quit\n");
}

// Load plugins dynamically from folder
// void load_plugins(const char *plugin_root) {
//     DIR *dir = opendir(plugin_root);
//     if (!dir) {
//         perror("opendir");
//         exit(EXIT_FAILURE);
//     }

//     struct dirent *entry;
//     while ((entry = readdir(dir))) {
//         if (entry->d_type != DT_DIR || entry->d_name[0] == '.')
//             continue;

//         char so_path[256];
//         snprintf(so_path, sizeof so_path, "%s/%s/plugin.so", plugin_root, entry->d_name);

//         void *handle = dlopen(so_path, RTLD_NOW);
//         if (!handle) {
//             fprintf(stderr, "Failed to load %s: %s\n", so_path, dlerror());
//             continue;
//         }

//         void (*plugin_register)(void) = dlsym(handle, "plugin_register");
//         if (!plugin_register) {
//             fprintf(stderr, "No plugin_register in %s\n", so_path);
//             dlclose(handle);
//             continue;
//         }

//         plugin_register();  // Plugin self-registration
//     }
//     closedir(dir);
// }
void load_plugins(const char *plugin_root) {
    DIR *dir = opendir(plugin_root);
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type != DT_DIR || entry->d_name[0] == '.')
            continue;

        char so_path[256];
        snprintf(so_path, sizeof so_path, "%s/%s/plugin.so", plugin_root, entry->d_name);

        printf("Loading plugin: %s\n", so_path);

        void *handle = dlopen(so_path, RTLD_NOW);
        if (!handle) {
            fprintf(stderr, "Failed to load %s: %s\n", so_path, dlerror());
            continue;
        }

        void (*plugin_register)(void) = dlsym(handle, "plugin_register");
        if (!plugin_register) {
            fprintf(stderr, "No plugin_register in %s\n", so_path);
            dlclose(handle);
            continue;
        }

        plugin_register();
    }
    closedir(dir);
}


int main(void) {
    char buf[64];  // increased buffer size for commands

    load_items("data.bin");
    load_plugins("./plugins");
    if (item_count == 0) load_default();

    while (1) {
        show_items();
        printf("-> ");

        if (!fgets(buf, sizeof buf, stdin)) break;
        buf[strcspn(buf, "\n")] = 0;  // remove newline

        if (buf[0] == 'q' || buf[0] == 'Q') break;

        // Check if input is numeric (with optional +/-)
        char *endptr;
        long n = strtol(buf, &endptr, 10);

        if (*endptr == '\0') {
            // Input was a number
            int idx = abs((int)n) - 1;
            if (idx < 0 || idx >= item_count) {
                printf("Out of range\n\n");
                continue;
            }
            M_L[idx].value += (n > 0) ? 1 : -1;
            printf("\n");
        } else {
            // Input was non-numeric → treat as plugin command
            run_plugin(buf, M_L, &item_count);
        }
    }

    printf("Bye!\n");
    save_items("data.bin");
    return 0;
}
