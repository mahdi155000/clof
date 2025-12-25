#include <ncursesw/ncurses.h>
#include <string.h>
#include "../movie.h"
#include "../plugin.h"
#include "popup.h"

void plugin_search(WINDOW *parent)
{
    (void)parent;

    WINDOW *win = popup_create(10, 50, "Search");
    char query[TITLE_LEN];

    echo();
    mvwprintw(win, 2, 2, "Search title:");
    mvwgetnstr(win, 2, 16, query, TITLE_LEN-1);

    int row = 4;
    for (int i = 0; i < movie_count && row < 8; i++) {
        if (strstr(movies[i].title, query))
            mvwprintw(win, row++, 2, "%d) %s", i+1, movies[i].title);
    }

    wrefresh(win);
    wgetch(win);
    noecho();
    popup_close(win);
}

__attribute__((constructor))
static void register_me(void)
{
    register_plugin("search", plugin_search);
}