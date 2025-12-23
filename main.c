#include "plugin.h"
#include "movie.h"

extern Plugin plugins[];
extern int plugin_count;

int main(void) {
    if (init_db() != SQLITE_OK)
        return 1;  // exit if DB failed

    load_movies();

    plugin_tui();  // TUI interface

    save_movies();
    close_db();
    return 0;
}
