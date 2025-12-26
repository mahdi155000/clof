#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "popup.h"
#include "../movie.h"
#include "../plugin.h"

/* =========================================================
 * Modes
 * ========================================================= */

typedef enum {
    MODE_NORMAL,
    MODE_NUMBER,
    MODE_COMMAND
} InputMode;

/* =========================================================
 * Action menus
 * ========================================================= */

typedef enum {
    SERIES_ADD,
    SERIES_REMOVE,
    SERIES_INFO,
    SERIES_CANCEL,
    SERIES_ACTION_COUNT
} SeriesAction;

typedef enum {
    MOVIE_MARK_WATCHED,
    MOVIE_INFO,
    MOVIE_CANCEL,
    MOVIE_ACTION_COUNT
} MovieAction;

static const char *series_action_items[] = {
    "Add episode",
    "Remove episode",
    "Show full info",
    "Cancel"
};

static const char *movie_action_items[] = {
    "Toggle watched",
    "Show info",
    "Cancel"
};

/* =========================================================
 * Drawing
 * ========================================================= */

static void draw_screen(int selected, InputMode mode,
                        const char *status, const char *input)
{
    clear();
    mvprintw(0, 0, "clof (ncursesw)");
    mvprintw(1, 0, "↑/↓ move | ENTER menu | n number | : command | q quit");

    int row = 3;

    for (int i = 0; i < movie_count; i++) {
        wchar_t mark[2] = L" ";
        if (movies[i].watched)
            mark[0] = L'\u2714';
        mark[1] = 0;

        if (i == selected)
            attron(A_REVERSE);

        if (movies[i].is_series) {
            mvprintw(row, 0, "%2d) [%ls] %-20s S%02dE%02d",
                     i + 1, mark,
                     movies[i].title,
                     movies[i].season,
                     movies[i].episode);
        } else {
            mvprintw(row, 0, "%2d) [%ls] %-20s (movie)",
                     i + 1, mark,
                     movies[i].title);
        }

        if (i == selected)
            attroff(A_REVERSE);

        row++;
    }

    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    mvhline(maxy - 3, 0, '-', maxx);

    if (mode == MODE_COMMAND)
        mvprintw(maxy - 2, 0, ":%s", input);
    else if (mode == MODE_NUMBER)
        mvprintw(maxy - 2, 0, "NUMBER: %s", input);
    else
        mvprintw(maxy - 2, 0, "NORMAL MODE");

    mvprintw(maxy - 1, 0, "%s", status);
    clrtoeol();
    refresh();
}

/* =========================================================
 * Popup menu
 * ========================================================= */

static int popup_menu(const char **items, int count)
{
    int choice = 0, ch;

    WINDOW *win = newwin(count + 2, 32,
                         (LINES - count - 2) / 2,
                         (COLS - 32) / 2);

    box(win, 0, 0);
    keypad(win, TRUE);

    while (1) {
        for (int i = 0; i < count; i++) {
            if (i == choice)
                wattron(win, A_REVERSE);
            mvwprintw(win, i + 1, 2, "%s", items[i]);
            if (i == choice)
                wattroff(win, A_REVERSE);
        }

        wrefresh(win);
        ch = wgetch(win);

        if (ch == KEY_UP && choice > 0)
            choice--;
        else if (ch == KEY_DOWN && choice < count - 1)
            choice++;
        else if (ch == '\n' || ch == KEY_ENTER) {
            delwin(win);
            return choice;
        }
        else if (ch == 27) {
            delwin(win);
            return count - 1;
        }
    }
}

// ------------- show info function on popup menu --------------------
static void tui_popup_series_info(int index)
{
    int h = 9, w = 50;
    WINDOW *win = popup_create(h, w, "Series Info");

    Movie *m = &movies[index];

    mvwprintw(win, 2, 2, "Title   : %s", m->title);
    mvwprintw(win, 3, 2, "Genre   : %s", m->genre);
    mvwprintw(win, 4, 2, "Season  : %02d", m->season);
    mvwprintw(win, 5, 2, "Episode : %02d", m->episode);
    mvwprintw(win, 6, 2, "Watched : %s", m->watched ? "YES" : "NO");

    mvwprintw(win, h - 2, 2, "Press any key to close");
    wrefresh(win);

    wgetch(win);
    popup_close(win);
}

static void tui_popup_movie_info(int index)
{
    int h = 8, w = 50;
    WINDOW *win = popup_create(h, w, "Movie Info");

    Movie *m = &movies[index];

    mvwprintw(win, 2, 2, "Title   : %s", m->title);
    mvwprintw(win, 3, 2, "Genre   : %s", m->genre);
    mvwprintw(win, 4, 2, "Watched : %s", m->watched ? "YES" : "NO");

    mvwprintw(win, h - 2, 2, "Press any key to close");
    wrefresh(win);

    wgetch(win);
    popup_close(win);
}

/* =========================================================
 * Execute command (SAFE)
 * ========================================================= */

static void execute_command(const char *cmd, char *status, int len)
{
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(cmd, plugins[i].name) == 0) {

            /* Leave ncurses safely */
            def_prog_mode();
            endwin();

            // plugins[i].func();   // printf / scanf allowed here
            plugins[i].func(stdscr);

            /* Restore ncurses */
            reset_prog_mode();
            refresh();

            snprintf(status, len, "%s done", cmd);
            return;
        }
    }

    snprintf(status, len, "Unknown command: %s", cmd);
}

/* =========================================================
 * TUI entry
 * ========================================================= */

void plugin_tui(void)
{
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int ch, selected = 0;
    InputMode mode = MODE_NORMAL;

    char status[256] = "";
    char input[64] = "";
    int in_len = 0;

    while (1) {
        draw_screen(selected, mode, status, input);
        ch = getch();

        /* NUMBER MODE */
        if (mode == MODE_NUMBER) {
            if ((ch >= '0' && ch <= '9') || (ch == '-' && in_len == 0)) {
                input[in_len++] = ch;
                input[in_len] = 0;
            }
            else if (ch == '\n') {
                int val = atoi(input);
                int idx = abs(val) - 1;

                if (idx >= 0 && idx < movie_count) {
                    selected = idx;
                    if (val > 0) next_episode(idx);
                    else prev_episode(idx);
                    snprintf(status, sizeof(status), "Episode updated");
                } else {
                    snprintf(status, sizeof(status), "Out of range");
                }

                mode = MODE_NORMAL;
                in_len = 0;
                input[0] = 0;
            }
            else if (ch == 27) {
                mode = MODE_NORMAL;
                in_len = 0;
                input[0] = 0;
            }
            continue;
        }

        /* COMMAND MODE */
        if (mode == MODE_COMMAND) {
            if (ch == '\n') {
                execute_command(input, status, sizeof(status));
                mode = MODE_NORMAL;
                in_len = 0;
                input[0] = 0;
            }
            else if (ch == 27) {
                mode = MODE_NORMAL;
                in_len = 0;
                input[0] = 0;
            }
            else if (ch == KEY_BACKSPACE || ch == 127) {
                if (in_len > 0) input[--in_len] = 0;
            }
            else if (ch >= 32 && ch <= 126) {
                input[in_len++] = ch;
                input[in_len] = 0;
            }
            continue;
        }

        /* NORMAL MODE */
        if (ch == 'q') break;
        else if (ch == KEY_UP && selected > 0) selected--;
        else if (ch == KEY_DOWN && selected < movie_count - 1) selected++;
        else if (ch == 'n') {
            mode = MODE_NUMBER;
            in_len = 0;
            input[0] = 0;
        }
        else if (ch == ':') {
            mode = MODE_COMMAND;
            in_len = 0;
            input[0] = 0;
        }
        else if (ch == '\n') {
            if (movies[selected].is_series) {
                int a = popup_menu(series_action_items, SERIES_ACTION_COUNT);
                if (a == SERIES_ADD) next_episode(selected);
                else if (a == SERIES_REMOVE) prev_episode(selected);
                else if (a == SERIES_INFO) tui_popup_series_info(selected);
            } else {
                int a = popup_menu(movie_action_items, MOVIE_ACTION_COUNT);

                if (a == MOVIE_MARK_WATCHED) {
                    movies[selected].watched ^= 1;
                    snprintf(status, sizeof(status), "Watch status toggled");
                }
                else if (a == MOVIE_INFO) {
                    tui_popup_movie_info(selected);
                }
            }
        }
    }

    endwin();
}
