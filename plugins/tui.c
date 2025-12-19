#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
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
    ACTION_COUNT
} ActionType;

static const char *action_items[ACTION_COUNT] = {
    "Advance",
    "Info",
    "Edit",
    "Tags",
    "Cancel"
};

/* =========================================================
 * Input modes
 * ========================================================= */

typedef enum {
    MODE_NORMAL,
    MODE_NUMBER
} InputMode;

/* =========================================================
 * Draw screen
 * ========================================================= */

static void draw_lof_screen(int selected,
                            const char *status,
                            const char *numbuf,
                            InputMode mode)
{
    clear();

    mvprintw(0, 0, "clof (ncurses)");
    mvprintw(1, 0,
             "↑ ↓ move | ENTER menu | n number-mode | q quit");

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

    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    mvhline(maxy - 3, 0, '-', maxx);

    if (mode == MODE_NUMBER)
        mvprintw(maxy - 2, 0, "NUMBER MODE: %s", numbuf);
    else
        mvprintw(maxy - 2, 0, "MODE: NORMAL");

    mvprintw(maxy - 1, 0, "%s", status);

    refresh();
}

/* =========================================================
 * Action menu (unchanged)
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

        if (ch == KEY_UP && choice > 0)
            choice--;
        else if (ch == KEY_DOWN && choice < ACTION_COUNT - 1)
            choice++;
        else if (ch == '\n' || ch == KEY_ENTER) {
            delwin(win);
            return (ActionType)choice;
        } else if (ch == 27 || ch == 'q') {
            delwin(win);
            return ACTION_CANCEL;
        }
    }
}

/* =========================================================
 * ENTER handler (normal mode)
 * ========================================================= */

static void handle_enter(int selected, char *status, int len) {
    ActionType action = action_menu();

    switch (action) {
    case ACTION_ADVANCE:
        snprintf(status, len, "Advance selected");
        break;
    case ACTION_INFO:
        snprintf(status, len, "Title: %s", movies[selected].title);
        break;
    default:
        snprintf(status, len, "Canceled");
        break;
    }
}

/* =========================================================
 * Plugin entry
 * ========================================================= */

void plugin_tui(void) {
    int ch;
    int selected = 0;

    InputMode mode = MODE_NORMAL;

    char status[80] = "";
    char numbuf[8]  = "";
    int  numlen     = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    while (1) {
        draw_lof_screen(selected, status, numbuf, mode);
        ch = getch();

        /* ================= NUMBER MODE ================= */
        if (mode == MODE_NUMBER) {

            if ((ch >= '0' && ch <= '9') ||
                (ch == '-' && numlen == 0)) {

                if (numlen < (int)sizeof(numbuf) - 1) {
                    numbuf[numlen++] = ch;
                    numbuf[numlen] = '\0';
                }

            } else if (ch == '\n' || ch == KEY_ENTER) {

                int value = atoi(numbuf);
                int index = abs(value) - 1;

                if (value == 0) {
                    snprintf(status, sizeof(status), "Invalid number");
                } else if (index < 0 || index >= movie_count) {
                    snprintf(status, sizeof(status), "Out of range");
                } else {
                    selected = index;

                    if (value > 0) {
                        next_episode(index);
                        snprintf(status, sizeof(status),
                                 "Advanced: %s",
                                 movies[index].title);
                    } else {
                        prev_episode(index);
                        snprintf(status, sizeof(status),
                                 "Reverted: %s",
                                 movies[index].title);
                    }

                    save_movies();
                }

                numbuf[0] = '\0';
                numlen = 0;
                mode = MODE_NORMAL;

            } else if (ch == 27) {   /* ESC */
                numbuf[0] = '\0';
                numlen = 0;
                mode = MODE_NORMAL;
                snprintf(status, sizeof(status), "Number mode canceled");
            }

            continue;
        }

        /* ================= NORMAL MODE ================= */
        if (ch == 'q') {
            break;
        } else if (ch == KEY_UP && selected > 0) {
            selected--;
        } else if (ch == KEY_DOWN && selected < movie_count - 1) {
            selected++;
        } else if (ch == '\n' || ch == KEY_ENTER) {
            handle_enter(selected, status, sizeof(status));
        } else if (ch == 'n') {
            mode = MODE_NUMBER;
            numbuf[0] = '\0';
            numlen = 0;
            snprintf(status, sizeof(status), "Number mode");
        }
    }

    endwin();
}
