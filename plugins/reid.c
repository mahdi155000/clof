#include <stdio.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"

/* Reposition (reid) plugin */
void plugin_reid(void) {
    if (movie_count < 2) {
        printf("Not enough movies to reposition.\n");
        return;
    }

    char tmp[16];
    int from = -1, to = -1;

    printf("Move movie at position #: ");
    if (!fgets(tmp, sizeof(tmp), stdin)) return;
    from = atoi(tmp) - 1;

    if (from < 0 || from >= movie_count) {
        printf("Invalid position.\n");
        return;
    }

    printf("Move to position #: ");
    if (!fgets(tmp, sizeof(tmp), stdin)) return;
    to = atoi(tmp) - 1;

    if (to < 0 || to >= movie_count) {
        printf("Invalid position.\n");
        return;
    }

    if (from == to) {
        printf("Already at that position.\n");
        return;
    }

    /* Move movie in the array */
    Movie temp = movies[from];

    if (from < to) {
        for (int i = from; i < to; i++)
            movies[i] = movies[i + 1];
    } else {
        for (int i = from; i > to; i--)
            movies[i] = movies[i - 1];
    }

    movies[to] = temp;

    printf("Movie repositioned from %d to %d.\n", from + 1, to + 1);
}


// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("reid", plugin_reid);
}