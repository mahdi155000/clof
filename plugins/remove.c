#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../movie.h"
#include "../db.h"
#include "../plugin.h"

void plugin_remove(void)
{
    int num;
    int index;

    printf("Remove entry number: ");
    if (scanf("%d", &num) != 1) {
        getchar();
        return;
    }
    getchar(); // consume newline

    index = num - 1;

    if (index < 0 || index >= movie_count) {
        printf("Invalid index.\n");
        return;
    }

    printf("Removed: %s\n", movies[index].title);

    /* shift movies left */
    for (int i = index; i < movie_count - 1; i++) {
        movies[i] = movies[i + 1];
    }

    movie_count--;

    /* persist change */
    db_save_movies();
}

/* register automatically */
__attribute__((constructor))
static void register_me(void)
{
    register_plugin("remove", plugin_remove);
}
