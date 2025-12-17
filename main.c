#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { 
	char title[100];
	int watched;	// 0 for no and 1 for yes
} Movie;

#define MAX_MOVIES 100
Movie movies[MAX_MOVIES];
int movie_count =0;


void add_movie(const char *title, int watched) {
	if (movie_count < MAX_MOVIES) {
		strcpy(movies[movie_count].title, title);
		movies[movie_count].watched = watched;
		movie_count++;
	} else {
		printf("Movie database full!\n");
	}
}


void list_movies(){
	if (movie_count == 0) {
		printf("Ther is no movies in database.\n");
		return;
	}

	for (int i = 0;i  < movie_count; i++){
		printf("%d: %s (watched: %d)\n", i+1, movies[i].title, movies[i].watched);
	}

}


void save_movies(){
	FILE *file = fopen("movies.dat", "wb");
	if (file != NULL) {
		fwrite(&movie_count, sizeof(int), 1, file);
		fwrite(movies, sizeof(Movie), movie_count, file);
		fclose(file);
		printf("Movies saved to file.\n");
	} else
	{
		printf("Error saving movies.\n");
	}
	
}


void load_movies() {
	FILE *file = fopen("movies.dat", "rb");
	if (file != NULL) {
		fread(&movie_count, sizeof(int), 1, file);
		fread(movies, sizeof(Movie), movie_count, file);
		fclose(file);
		printf("Movies loaded from file.\n");
	} else {
		printf("No saved movies found.\n");
	}
}

int movie_exists(const char *title) {
	for (int i = 0; i < movie_count; i++) {
		if (strcmp(movies[i].title, title) == 0) {
			return 1; // movie found
		}
	}
	return 0; // movie not found
}

int main(int argc, char *argv[]) {
	char title[100];
	int watched;

	printf("clof started\n");

	load_movies();

//    add_movie("Breaking bad S01E01", 0);
//    add_movie("Super Natural S01E01", 1);
	printf("Enter movie title: ");
	fgets(title, sizeof(title), stdin);
	title[strcspn(title, "\n")] = 0;

	if (movie_exists(title)) {
		printf("Movie '%s' already exists!\n", title);
	} else {

		printf("Have you watched it? (0 = No, 1 = Yes): ");
		scanf("%d" ,&watched);
		getchar();

		add_movie(title, watched);
	}

	list_movies();

	save_movies();

	return 0;
}

