#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"
#include "popup.h"

void plugin_update(WINDOW *parent)
{
    (void)parent;

    WINDOW *win = popup_create(9, 48, "Update Episode");

    char buf[16];
    int index;

    echo();
    mvwprintw(win, 2, 2, "Movie number:");
    mvwgetnstr(win, 2, 17, buf, sizeof(buf)-1);
    index = atoi(buf) - 1;

    if (index < 0 || index >= movie_count) {
        mvwprintw(win, 6, 2, "Invalid index");
        wrefresh(win);
        wgetch(win);
        goto done;
    }

    next_episode(index);
    mvwprintw(win, 6, 2, "Episode updated");
    wrefresh(win);
    wgetch(win);

done:
    noecho();
    popup_close(win);
}

__attribute__((constructor))
static void register_me(void)
{
    register_plugin("update", plugin_update);
}
