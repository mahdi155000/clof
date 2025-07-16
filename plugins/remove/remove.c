#include <stdio.h>
#include "../../storage.h"
#include "../../plugins_manager.h"

void plugin_remove(Item *M_L, int *item_count) {
	if (*item_count == 0) {
		printf("No item to remove.\n");
		return;
	}

	int index;
	printf("Enter item number to remove (1 to %d): ", *item_count);
	if (scanf("%d", &index) != 1 || index < 1 || index > *item_count) {
		while (getchar() != '\n');
		return;
	}

	// Convert 1-bed to 0based
	index --;

	// Shift item left
	for (int i = index; i < *item_count -1; i++){
		M_L[i] = M_L[i + 1];
	}

	(*item_count)--;

	printf("Item %d removed.\n", index + 1);

	while (getchar() != '\n');
}

void plugin_register(void) {
	register_plugin("remove", plugin_remove);
}
