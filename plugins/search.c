#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <string.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_search(WINDOW *win) {
    char query[TITLE_LEN];
    int found = 0;

    wclear(win);
    wprintw(win, "Search query: ");
    wrefresh(win);

    echo();
    wgetnstr(win, query, sizeof(query) - 1);
    noecho();
    query[strcspn(query, "\n")] = 0;

    wprintw(win, "\n==== Search Results ====\n");

    for (int i = 0; i < movie_count; i++) {
        if (strstr(movies[i].title, query)) {
            if (movies[i].is_series) {
                wprintw(win, "%2d) %-25s S%02dE%02d\n",
                        i + 1, movies[i].title,
                        movies[i].season, movies[i].episode);
            } else {
                wprintw(win, "%2d) %-25s (movie)\n", i + 1, movies[i].title);
            }
            found = 1;
        }
    }

    if (!found) wprintw(win, "No matches found.\n");
    wrefresh(win);
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("search", plugin_search);
}
