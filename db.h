#ifndef DB_H
#define DB_H

#include "movie.h"

/* Database lifecycle */
int  db_init(void);
void db_close(void);

/* Persistence */
void db_load_movies(void);
void db_save_movies(void);

#endif
