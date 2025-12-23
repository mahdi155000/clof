#include <stdio.h>
#include <stdlib.h>
#include "../movie.h"

void plugin_update(void) {
    int input;
    int index;

    printf("Entry number (+n / -N): ");
    if (scanf("%d", &input) != 1) {
        getchar();
        return;
    }

    getchar();

    index = abs(input) - 1;

    if (scanf("%d", &input) != 1) {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n');  // Clear stdin
            return;
}
    if (!movies[index].is_series) {
        printf("Not a series.\n");
        return;
    }

    if (input > 0 ) 
        next_episode(index);
    else
        prev_episode(index);
    

    printf("Updated: %s S%02dE%02d\n",
            movies[index].title,
                movies[index].season,
            movies[index].episode);
}