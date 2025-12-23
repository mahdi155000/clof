#include <stdio.h>
#include <string.h>
#include "../movie.h"

void plugin_add(void) {
    char title[TITLE_LEN];
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
    if (scanf("%d", &is_series) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar(); /* consume newline */

    if (is_series) {
        printf("Season: ");
        scanf("%d", &season);
        printf("Episode: ");
        scanf("%d", &episode);
        getchar(); /* consume newline */
    }

    add_movie(title, is_series, season, episode);

    /* NEW: default watched state */
    movies[movie_count - 1].watched = 0;

    save_movies();

    printf("Added.\n");
}
