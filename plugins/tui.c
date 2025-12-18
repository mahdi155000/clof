#include <ncurses.h>
#include <stdio.h>
#include "../movie.h"

/* =========================================================
 * Action menu definitions
 * ========================================================= */

typedef enum {
    ACTION_ADVANCE,
    ACTION_INFO,
    ACTION_EDIT,
    ACTION_TAGS,
    ACTION_CANCEL,
    ACTION_COUNT          /* MUST be last */
} ActionType;

static const char *action_items[ACTION_COUNT] = {
    "Advance",
    "Info",
    "Edit",
    "Tags",
    "Cancel"
};

/* =========================================================
 * Draw main LOF screen
 * ========================================================= */

static void draw_lof_screen(int selected, const char *status) {
    clear();

    mvprintw(0, 0, "clof (ncurses)");
    mvprintw(1, 0, "↑ ↓ move | ENTER menu | q quit");

    int row = 3;

    for (int i = 0; i < movie_count; i++) {
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

        if (i == selected)
            attroff(A_REVERSE);

        row++;
    }

    /* status bar */
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    mvhline(maxy - 2, 0, '-', maxx);
    mvprintw(maxy - 1, 0, "%s", status);

    refresh();
}

/* =========================================================
 * Action menu (modal)
 * ========================================================= */

static ActionType action_menu(void) {
    int choice = 0;
    int ch;

    int height = ACTION_COUNT + 2;
    int width  = 24;
    int starty = (LINES - height) / 2;
    int startx = (COLS  - width)  / 2;

    WINDOW *win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    keypad(win, TRUE);

    while (1) {
        for (int i = 0; i < ACTION_COUNT; i++) {
            if (i == choice)
                wattron(win, A_REVERSE);

            mvwprintw(win, i + 1, 2, "%s", action_items[i]);

            if (i == choice)
                wattroff(win, A_REVERSE);
        }

        wrefresh(win);
        ch = wgetch(win);

        if (ch == KEY_UP && choice > 0) {
            choice--;
        } else if (ch == KEY_DOWN && choice < ACTION_COUNT - 1) {
            choice++;
        } else if (ch == '\n' || ch == KEY_ENTER) {
            delwin(win);
            return (ActionType)choice;
        } else if (ch == 27 || ch == 'q') {   /* ESC or q */
            delwin(win);
            return ACTION_CANCEL;
        }
    }
}

/* =========================================================
 * ENTER key handler (dispatch only)
 * ========================================================= */

static void handle_enter(int selected, char *status, int len) {
    if (selected < 0 || selected >= movie_count) {
        snprintf(status, len, "Invalid selection");
        return;
    }

    ActionType action = action_menu();

    switch (action) {
    case ACTION_ADVANCE:
        snprintf(status, len, "Advance selected (not implemented)");
        break;

    case ACTION_INFO:
        snprintf(status, len, "Title: %s", movies[selected].title);
        break;

    case ACTION_EDIT:
        snprintf(status, len, "Edit selected");
        break;

    case ACTION_TAGS:
        snprintf(status, len, "Tags selected");
        break;

    case ACTION_CANCEL:
    default:
        snprintf(status, len, "Canceled");
        break;
    }
}

/* =========================================================
 * Plugin entry point
 * ========================================================= */

void plugin_tui(void) {
    int ch;
    int selected = 0;
    char status[80] = "";

    /* initialize ncurses */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    while (1) {
        draw_lof_screen(selected, status);
        ch = getch();

        if (ch == 'q') {
            break;
        } else if (ch == KEY_UP && selected > 0) {
            selected--;
        } else if (ch == KEY_DOWN && selected < movie_count - 1) {
            selected++;
        } else if (ch == '\n' || ch == KEY_ENTER) {
            handle_enter(selected, status, sizeof(status));
        }
    }

    /* restore terminal */
    endwin();
}
