#include "popup.h"

WINDOW *popup_create(int h, int w, const char *title)
{
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;

    WINDOW *win = newwin(h, w, y, x);
    box(win, 0, 0);
    keypad(win, TRUE);

    if (title)
        mvwprintw(win, 0, 2, " %s ", title);

    wrefresh(win);
    return win;
}

void popup_close(WINDOW *win)
{
    delwin(win);
}
