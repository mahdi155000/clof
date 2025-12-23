#include <stdio.h>
#include <string.h>
#include <stdlib.h>   // added for atoi
#include "../movie.h"
#include "../plugin.h"

void plugin_add(void) {
    char title[TITLE_LEN];
    char tmp[16];
    int is_series = 0;
    int season = 0;
    int episode = 0;

    printf("Title: ");
    if (!fgets(title, sizeof(title), stdin))
        return;

    title[strcspn(title, "\n")] = '\0';

    if (title[0] == '\0') {
        printf("Empty title.\n");
        return;
    }

    if (movie_exists(title) != -1) {
        printf("Already exists.\n");
        return;
    }

    printf("Is series? (0 = No, 1 = Yes): ");
    if (!fgets(tmp, sizeof(tmp), stdin))
        return;
    is_series = atoi(tmp);

    if (is_series) {
        printf("Season: ");
        if (!fgets(tmp, sizeof(tmp), stdin))
            return;
        season = atoi(tmp);

        printf("Episode: ");
        if (!fgets(tmp, sizeof(tmp), stdin))
            return;
        episode = atoi(tmp);
    }

    add_movie(title, is_series, season, episode);
    movies[movie_count - 1].watched = 0;

    printf("Added.\n");
}
// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("add", plugin_add);
}