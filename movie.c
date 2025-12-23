#include "movie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Movie movies[MAX_MOVIES];
int movie_count = 0;

/* ---- persistence ---- */


int movie_exists(const char *title) {
    for (int i = 0; i < movie_count; i++) {
        if (strcmp(movies[i].title, title) == 0)
            return i;
    }
    return -1;
}
/* ---- episode control ---- */

void next_episode(int index) {
    if (index < 0 || index >= movie_count)
        return;

    if (!movies[index].is_series)
        return;

    movies[index].episode++;
}

void prev_episode(int index) {
    if (index < 0 || index >= movie_count)
        return;

    if (!movies[index].is_series)
        return;

    if (movies[index].episode > 1)
        movies[index].episode--;
}

void add_movie(const char *title, int is_series, int season, int episode) {
    if (movie_count >= MAX_MOVIES)
        return;

    strncpy(movies[movie_count].title, title, TITLE_LEN - 1);
    movies[movie_count].title[TITLE_LEN - 1] = '\0';

    strcpy(movies[movie_count].genre, "");

    movies[movie_count].is_series = is_series;
    movies[movie_count].season = season;
    movies[movie_count].episode = episode;
    movies[movie_count].watched = 0;

    movie_count++;
}