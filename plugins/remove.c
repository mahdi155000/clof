#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../movie.h"
#include "../db.h"
#include "../plugin.h"
#include "popup.h"

void plugin_remove(WINDOW *parent)
{
    (void)parent;

    WINDOW *win = popup_create(8, 48, "Remove Movie");
    char buf[16];
    int index;

    echo();
    mvwprintw(win, 2, 2, "Movie number:");
    mvwgetnstr(win, 2, 17, buf, sizeof(buf)-1);
    index = atoi(buf) - 1;

    if (index < 0 || index >= movie_count) {
        mvwprintw(win, 5, 2, "Invalid index");
        wrefresh(win);
        wgetch(win);
        goto done;
    }

    db_delete_movie(movies[index].title);
    remove_movie(index);   // IMPORTANT (memory sync)
    db_save_movies();

    mvwprintw(win, 5, 2, "Removed successfully");
    wrefresh(win);
    wgetch(win);

done:
    noecho();
    popup_close(win);
}

__attribute__((constructor))
static void register_me(void)
{
    register_plugin("remove", plugin_remove);
}
