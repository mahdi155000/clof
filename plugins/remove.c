#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncursesw/ncurses.h>
#include "../movie.h"
#include "../db.h"
#include "../plugin.h"

void plugin_remove(WINDOW *win)
{
    char buf[16];
    int num;
    int index;
    echo();

    // printf("Remove entry number: ");
    wprintw(win, "Remove entry number: ");
    wrefresh(win);
    wgetnstr(win,buf,sizeof(buf) -1);
    num = atoi(buf);
    // if (scanf("%d", &num) != 1) {
    //     getchar();
    //     return;
    // }
    // getchar(); // consume newline

    noecho();

    index = num - 1;

    if (index < 0 || index >= movie_count) {
        // printf("Invalid index.\n");
        wprintw(win, "Invalid index.\n");
        wrefresh(win);
        return;
    }

    // printf("Removed: %s\n", movies[index].title);
    wprintw(win,"Removed: %s\n",movies[index].title);
    wrefresh(win);

    /* shift movies left */
    for (int i = index; i < movie_count - 1; i++) {
        movies[i] = movies[i + 1];
    }

    movie_count--;

    /* persist change */
    db_save_movies();
}

/* register automatically */
__attribute__((constructor))
static void register_me(void)
{
    register_plugin("remove", plugin_remove);
}
