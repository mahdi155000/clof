#include <stdio.h>
#include <string.h>
#include "../movie.h"

void plugin_add(void){ 
    char title[TITLE_LEN];
    int is_series;
    int season = 0;
    int episode = 0;

    printf("Title: ");
    if (!fgets(title,sizeof(title), stdin))
        return;

    title[strcspn(title, "\n")] = 0;

    if (movie_exists(title != -1)){
        printf("Already exists.\n");
        return;
    }

    printf("Is series? ( 0 = No, 1 = Yes )");
    scanf("%d", &is_series);

    if (is_series){
        printf("Season: ");
        scanf("%d", &season);
        printf("Episode: ");
        scanf("%d", &episode);
        getchar();
        
    }

    add_movie(title,is_series, season, episode);
    save_movies();

    printf("Added.\n")
    
}