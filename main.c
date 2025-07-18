#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "storage.h"
#include "plugins_manager.h"
#include <readline/readline.h>
#include <readline/history.h>

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
// void show_items() {
//     printf("Hello world!\n");
//     printf("----------------------------------------------------------------------------------\n");

//     for (int i = 0; i < item_count; i++) {
//         Item *it = &M_L[i];
//         printf("%3d) %-65s | %4d\n", i + 1, it->title, it->value);
//     }

//     printf("----------------------------------------------------------------------------------\n");
//     printf("Enter N to +1, -N to -1, or type plugin command, Q to quit\n");
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

        // printf("Loading plugin: %s\n", so_path);

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

// --- Enhanced auto-completion logic for plugin commands and subcommands ---
static char *plugin_command_generator(const char *text, int state) {
    static int list_index = 0;
    static int len = 0;
    if (state == 0) {
        list_index = 0;
        len = strlen(text);
    }
    int count = get_plugin_count();
    while (list_index < count) {
        const Plugin *p = get_plugin(list_index++);
        if (p && strncmp(p->name, text, len) == 0) {
            return strdup(p->name);
        }
    }
    return NULL;
}

// Subcommand completion for 'show' plugin
static char *show_subcmd_generator(const char *text, int state) {
    static int idx = 0;
    static const char *subcmds[] = {"item", NULL};
    if (state == 0) idx = 0;
    // Only offer subcommands if none are already present
    // Check if 'item' is already in the input
    char *buf = rl_line_buffer;
    if (strstr(buf, "item") != NULL) return NULL;
    while (subcmds[idx]) {
        if (strncmp(subcmds[idx], text, strlen(text)) == 0)
            return strdup(subcmds[idx++]);
        idx++;
    }
    return NULL;
}

static char **plugin_completion(const char *text, int start, int end) {
    rl_attempted_completion_over = 1; // Prevent filename completion fallback
    // Only complete the first word (the command)
    if (start == 0) {
        return rl_completion_matches(text, plugin_command_generator);
    }
    // Subcommand completion for 'show'
    // Get the current input buffer
    char *buf = rl_line_buffer;
    // Extract the first word (command)
    char cmd[64] = {0};
    sscanf(buf, "%63s", cmd);
    if (strcmp(cmd, "show") == 0 && start > 0) {
        return rl_completion_matches(text, show_subcmd_generator);
    }
    // No completion for other plugins' arguments
    return NULL;
}

int main(void) {
    char buf[64];  // increased buffer size for commands

    load_items("data.bin");
    load_plugins("./plugins");
    if (item_count == 0) load_default();
    // Replace the show_items() with run_plugin
    // show_items();
    run_plugin("show", M_L, &item_count);

    rl_attempted_completion_function = plugin_completion;

    while (1) {
        char *line = readline("-> ");
        if (!line) break;
        if (*line) add_history(line);
        // Remove trailing newline if present
        line[strcspn(line, "\n")] = 0;
        if (line[0] == 'q' || line[0] == 'Q') { free(line); break; }
        // Check if input is numeric (with optional +/-)
        char *endptr;
        long n = strtol(line, &endptr, 10);
        if (*endptr == '\0') {
            int idx = abs((int)n) - 1;
            if (idx < 0 || idx >= item_count) {
                printf("Out of range\n\n");
                free(line);
                continue;
            }
            M_L[idx].value += (n > 0) ? 1 : -1;
            printf("\n");
        } else {
            run_plugin(line, M_L, &item_count);
        }
        free(line);
    }

    printf("Bye!\n");
    save_items("data.bin");
    return 0;
}
