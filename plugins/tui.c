#include <ncurses.h>
#include "../movie.h"

static void draw_lof_screen(void);

void plugin_tui(void) {
    // initialize ncurces
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    draw_lof_screen();
    refresh();

    // wait until user press 'q'
    int ch;
    while ((ch = getch()) != 'q') {
        // pass just for now
    }

    // restore terminal
    endwin();
}

static void draw_lof_screen(void) {
    clear();

    mvprintw(0, 0, "LOF (ncurses)");
    mvprintw(1, 0, "Press q to quit");

    int row = 3;

    for (int i = 0; i < movie_count; i++) {
        if (movies[i].is_series) {
            mvprintw(row, 0,
                     "%2d) %-20s S%02dE%02d",
                     i + 1,
                     movies[i].title,
                     movies[i].season,
                     movies[i].episode);
        } else {
            mvprintw(row, 0,
                     "%2d) %-20s (movie)",
                     i + 1,
                     movies[i].title);
        }
        row++;
    }
}
