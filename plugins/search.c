#include <stdio.h>
#include <string.h>
#include "../movie.h"

void plugin_search(void) {
    char query[TITLE_LEN];
    int found = 0;

    printf("Search query: ");
    if (!fgets(query, sizeof(query), stdin))
        return;

    query[strcspn(query, "\n")] = 0;

    printf("\n==== Search Results ====\n");

    for (int i = 0; i < movie_count; i++) {
        if (strstr(movies[i].title, query)) {
            if (movies[i].is_series) {
                printf("%2d) %-25s S%02dE%02d\n", i + 1,
                    movies[i].title,
                    movies[i].season,
                    movies[i].episode);
            } else {
                printf("%2d) %-25s (movie)\n", i + 1, movies[i].title);
            }
            found = 1;
        }
    }

    if (!found) {
        printf("No matches found.\n");
    }
}