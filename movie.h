#ifndef MOVIE_H
#define MOVIE_H

#include <sqlite3.h>   

//  ----- constants -------
#define MAX_MOVIES 1000
#define TITLE_LEN 100
#define GENRE_LEN 50

extern sqlite3 *db;

//  ----- data structures ------
typedef struct
{
    char title[TITLE_LEN];
    char genre[GENRE_LEN];

    int is_series;
    int season;
    int episode;
    int watched;
}Movie;

// ---- global storage ---- 
extern Movie movies[MAX_MOVIES];
extern int movie_count;

//  ---- core functions ----

void load_movies(void);
void save_movies(void);


int movie_exists(const char *title);  // return index or -1
void add_movie(const char *title, int is_series, int season, int episode);
void remove_movie(int index);

// progress controll
void next_episode(int index);
void prev_episode(int index);

// genre management
void set_movie_genre(int index, const char *genre);

// Add function prototypes for DB
int init_db(void);
void close_db(void);

#endif