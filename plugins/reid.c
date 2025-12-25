#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"

/* Reposition (reid) plugin */
void plugin_reid(WINDOW *win) {
    if (movie_count < 2) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "Not enough movies to reposition.");
        mvwprintw(win, 3, 2, "Press any key to continue...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    char input[16];
    int from = -1, to = -1;

    // Get "from" position
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Move movie at position #: ");
    wrefresh(win);
    wgetnstr(win, input, sizeof(input) - 1);
    from = atoi(input) - 1;

    if (from < 0 || from >= movie_count) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "Invalid position.");
        mvwprintw(win, 3, 2, "Press any key to continue...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    // Get "to" position
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Move to position #: ");
    wrefresh(win);
    wgetnstr(win, input, sizeof(input) - 1);
    to = atoi(input) - 1;

    if (to < 0 || to >= movie_count) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "Invalid position.");
        mvwprintw(win, 3, 2, "Press any key to continue...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    if (from == to) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "Already at that position.");
        mvwprintw(win, 3, 2, "Press any key to continue...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    /* Move movie in the array */
    Movie temp = movies[from];

    if (from < to) {
        for (int i = from; i < to; i++)
            movies[i] = movies[i + 1];
    } else {
        for (int i = from; i > to; i--)
            movies[i] = movies[i - 1];
    }

    movies[to] = temp;

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Movie repositioned from %d to %d.", from + 1, to + 1);
    mvwprintw(win, 3, 2, "Press any key to continue...");
    wrefresh(win);
    wgetch(win);
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("reid", plugin_reid);
}
