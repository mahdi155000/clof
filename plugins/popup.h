#ifndef POPUP_H
#define POPUP_H

#include <ncursesw/ncurses.h>

WINDOW *popup_create(int h, int w, const char *title);
void popup_close(WINDOW *win);

#endif
