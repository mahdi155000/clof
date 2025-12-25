#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "../movie.h"
#include "../db.h"
#include "../plugin.h"

void plugin_remove(WINDOW *parent_win)
{
    int height = 7, width = 60;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW *win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    keypad(win, TRUE);

    char input_str[16];
    int index;

    echo();

    wmove(win, 1, 2);
    wprintw(win, "Entry number to remove: ");
    wrefresh(win);
    wgetnstr(win, input_str, sizeof(input_str) - 1);

    index = atoi(input_str) - 1;

    if (index < 0 || index >= movie_count) {
        wmove(win, 3, 2);
        wprintw(win, "Invalid entry number.");
        wrefresh(win);
        wgetch(win);
        noecho();
        delwin(win);
        return;
    }

    wmove(win, 3, 2);
    wprintw(win, "Remove '%s'? (y/n)", movies[index].title);
    wrefresh(win);

    int ch = wgetch(win);
    if (ch != 'y' && ch != 'Y') {
        wmove(win, 4, 2);
        wprintw(win, "Canceled.");
        wrefresh(win);
        wgetch(win);
        noecho();
        delwin(win);
        return;
    }

    /* shift movies */
    for (int i = index; i < movie_count - 1; i++) {
        movies[i] = movies[i + 1];
    }
    movie_count--;

    db_save_movies();

    wmove(win, 4, 2);
    wprintw(win, "Removed successfully.");
    wrefresh(win);

    wgetch(win);

    noecho();
    delwin(win);
}

/* auto register */
__attribute__((constructor))
static void register_me(void)
{
    register_plugin("remove", plugin_remove);
}
