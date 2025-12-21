#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

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
 * Action menu
 * ========================================================= */

typedef enum {
    ACTION_Add,
    ACTION_INFO,
    ACTION_CANCEL,
    ACTION_COUNT
} Action;

typedef enum {
    SERIES_Add,
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


static const char *series_action_items[SERIES_ACTION_COUNT] = {
    "Add episode",
    "Show info",
    "Cancel"
};

static const char *movie_action_items[MOVIE_ACTION_COUNT] = {
    "Mark watched",
    "Show info",
    "Cancel"
};


/* =========================================================
 * Drawing
 * ========================================================= */

static void draw_screen(int selected,
                        InputMode mode,
                        const char *status,
                        const char *input)
{
    clear();

    mvprintw(0, 0, "clof (ncurses)");
    mvprintw(1, 0,
        "up/down arrow move | ENTER menu | n number | : command | q quit");

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

    int maxy;
    getmaxyx(stdscr, maxy, (int){0});

    mvhline(maxy - 3, 0, '-', COLS);

    if (mode == MODE_NUMBER)
        mvprintw(maxy - 2, 0, "NUMBER: %s", input);
    else if (mode == MODE_COMMAND)
        mvprintw(maxy - 2, 0, ":%s", input);
    else
        mvprintw(maxy - 2, 0, "NORMAL MODE");

    mvprintw(maxy - 1, 0, "%s", status);

    refresh();
}

/* =========================================================
 * Action menu
 * ========================================================= */

 static int generic_menu(const char **items, int count)
{
    int choice = 0;
    int ch;

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
        else if (ch == 27) { /* ESC */
            delwin(win);
            return count - 1; /* cancel */
        }
    }
}

/* =========================================================
 * Command execution
 * ========================================================= */

static void execute_command(const char *cmd, char *status, int len)
{
    // Temporarily suspend ncurses to run CLI commands
    def_prog_mode();    // Save current terminal mode
    endwin();           // End ncurses mode

    if (strcmp(cmd, "add") == 0) {
        plugin_add();
        snprintf(status, len, "add done");
    }
    else if (strcmp(cmd, "remove") == 0) {
        plugin_remove();
        snprintf(status, len, "remove done");
    }
    else if (strcmp(cmd, "update") == 0) {
        plugin_update();
        snprintf(status, len, "update done");
    }
    else if (strcmp(cmd, "help") == 0) {
        plugin_help();
        snprintf(status, len, "help");
    }
    else if (strcmp(cmd, "list") == 0) {
        plugin_list();
        snprintf(status, len, "list");
    }
    else if (strcmp(cmd, "search") == 0) {
        plugin_search();
        snprintf(status, len, "search done");
    }
    else {
        snprintf(status, len, "Unknown command: %s", cmd);
    }

    // Resume ncurses
    reset_prog_mode();  // Restore terminal mode
    refresh();          // Refresh the screen
}

/* =========================================================
 * Plugin entry
 * ========================================================= */

void plugin_tui(void)
{
    int ch;
    int selected = 0;

    InputMode mode = MODE_NORMAL;

    char status[80] = "";
    char input[16]  = "";
    int  in_len     = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    while (1) {
        draw_screen(selected, mode, status, input);
        ch = getch();

        /* ================= NUMBER MODE ================= */
        if (mode == MODE_NUMBER) {

            if ((ch >= '0' && ch <= '9') ||
                (ch == '-' && in_len == 0)) {

                if (in_len < (int)sizeof(input) - 1) {
                    input[in_len++] = ch;
                    input[in_len] = '\0';
                }
            }
            else if (ch == '\n' || ch == KEY_ENTER) {
                int value = atoi(input);
                int index = abs(value) - 1;

                if (index >= 0 && index < movie_count) {
                    selected = index;

                    if (value > 0)
                        next_episode(index);
                    else
                        prev_episode(index);

                    save_movies();
                    snprintf(status, sizeof(status), "Updated");
                } else {
                    snprintf(status, sizeof(status), "Out of range");
                }

                in_len = 0;
                input[0] = '\0';
                mode = MODE_NORMAL;
            }
            else if (ch == 27) {
                in_len = 0;
                input[0] = '\0';
                mode = MODE_NORMAL;
                snprintf(status, sizeof(status), "Canceled");
            }

            continue;
        }

        /* ================= COMMAND MODE ================= */
        if (mode == MODE_COMMAND) {

            if (ch == '\n' || ch == KEY_ENTER) {
                execute_command(input, status, sizeof(status));
                in_len = 0;
                input[0] = '\0';
                mode = MODE_NORMAL;
            }
            else if (ch == 27) {
                in_len = 0;
                input[0] = '\0';
                mode = MODE_NORMAL;
                snprintf(status, sizeof(status), "Canceled");
            }
            else if (ch == KEY_BACKSPACE || ch == 127) {
                if (in_len > 0) {
                    input[--in_len] = '\0';
                }
            }
            else if (ch >= 32 && ch <= 126) {
                if (in_len < (int)sizeof(input) - 1) {
                    input[in_len++] = ch;
                    input[in_len] = '\0';
                }
            }

            continue;
        }

        /* ================= NORMAL MODE ================= */
        if (ch == 'q')
            break;
        else if (ch == KEY_UP && selected > 0)
            selected--;
        else if (ch == KEY_DOWN && selected < movie_count - 1)
            selected++;
        else if (ch == 'n') {
            mode = MODE_NUMBER;
            in_len = 0;
            input[0] = '\0';
        }
        else if (ch == ':') {
            mode = MODE_COMMAND;
            in_len = 0;
            input[0] = '\0';
        }
        else if (ch == '\n' || ch == KEY_ENTER) {

    if (movies[selected].is_series) {
        int a = generic_menu(
            series_action_items,
            SERIES_ACTION_COUNT
        );

        if (a == SERIES_Add) {
            next_episode(selected);
            save_movies();
            snprintf(status, sizeof(status), "Episode Addd");
        }
        else if (a == SERIES_INFO) {
            snprintf(status, sizeof(status),
                "%s S%02dE%02d",
                movies[selected].title,
                movies[selected].season,
                movies[selected].episode);
        }
    }
        else {
            int a = generic_menu(
                movie_action_items,
                MOVIE_ACTION_COUNT
            );

            if (a == MOVIE_MARK_WATCHED) {
                movies[selected].episode = 1; /* example */
                save_movies();
                snprintf(status, sizeof(status), "Marked watched");
            }
            else if (a == MOVIE_INFO) {
                snprintf(status, sizeof(status),
                    "%s (movie)",
                    movies[selected].title);
            }
        }
    }

    }

    endwin();
}
