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

int main(int argc, char *argv[]) {
	char title[100];
	int watched;

	printf("clof started\n");

//    add_movie("Breaking bad S01E01", 0);
//    add_movie("Super Natural S01E01", 1);
	printf("Enter movie title: ");
	fgets(title, sizeof(title), stdin);
	title[strcspn(title, "\n")] = 0;

	printf("Have you watched it? (0 = No, 1 = Yes): ");
	scanf("%d" ,&watched);
	getchar();

	add_movie(title, watched);

	list_movies();

	return 0;
}

