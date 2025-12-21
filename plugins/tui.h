#ifndef TUI_H
#define TUI_H

#include <ncurses.h>

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

extern const char *series_action_items[SERIES_ACTION_COUNT];
extern const char *movie_action_items[MOVIE_ACTION_COUNT];

#endif /* TUI_H */