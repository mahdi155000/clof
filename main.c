#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { 
	char title[100];
	int is_series;	// 0 = movie, 1 = series
	int season;
	int episode;
	int watched;	// 0 for no and 1 for yes
} Movie;

#define MAX_MOVIES 100
Movie movies[MAX_MOVIES];
int movie_count =0;


void add_movie(const char *title, int is_series, int season, int episode, int watched) {
	if (movie_count >= MAX_MOVIES) {
		printf("Database full!\n");
		return;
	}
	strcpy(movies[movie_count].title, title);
	movies[movie_count].is_series = is_series;
	movies[movie_count].season = season;
	movies[movie_count].episode = episode;
	movies[movie_count].watched = watched;
	movie_count++;
}

void mark_watched(int index) {
	if (movies[index].is_series) {
		movies[index].episode += 1;
		movies[index].watched = 0;

		printf("Nedxt episode: %s S%02dE%02d\n",
			movies[index].title,
			movies[index].season,
			movies[index].episode);
	} else {
		movies[index].watched = 1;
		printf("Movie '%s' marked as watched.\n", movies[index].title);
	}
}


void list_movies(){
	printf("\n====== LOF =======\n");

	for (int i =0; i < movie_count; i++){
		if (movies[i].is_series) {
			printf("%2d) %-25s	S%02dE%02d\n", i + 1,
				movies[i].title,
				movies[i].season,
				movies[i].episode);
		} else {
			printf("%2d) %-25s	(movie)\n", i + 1, movies[i].title);
		}
	}
	printf("=====================\n");
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
		if (strcmp(movies[i].title, title) == 0 ) {
			return i; // movie found
		}
	}
	return -1; // movie not found
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

    printf("Enter title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    if (movie_exists(title) != -1) {
        printf("Title already exists.\n");
        return;
    }

    printf("Is this a series? (0 = No, 1 = Yes): ");
    scanf("%d", &is_series);
    getchar();

    if (is_series == 1) {
        printf("Enter starting season: ");
        scanf("%d", &season);

        printf("Enter starting episode: ");
        scanf("%d", &episode);
        getchar();
    }

    add_movie(title, is_series, season, episode, 0);
    printf("Title added successfully.\n");
}

void update_movie_interactive() {
	char title[100];
	int index;

	printf("Enter title to update: ");
	fgets(title, sizeof(title), stdin);
	title[strcspn(title, "\n")] = 0;

	index = movie_exists(title);

	if (index == -1){
		printf("Title not found.\n");
		return;
	}

	mark_watched(index);


}

void apply_lof_input(int input) {
	int index = abs(input) -1;

	if (index < 0 || index >= movie_count) {
		printf("Invalid input.\n");
		return;
	}

	if (!movies[index].is_series) {
		printf("'%s' is a movie not a series.\n", movies[index].title);
		return;
	}

	if (input > 0) {
		// plus -> next episode
		movies[index].episode++;
		printf("-> %s S%02dE%02d\n",
			movies[index].title,
			movies[index].season,
			movies[index].episode);
	} else {
		printf("Already at first episode.\n");
	}
}

int main() {
	int input;

	// printf("Welcome to clof\n");

	load_movies();

	while (1) {
		// show_menu();
		list_movies();

		printf("Input (+N / -N / 0 / q): ");

		if (scanf("%d", &input) != 1) {
			break;	// non-number (q)
		}

		getchar();

		if (input == 0) {
			continue;
		}

		apply_lof_input(input);
		save_movies();
	}
	printf("goodbye.\n");
	return 0;
}