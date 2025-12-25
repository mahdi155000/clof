
#include <ncursesw/ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_add(WINDOW *parent)
{
    (void)parent; // we do NOT use stdscr or bottom window

    int h = 13;
    int w = 52;
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;

    WINDOW *win = newwin(h, w, y, x);
    box(win, 0, 0);
    keypad(win, TRUE);

    char title[TITLE_LEN] = {0};
    char buf[16] = {0};
    int is_series = 0;
    int season = 0;
    int episode = 0;

    echo();

    mvwprintw(win, 1, 2, "Add new movie / series");
    mvwprintw(win, 3, 2, "Title:");
    wrefresh(win);
    mvwgetnstr(win, 3, 10, title, TITLE_LEN - 1);

    if (title[0] == '\0') {
        mvwprintw(win, h - 2, 2, "Title cannot be empty");
        wrefresh(win);
        wgetch(win);
        goto done;
    }

    if (movie_exists(title) != -1) {
        mvwprintw(win, h - 2, 2, "Already exists");
        wrefresh(win);
        wgetch(win);
        goto done;
    }

    mvwprintw(win, 5, 2, "Is series (0/1):");
    wrefresh(win);
    mvwgetnstr(win, 5, 22, buf, sizeof(buf) - 1);
    is_series = atoi(buf);

    if (is_series) {
        mvwprintw(win, 6, 2, "Season:");
        wrefresh(win);
        mvwgetnstr(win, 6, 11, buf, sizeof(buf) - 1);
        season = atoi(buf);

        mvwprintw(win, 7, 2, "Episode:");
        wrefresh(win);
        mvwgetnstr(win, 7, 12, buf, sizeof(buf) - 1);
        episode = atoi(buf);
    }

    add_movie(title, is_series, season, episode);
    movies[movie_count - 1].watched = 0;

    mvwprintw(win, h - 2, 2, "Added successfully");
    wrefresh(win);
    wgetch(win);

done:
    noecho();
    delwin(win);
}

/* auto register */
__attribute__((constructor))
static void register_me(void)
{
    register_plugin("add", plugin_add);
}

