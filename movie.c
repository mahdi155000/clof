#include <stdio.h>
#include <string.h>
#include "movie.h"

/* ---- storage ---- */

Movie movies[MAX_MOVIES];
int movie_count = 0;

/* ---- persistence ---- */

void load_movies(void) {
    FILE *f = fopen("movies.db", "rb");
    if (!f)
        return;

    fread(&movie_count, sizeof(int), 1, f);
    fread(movies, sizeof(Movie), movie_count, f);
    fclose(f);
}

void save_movies(void) {
    FILE *f = fopen("movies.db", "wb");
    if (!f)
        return;

    fwrite(&movie_count, sizeof(int), 1, f);
    fwrite(movies, sizeof(Movie), movie_count, f);
    fclose(f);
}

/* ---- core logic ---- */

int movie_exists(const char *title) {
    for (int i = 0; i < movie_count; i++) {
        if (strcmp(movies[i].title, title) == 0)
            return i;
    }
    return -1;
}

void add_movie(const char *title, int is_series, int season, int episode) {
    if (movie_count >= MAX_MOVIES)
        return;

    strncpy(movies[movie_count].title, title, TITLE_LEN - 1);
    movies[movie_count].title[TITLE_LEN - 1] = '\0';

    movies[movie_count].is_series = is_series;
    movies[movie_count].season = season;
    movies[movie_count].episode = episode;

    movie_count++;
}

void remove_movie(int index) {
    if (index < 0 || index >= movie_count)
        return;

    for (int i = index; i < movie_count - 1; i++) {
        movies[i] = movies[i + 1];
    }

    movie_count--;
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
