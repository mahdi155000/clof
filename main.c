#include <stdio.h>
#include <string.h>
#include "plugin.h"
#include "movie.h"



void load_movies(void) {
    FILE *fp = fopen("movies.dat", "rb");

    if (fp == NULL) {
        /* file does not exist yet — this is NOT an error */
        movie_count = 0;
        return;
    }

    if (fread(&movie_count, sizeof(int), 1, fp) != 1) {
        movie_count = 0;
        fclose(fp);
        return;
    }

    if (movie_count > MAX_MOVIES) {
        movie_count = 0;
        fclose(fp);
        return;
    }

    fread(movies, sizeof(Movie), movie_count, fp);
    fclose(fp);
}

void save_movies(void) {
    FILE *fp = fopen("movies.dat", "wb");

    if (fp == NULL) {
        perror("save_movies");
        return;
    }

    fwrite(&movie_count, sizeof(int), 1, fp);
    fwrite(movies, sizeof(Movie), movie_count, fp);

    fclose(fp);
}


extern Plugin plugins[];
extern int plugin_count;


int main(void) {
	char cmd[32];

	load_movies();

	while (1) {
		printf("\nclof> ");

		if (!fgets(cmd, sizeof(cmd), stdin))
			break;
		
		cmd[strcspn(cmd, "\n")]  = 0;

		if (strcmp(cmd, "quit") == 0)
			break;

		int found = 0;

		for (int i = 0; i <plugin_count; i++) {
			if (strcmp(cmd, plugins[i].name) == 0){
				plugins[i].run();
				found = 1;
				break;
			}
		}

		if (!found) {
			printf("Unknown command. Type 'help'.\n");
		}
	}

	save_movies();
	return 0;
}