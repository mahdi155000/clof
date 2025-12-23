#include "movie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>


sqlite3 *db = NULL;  // global DB handle

/* ---- storage ---- */

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

void add_movie(const char *title, int is_series, int season, int episode) {
    if (movie_count >= MAX_MOVIES)
        return;

    strncpy(movies[movie_count].title, title, TITLE_LEN - 1);
    movies[movie_count].title[TITLE_LEN - 1] = '\0';

    strcpy(movies[movie_count].genre, "");

    movies[movie_count].is_series = is_series;
    movies[movie_count].season = season;
    movies[movie_count].episode = episode;

    movie_count++;
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

/* ------------------- Database Initialization ------------------- */
int init_db(void) {
    int rc = sqlite3_open("movies.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    const char *sql_create = 
        "CREATE TABLE IF NOT EXISTS movies ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL UNIQUE,"
        "genre TEXT,"
        "is_series INTEGER,"
        "season INTEGER,"
        "episode INTEGER);";

    char *errmsg = NULL;
    rc = sqlite3_exec(db, sql_create, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return rc;
    }

    return SQLITE_OK;
}

/* ------------------- Load Movies ------------------- */
void load_movies(void) {
    movie_count = 0;

    const char *sql = "SELECT title, genre, is_series, season, episode FROM movies;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
        return;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        strncpy(movies[movie_count].title, (const char*)sqlite3_column_text(stmt, 0), TITLE_LEN - 1);
        movies[movie_count].title[TITLE_LEN - 1] = '\0';

        strncpy(movies[movie_count].genre, (const char*)sqlite3_column_text(stmt, 1), GENRE_LEN - 1);
        movies[movie_count].genre[GENRE_LEN - 1] = '\0';

        movies[movie_count].is_series = sqlite3_column_int(stmt, 2);
        movies[movie_count].season    = sqlite3_column_int(stmt, 3);
        movies[movie_count].episode   = sqlite3_column_int(stmt, 4);

        movie_count++;
        if (movie_count >= MAX_MOVIES)
            break;
    }

    sqlite3_finalize(stmt);
}

/* ------------------- Save Movies ------------------- */
void save_movies(void) {
    for (int i = 0; i < movie_count; i++) {
        const char *sql_insert =
            "INSERT INTO movies (title, genre, is_series, season, episode) "
            "VALUES (?, ?, ?, ?, ?) "
            "ON CONFLICT(title) DO UPDATE SET "
            "genre=excluded.genre, is_series=excluded.is_series, "
            "season=excluded.season, episode=excluded.episode;";

        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0) != SQLITE_OK)
            continue;

        sqlite3_bind_text(stmt, 1, movies[i].title, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, movies[i].genre, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, movies[i].is_series);
        sqlite3_bind_int(stmt, 4, movies[i].season);
        sqlite3_bind_int(stmt, 5, movies[i].episode);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

/* ------------------- Remove Movie ------------------- */
void remove_movie(int index) {
    if (index < 0 || index >= movie_count) return;

    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM movies WHERE title='%s';", movies[index].title);

    char *errmsg = NULL;
    sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (errmsg) sqlite3_free(errmsg);

    // Remove from local array
    for (int i = index; i < movie_count - 1; i++)
        movies[i] = movies[i + 1];

    movie_count--;
}

/* ------------------- Close DB ------------------- */
void close_db(void) {
    if (db) sqlite3_close(db);
}
