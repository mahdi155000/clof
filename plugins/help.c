#include <ncursesw/ncurses.h>
#include "../plugin.h"
#include "popup.h"

void plugin_help(WINDOW *parent)
{
    (void)parent;

    WINDOW *win = popup_create(10, 40, "Help");

    mvwprintw(win, 2, 2, ":add     Add movie");
    mvwprintw(win, 3, 2, ":remove  Remove movie");
    mvwprintw(win, 4, 2, ":update  Next episode");
    mvwprintw(win, 5, 2, ":search  Search title");
    mvwprintw(win, 6, 2, ":help    This help");

    wrefresh(win);
    wgetch(win);
    popup_close(win);
}

__attribute__((constructor))
static void register_me(void)
{
    register_plugin("help", plugin_help);
}
