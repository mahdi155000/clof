#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_add(WINDOW *win) {
    char title[TITLE_LEN];
    char tmp[16];
    int is_series = 0;
    int season = 0;
    int episode = 0;

    wprintw(win, "Title: ");
    wrefresh(win);
    wgetnstr(win, title, sizeof(title)-1);
    title[strcspn(title, "\n")] = '\0';

    if (title[0] == '\0') {
        wprintw(win, "Empty title.\n");
        wrefresh(win);
        return;
    }

    if (movie_exists(title) != -1) {
        wprintw(win, "Already exists.\n");
        wrefresh(win);
        return;
    }

    wprintw(win, "Is series? (0 = No, 1 = Yes): ");
    wrefresh(win);
    wgetnstr(win, tmp, sizeof(tmp)-1);
    is_series = atoi(tmp);

    if (is_series) {
        wprintw(win, "Season: ");
        wrefresh(win);
        wgetnstr(win, tmp, sizeof(tmp)-1);
        season = atoi(tmp);

        wprintw(win, "Episode: ");
        wrefresh(win);
        wgetnstr(win, tmp, sizeof(tmp)-1);
        episode = atoi(tmp);
    }

    add_movie(title, is_series, season, episode);
    movies[movie_count - 1].watched = 0;

    wprintw(win, "Added.\n");
    wrefresh(win);
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("add", plugin_add);
}
