#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

static sqlite3 *db = NULL;

/* ------------------ init ------------------ */
int db_init(void) {
    int rc = sqlite3_open("movies.db", &db);
    if (rc) {
        fprintf(stderr, "DB open error: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS movies ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT UNIQUE,"
        "genre TEXT,"
        "is_series INTEGER,"
        "season INTEGER,"
        "episode INTEGER,"
        "watched INTEGER DEFAULT 0"
        ");";

    char *err = NULL;
    rc = sqlite3_exec(db, sql, 0, 0, &err);
    if (err) {
        fprintf(stderr, "SQL error: %s\n", err);
        sqlite3_free(err);
    }

    return rc;
}

/* ------------------ load ------------------ */
void db_load_movies(void) {
    movie_count = 0;

    const char *sql =
        "SELECT title, genre, is_series, season, episode, watched FROM movies;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return;

    while (sqlite3_step(stmt) == SQLITE_ROW && movie_count < MAX_MOVIES) {
        Movie *m = &movies[movie_count++];

        strncpy(m->title, (char*)sqlite3_column_text(stmt, 0), TITLE_LEN);
        strncpy(m->genre, (char*)sqlite3_column_text(stmt, 1), GENRE_LEN);
        m->is_series = sqlite3_column_int(stmt, 2);
        m->season    = sqlite3_column_int(stmt, 3);
        m->episode   = sqlite3_column_int(stmt, 4);
        m->watched   = sqlite3_column_int(stmt, 5);
    }

    sqlite3_finalize(stmt);
}

/* ------------------ save ------------------ */
void db_save_movies(void) {
    sqlite3_exec(db, "DELETE FROM movies;", 0, 0, NULL);

    const char *sql =
        "INSERT INTO movies (title, genre, is_series, season, episode, watched)"
        "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    for (int i = 0; i < movie_count; i++) {
        sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        sqlite3_bind_text(stmt, 1, movies[i].title, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, movies[i].genre, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, movies[i].is_series);
        sqlite3_bind_int(stmt, 4, movies[i].season);
        sqlite3_bind_int(stmt, 5, movies[i].episode);
        sqlite3_bind_int(stmt, 6, movies[i].watched);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

/* ------------------ delete ------------------ */
void db_delete_movie(const char *title) {
    const char *sql = "DELETE FROM movies WHERE title = ?;";
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

/* ------------------ close ------------------ */
void db_close(void) {
    if (db) sqlite3_close(db);
}
