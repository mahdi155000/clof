#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_update(WINDOW *parent_win) {
    int height = 6, width = 50;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW *win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    keypad(win, TRUE);

    char input_str[16];
    int input;
    int index;

    // Enable echo so the user can see what they type
    echo();

    wmove(win, 1, 2);
    wprintw(win, "Entry number (+N / -N): ");
    wrefresh(win);
    wgetnstr(win, input_str, sizeof(input_str) - 1);

    input = atoi(input_str);
    index = abs(input) - 1;

    if (index < 0 || index >= movie_count) {
        wmove(win, 3, 2);
        wprintw(win, "Invalid entry number.");
        wrefresh(win);
        wgetch(win);  // wait for key
        noecho();
        delwin(win);
        return;
    }

    if (!movies[index].is_series) {
        wmove(win, 3, 2);
        wprintw(win, "'%s' is not a series.", movies[index].title);
        wrefresh(win);
        wgetch(win);
        noecho();
        delwin(win);
        return;
    }

    if (input > 0) next_episode(index);
    else prev_episode(index);

    wmove(win, 3, 2);
    wprintw(win, "Updated: %s S%02dE%02d",
            movies[index].title, movies[index].season, movies[index].episode);
    wrefresh(win);

    wgetch(win);  // wait for user to see result

    noecho();
    delwin(win);
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("update", plugin_update);
}
