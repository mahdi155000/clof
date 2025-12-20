#include <stdio.h>
#include <string.h>
#include "plugin.h"
#include "movie.h"



extern Plugin plugins[];
extern int plugin_count;


int main(void) {
	load_movies();

    /* Start TUI as the main interface */
    plugin_tui();

    save_movies();
    return 0;
}