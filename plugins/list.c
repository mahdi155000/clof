#include <ncursesw/ncurses.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_list(WINDOW *win) {
    // Clear the window and draw a border
    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 1, 2, "==== clof ====");
    int row = 2;

    for (int i = 0; i < movie_count; i++) {
        if (movies[i].is_series) {
            mvwprintw(win, ++row, 2, "%2d) %-25s S%02dE%02d %s",
                      i + 1,
                      movies[i].title,
                      movies[i].season,
                      movies[i].episode,
                      movies[i].watched ? "[✔]" : "[ ]");
        } else {
            mvwprintw(win, ++row, 2, "%2d) %-25s (movie) %s",
                      i + 1,
                      movies[i].title,
                      movies[i].watched ? "[✔]" : "[ ]");
        }
    }

    mvwprintw(win, ++row + 1, 2, "Press any key to continue...");
    wrefresh(win);

    wgetch(win); // wait for user input
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("list", plugin_list);
}
