#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { 
	char title[100];
	int watched;	// 0 for no and 1 for yes
} Movie;

int main(int argc, char *argv[]) {
    printf("clof started\n");
	Movie myMovie;

	strcpy(myMovie.title, "Breaking Bad S01E01");

	myMovie.watched = 0;

	printf("Movie: %s\n", myMovie.title);
	printf("Watched: %d\n", myMovie.watched);

    return 0;
}

