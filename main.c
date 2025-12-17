#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { 
	char title[100];
	int watched;	// 0 for no and 1 for yes
	int season;
	int episode;
} Movie;

#define MAX_MOVIES 100
Movie movies[MAX_MOVIES];
int movie_count =0;


void add_movie(const char *title, int season, int episdoe, int watched) {
	if (movie_count < MAX_MOVIES) {
		strcpy(movies[movie_count].title, title);
		movies[movie_count].season = season;
		movies[movie_count].episode = episdoe;
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

	for (int i = 0; i < movie_count; i++){
		if (movies[i].season > 0) {
			printf("%d: %s S%02dE%02d (watched: %d)\n",
			i + 1, movies[i].title,
			movies[i].season, movies[i].episode,
			movies[i].watched);
		} else {
			printf("%d: %s (watched: %d)\n",
			i + 1, movies[i].title, movies[i].watched);
		}
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

int movie_exists(const char *title, int season, int episode) {
	for (int i = 0; i < movie_count; i++) {
		if (strcmp(movies[i].title, title) == 0 && 
			movies[i].season == season &&
			movies[i].episode == episode) {
			return 1; // movie found
		}
	}
	return 0; // movie not found
}

// Update the watched status of a movie
void update_watched(const char *title, int season, int episode, int watched){
	for (int i = 0; i < movie_count; i++){
		if (strcmp(movies[i].title, title) == 0 &&
			movies[i].season == season &&
			movies[i].episode == episode) {

			movies[i].watched = watched;
			printf("Updated '%s' S%02dE%02d ->  watched = %d\n", title, season, episode, watched);
			return; // stop searching after update
		}
	}
	printf("Movie '%s' not found\n", title); 
}

void show_menu() {
	printf("\n===== LOF Menu =====\n");
    printf("1. Add movie / episode\n");
    printf("2. List movies\n");
    printf("3. Update watched status\n");
    printf("4. Save and exit\n");
    printf("====================\n");
    printf("Choose an option: ");
}


void add_movie_interactive() {
	char title[100];
	int is_series;
	int season = 0;
	int episode = 0;
	int watched;

	printf("Enter title: ");
	fgets(title, sizeof(title), stdin);
	title[strcspn(title, "\n")] = 0;

	printf("Is this a series? (0 = No ,1 = Yes): ");
	scanf("%d", &is_series);
	getchar(); // consume newline

	if ( is_series == 1) {
		printf("Enter season number: ");
		scanf("%d", &season);

		printf("Enter episode number: ");
		scanf("%d", &episode);
		getchar();
	}

	if (movie_exists(title,season,episode)) {
		printf("This movie/episode is already exists!\n");
		return;
	}

	printf("Have you watched it? (0 = No, 1 = Yes)");
	scanf("%d", &watched);
	getchar();

	add_movie(title,season,episode,watched);
	printf("Movie added successfuly.\n");
}

void update_movie_interactive() {
	char title[100];
	int season;
	int episode;
	int watched;

	printf("Enter title to update: ");
	fgets(title, sizeof(title), stdin);
	title[strcspn(title, "\n")] = 0;

	printf("Enter season number (0 if movie): ");
	scanf("%d", &season);

	printf("Enter episode number (0 if movie): ");
	scanf("%d", &episode);
	getchar();

	if (!movie_exists(title,season,episode)) {
		printf("Movie/episode not found!\n");
		return;
	}

	printf("Enter new watched status (0 = No, 1 = Yes): ");
	scanf("%d", &watched);
	getchar();


	update_watched(title,season,episode,watched);


}

int main(int argc, char *argv[]) {
	int choice;

	printf("Welcome to clof\n");

	load_movies();

	while (1) {
		show_menu();

		scanf("%d", &choice);
		getchar(); // consume newline

		if (choice == 1) {
			add_movie_interactive();
		}
		else if ( choice == 2 ) {
			list_movies();
		}
		else if ( choice == 3 ) {
			update_movie_interactive();
		}
		else if ( choice == 4 ) {
			save_movies();
			printf("Goodbye!\n");
			break;
		}
		else {
			printf("Invalid option!\n");
		}
	}
	return 0;
}