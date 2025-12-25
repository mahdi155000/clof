#include <ncursesw/ncurses.h>
#include "../plugin.h"

void plugin_help(WINDOW *win) {
    // Clear the window and draw a border
    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 1, 2, "Available plugins:");
    int row = 2;

    // List all registered plugins
    for (int i = 0; i < plugin_count; i++) {
        mvwprintw(win, ++row, 2, "%2d) %s", i + 1, plugins[i].name);
    }

    mvwprintw(win, ++row + 1, 2, "Press any key to continue...");
    wrefresh(win);

    wgetch(win); // wait for user input
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("help", plugin_help);
}
