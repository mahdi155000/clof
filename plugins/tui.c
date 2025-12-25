#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "../movie.h"
#include "../plugin.h"
#include "../src/vlc_tracker.h"
#include "../src/vlc_queue.h"

// Forward declaration for db_save_movies
void db_save_movies(void);

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

typedef enum {
    CONFIRM_YES,
    CONFIRM_NO,
    CONFIRM_CANCEL,
    CONFIRM_ACTION_COUNT
} ConfirmAction;

static const char *series_action_items[SERIES_ACTION_COUNT] = {
    "Add episode",
    "Remove episode",
    "Show full info",
    "Cancel"
};

static const char *movie_action_items[MOVIE_ACTION_COUNT] = {
    "Toggle watched",
    "Show info",
    "Cancel"
};

static const char *confirm_action_items[CONFIRM_ACTION_COUNT] = {
    "Yes",
    "No",
    "Cancel"
};

/* =========================================================
 * Drawing
 * ========================================================= */

static void draw_screen(int selected, InputMode mode, const char *status, const char *input)
{
    clear();
    mvprintw(0, 0, "clof (ncursesw)");
    mvprintw(1, 0, "up/down move | ENTER menu | n number | : command | q quit");

    int row = 3;
    for (int i = 0; i < movie_count; i++) {
        wchar_t mark[3] = L" ";
        if (movies[i].watched) mark[0] = L'\u2714'; // ✔

        if (i == selected) attron(A_REVERSE);

        if (movies[i].is_series) {
            mvprintw(row, 0, "%2d) [%ls] %-20s S%02dE%02d",
                     i+1, mark, movies[i].title,
                     movies[i].season, movies[i].episode);
        } else {
            mvprintw(row, 0, "%2d) [%ls] %-20s (movie)",
                     i+1, mark, movies[i].title);
        }

        if (i == selected) attroff(A_REVERSE);
        row++;
    }

    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    mvhline(maxy-3, 0, '-', maxx);

    if (mode == MODE_NUMBER) mvprintw(maxy-2, 0, "NUMBER: %s", input);
    else if (mode == MODE_COMMAND) mvprintw(maxy-2, 0, ":%s", input);
    else mvprintw(maxy-2, 0, "NORMAL MODE");

    mvprintw(maxy-1, 0, "%s", status);
    clrtoeol();
    refresh();
}

/* =========================================================
 * Generic menu
 * ========================================================= */

static int generic_menu(const char **items, int count)
{
    int choice = 0, ch;
    WINDOW *win = newwin(count+2, 32, (LINES-count-2)/2, (COLS-32)/2);
    box(win, 0, 0);
    keypad(win, TRUE);

    while (1) {
        for (int i = 0; i < count; i++) {
            if (i == choice) wattron(win, A_REVERSE);
            mvwprintw(win, i+1, 2, "%s", items[i]);
            if (i == choice) wattroff(win, A_REVERSE);
        }
        wrefresh(win);

        ch = wgetch(win);
        if (ch == KEY_UP && choice > 0) choice--;
        else if (ch == KEY_DOWN && choice < count-1) choice++;
        else if (ch == '\n' || ch == KEY_ENTER) {
            delwin(win);
            return choice;
        } else if (ch == 27) {
            delwin(win);
            return count-1; // cancel
        }
    }
}

/* =========================================================
 * Confirmation popup
 * ========================================================= */

static int confirm_popup(const char *question)
{
    int choice = 0, ch;
    int height = 7;
    int width = strlen(question)+10;
    if (width < 40) width = 40;
    int starty = (LINES-height)/2;
    int startx = (COLS-width)/2;

    WINDOW *win = newwin(height, width, starty, startx);
    keypad(win, TRUE);
    box(win, 0, 0);

    mvwprintw(win, 1, 2, "%s", question);

    while (1) {
        for (int i = 0; i < CONFIRM_ACTION_COUNT; i++) {
            if (i == choice) wattron(win, A_REVERSE);
            mvwprintw(win, 3, 2+i*(width/CONFIRM_ACTION_COUNT), "%s", confirm_action_items[i]);
            if (i == choice) wattroff(win, A_REVERSE);
        }
        wrefresh(win);

        ch = wgetch(win);
        if (ch == KEY_LEFT && choice>0) choice--;
        else if (ch == KEY_RIGHT && choice<CONFIRM_ACTION_COUNT-1) choice++;
        else if (ch == '\n' || ch == KEY_ENTER) {
            delwin(win);
            return choice;
        } else if (ch == 27) {
            delwin(win);
            return CONFIRM_CANCEL;
        }
    }
}

/* =========================================================
 * Command execution
 * ========================================================= */

static void execute_command(const char *cmd, char *status, int len)
{
    def_prog_mode();
    endwin();

    for (int i=0; i<plugin_count; i++) {
        if (strcmp(cmd, plugins[i].name)==0) {
            plugins[i].func();
            snprintf(status, len, "%s done", cmd);
            reset_prog_mode();
            return;
        }
    }
    snprintf(status, len, "Unknown command: %s", cmd);
    reset_prog_mode();
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

    int ch, selected=0;
    InputMode mode = MODE_NORMAL;
    char status[256]="";
    char input[16]="";
    int in_len=0;

    while(1) {
        draw_screen(selected, mode, status, input);

        // --- 1️⃣ Process VLC new episodes ---
        NewEpisode ne;
        while(queue_pop(&vlc_new_queue, &ne)) {
            char question[200];
            snprintf(question, sizeof(question),
                     "Add \"%s S%02dE%02d\" to clof?",
                     ne.title, ne.season, ne.episode);
            int res = confirm_popup(question);
            if(res == CONFIRM_YES) {
                add_movie(ne.title, 1, ne.season, ne.episode);
                db_save_movies();
                snprintf(status, sizeof(status),
                         "Added: %s S%02dE%02d",
                         ne.title, ne.season, ne.episode);
            } else {
                snprintf(status, sizeof(status),
                         "Skipped: %s S%02dE%02d",
                         ne.title, ne.season, ne.episode);
            }
        }

        // --- 2️⃣ User input ---
        ch = getch();

        // NUMBER MODE
        if(mode==MODE_NUMBER) {
            if((ch>='0' && ch<='9') || (ch=='-' && in_len==0)) {
                if(in_len<(int)sizeof(input)-1) {
                    input[in_len++]=ch;
                    input[in_len]=0;
                }
            } else if(ch=='\n' || ch==KEY_ENTER) {
                int value = atoi(input);
                int index = abs(value)-1;
                if(index>=0 && index<movie_count) {
                    selected=index;
                    if(value>0) next_episode(index);
                    else prev_episode(index);
                    snprintf(status,sizeof(status),"Episode updated");
                } else snprintf(status,sizeof(status),"Out of range");
                in_len=0; input[0]=0; mode=MODE_NORMAL;
            } else if(ch==27) { mode=MODE_NORMAL; in_len=0; input[0]=0; }
            continue;
        }

        // COMMAND MODE
        if(mode==MODE_COMMAND) {
            if(ch=='\n' || ch==KEY_ENTER) {
                execute_command(input,status,sizeof(status));
                in_len=0; input[0]=0; mode=MODE_NORMAL;
            } else if(ch==27) { mode=MODE_NORMAL; in_len=0; input[0]=0; }
            else if(ch==KEY_BACKSPACE || ch==127) { if(in_len>0) input[--in_len]=0; }
            else if(ch>=32 && ch<=126) { if(in_len<(int)sizeof(input)-1) input[in_len++]=ch; input[in_len]=0; }
            continue;
        }

        // NORMAL MODE
        if(ch=='q') break;
        else if(ch==KEY_UP && selected>0) selected--;
        else if(ch==KEY_DOWN && selected<movie_count-1) selected++;
        else if(ch=='n') { mode=MODE_NUMBER; in_len=0; input[0]=0; }
        else if(ch==':') { mode=MODE_COMMAND; in_len=0; input[0]=0; }
        else if(ch=='\n' || ch==KEY_ENTER) {
            if(movies[selected].is_series) {
                int a = generic_menu(series_action_items,SERIES_ACTION_COUNT);
                if(a==SERIES_ADD) { next_episode(selected); snprintf(status,sizeof(status),"Episode added"); }
                else if(a==SERIES_REMOVE) { prev_episode(selected); snprintf(status,sizeof(status),"Episode removed"); }
                else if(a==SERIES_INFO) {
                    snprintf(status,sizeof(status),
                             "Title: %s | S%02dE%02d | Watched: %s",
                             movies[selected].title,
                             movies[selected].season,
                             movies[selected].episode,
                             movies[selected].watched?"YES":"NO");
                }
            } else {
                int a = generic_menu(movie_action_items,MOVIE_ACTION_COUNT);
                if(a==MOVIE_MARK_WATCHED) { movies[selected].watched=!movies[selected].watched;
                    snprintf(status,sizeof(status),"Watched: %s",movies[selected].watched?"YES":"NO"); }
                else if(a==MOVIE_INFO) snprintf(status,sizeof(status),"%s (movie)",movies[selected].title);
            }
        }
    }

    endwin();
}
