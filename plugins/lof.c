#include <stdio.h>
#include <stdlib.h>
#include "../movie.h"
#include "../plugin.h"

static void print_lof_list(void)
{
    printf("\n===== LOF =====\n");

    for (int i = 0; i < movie_count; i++) {
        if (movies[i].is_series) {
            printf("%2d) %-20s S%02dE%02d\n",
                   i + 1,
                   movies[i].title,
                   movies[i].season,
                   movies[i].episode);
        } else {
            printf("%2d) %-20s (movie)\n",
                   i + 1,
                   movies[i].title);
        }
    }

    printf("================\n");
}
void plugin_lof(void) {
    int input;

    while (1) {
        print_lof_list();

        printf("Input (+N / -N / 0 / q): ");
        if (scanf("%d", &input) != 1)
        {
            getchar();
            break;
        }

        getchar();

        if (input == 0) {
            continue;
        }

        int index = abs(input) - 1;

        if (index < 0 || index >= movie_count) {
            printf("Invalid entry.\n");
            continue;
        }

        if (!movies[index].is_series) {
            printf("'%s' is a movie.\n", movies[index].title);
            continue;
        }

        if (input > 0)
            next_episode(index);
        else
            prev_episode(index);

    }
}

// register automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("lof", plugin_lof);
}