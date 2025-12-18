#include <ncurses.h>
#include "../movie.h"

static void draw_lof_screen(int selected){
    clear();

    mvprintw(0,0, "clof (ncurses)");
    mvprintw(1,0, "Use ↑ ↓ to move, q to quit");

    int row = 3;

    for (int i = 0; i < movie_count; i++){
        if (i == selected)
            attron(A_REVERSE);

        if (movies[i].is_series) {
            mvprintw(row, 0,
                "%2d) %-20s S%02dE%02d",
                i + 1,
                movies[i].title,
                movies[i].season,
                movies[i].episode);
        } else {
            mvprintw(row, 0,
                "%2d) %-20s (movie)",
                i + 1,
                movies[i].title);
        }

        if ( i == selected)
            attroff(A_REVERSE);
        
        row ++;
    }
    
    refresh();
}

void plugin_tui(void) {
    int ch;
    int selected = 0;

    // initialize ncurces
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);    // hide cursor

    while (1){
        draw_lof_screen(selected);
        ch = getch();

        if (ch == 'q') {
            break;
        } else if (ch == KEY_UP)
        {
            if (selected > 0)
                selected--;
        } else if (ch == KEY_DOWN)
        {
            if (selected < movie_count -1)
                selected++;
        }
    }

    // restore terminal
    endwin();
}
