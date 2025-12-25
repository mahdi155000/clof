#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_lof(WINDOW *win) {
    char input_str[16];
    int input;

    while (1) {
        wclear(win);
        wprintw(win, "==== LOF ====\n");
        for (int i = 0; i < movie_count; i++) {
            if (movies[i].is_series) {
                wprintw(win, "%2d) %-20s S%02dE%02d\n",
                        i + 1, movies[i].title,
                        movies[i].season, movies[i].episode);
            } else {
                wprintw(win, "%2d) %-20s (movie)\n", i + 1, movies[i].title);
            }
        }
        wprintw(win, "================\n");
        wprintw(win, "Input (+N / -N / 0 / q): ");
        wrefresh(win);

        echo();
        wgetnstr(win, input_str, sizeof(input_str) - 1);
        noecho();

        if (input_str[0] == 'q') break;

        input = atoi(input_str);
        if (input == 0) continue;

        int index = abs(input) - 1;
        if (index < 0 || index >= movie_count) {
            wprintw(win, "Invalid entry.\n");
            wrefresh(win);
            continue;
        }

        if (!movies[index].is_series) {
            wprintw(win, "'%s' is a movie.\n", movies[index].title);
            wrefresh(win);
            continue;
        }

        if (input > 0) next_episode(index);
        else prev_episode(index);
    }
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("lof", plugin_lof);
}
