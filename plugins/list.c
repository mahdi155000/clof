#include <ncursesw/ncurses.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_list(WINDOW *win) {
    wclear(win);
    wprintw(win, "==== clof ====\n");

    for (int i = 0; i < movie_count; i++) {
        if (movies[i].is_series) {
            wprintw(win, "%2d) %-25s S%02dE%02d %s\n",
                    i + 1, movies[i].title,
                    movies[i].season, movies[i].episode,
                    movies[i].watched ? "[✔]" : "[ ]");
        } else {
            wprintw(win, "%2d) %-25s (movie) %s\n",
                    i + 1, movies[i].title,
                    movies[i].watched ? "[✔]" : "[ ]");
        }
    }
    wrefresh(win);
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("list", plugin_list);
}
