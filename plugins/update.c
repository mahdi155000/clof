#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"

void plugin_update(WINDOW *win) {
    char input_str[16];
    int input;
    int index;

    // Prompt for entry number
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Entry number (+N / -N): ");
    wrefresh(win);
    wgetnstr(win, input_str, sizeof(input_str) - 1);
    input = atoi(input_str);

    index = abs(input) - 1;

    if (index < 0 || index >= movie_count) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "Invalid entry number.");
        mvwprintw(win, 3, 2, "Press any key to continue...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    if (!movies[index].is_series) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "'%s' is not a series.", movies[index].title);
        mvwprintw(win, 3, 2, "Press any key to continue...");
        wrefresh(win);
        wgetch(win);
        return;
    }

    // Update episode
    if (input > 0)
        next_episode(index);
    else
        prev_episode(index);

    // Show result
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "Updated: %s S%02dE%02d",
              movies[index].title,
              movies[index].season,
              movies[index].episode);
    mvwprintw(win, 3, 2, "Press any key to continue...");
    wrefresh(win);
    wgetch(win);
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("update", plugin_update);
}