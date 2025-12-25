#include <ncursesw/ncurses.h>
#include <string.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_search(WINDOW *win) {
    char query[TITLE_LEN];
    int found = 0;

    // Clear the window and draw border
    werase(win);
    box(win, 0, 0);
    wrefresh(win);

    // Prompt user for query
    mvwprintw(win, 1, 2, "Search query: ");
    wrefresh(win);

    // Get user input
    echo();
    wgetnstr(win, query, sizeof(query) - 1);
    noecho();

    query[strcspn(query, "\n")] = '\0';  // remove newline

    // Clear space for results
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "==== Search Results ====");
    int row = 2;

    // Search through movies
    for (int i = 0; i < movie_count; i++) {
        if (strstr(movies[i].title, query)) {
            if (movies[i].is_series) {
                mvwprintw(win, ++row, 2, "%2d) %-25s S%02dE%02d",
                          i + 1, movies[i].title, movies[i].season, movies[i].episode);
            } else {
                mvwprintw(win, ++row, 2, "%2d) %-25s (movie)", i + 1, movies[i].title);
            }
            found = 1;
        }
    }

    if (!found) {
        mvwprintw(win, ++row, 2, "No matches found.");
    }

    mvwprintw(win, ++row + 1, 2, "Press any key to continue...");
    wrefresh(win);

    wgetch(win);  // wait for user
}

// Register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("search", plugin_search);
}
