#include "plugins/tui.h"
#include "plugin.h"
#include "movie.h"
#include "db.h"

extern PluginEntry plugins[];
extern int plugin_count;

int main(void) {
    db_init();
    db_load_movies();

    plugin_tui();

    db_save_movies();
    db_close();
    return 0;
}