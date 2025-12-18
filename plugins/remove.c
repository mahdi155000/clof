#include <stdio.h>
#include <stdlib.h>
#include "../movie.h"

void plugin_remove(void) {
    int num;
    int index;

    printf("Remove entry number: ");
    if(scanf("%d", &num) != 1) {
        getchar();
        return;
    }
    getchar();

    index =num -1;

    if (index < 0 || index >= movie_count) {
        printf("Invalid index.\n");
        return;
    }

    printf("Removed: %s\n", movies[index].title);

    remove_movie(index);
    save_movies();
}