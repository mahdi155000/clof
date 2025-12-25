#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_reid(WINDOW *win) {
    if (movie_count < 2) {
        wprintw(win, "Not enough movies to reposition.\n");
        wrefresh(win);
        return;
    }

    char tmp[16];
    int from = -1, to = -1;

    echo();
    wprintw(win, "Move movie at position #: ");
    wrefresh(win);
    wgetnstr(win, tmp, sizeof(tmp) - 1);
    from = atoi(tmp) - 1;

    wprintw(win, "Move to position #: ");
    wrefresh(win);
    wgetnstr(win, tmp, sizeof(tmp) - 1);
    to = atoi(tmp) - 1;
    noecho();

    if (from < 0 || from >= movie_count || to < 0 || to >= movie_count) {
        wprintw(win, "Invalid positions.\n");
        wrefresh(win);
        return;
    }
    if (from == to) {
        wprintw(win, "Already at that position.\n");
        wrefresh(win);
        return;
    }

    Movie temp = movies[from];
    if (from < to) {
        for (int i = from; i < to; i++) movies[i] = movies[i + 1];
    } else {
        for (int i = from; i > to; i--) movies[i] = movies[i - 1];
    }
    movies[to] = temp;

    wprintw(win, "Movie repositioned from %d to %d.\n", from + 1, to + 1);
    wrefresh(win);
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("reid", plugin_reid);
}
