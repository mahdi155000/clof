#include <stdio.h>
#include <string.h>
#include "plugin.h"
#include "movie.h"

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

		if (strcspn(cmd, "quit") == 0)
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