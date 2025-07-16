#include <stdio.h>
#include <string.h>
#include "../../storage.h"
#include "../../plugins_manager.h"

// Plugin functionality
void plugin_add(Item *M_L, int *item_count) {
	if (*item_count >= MAX_ITEMS) {
		printf("cannot add item: limit reached.\n");
	}

	Item new_item = {
		.id = *item_count + 1,
		.value = 0
	};

	strcpy(new_item.title, "Added");
	strcpy(new_item.constant, "new");
	strcpy(new_item.comment, "by Plugin");

	M_L[*item_count] = new_item;
	(*item_count)++;
}

// The plugin_register function called by dlopen in main program
void plugin_register(void) {
    register_plugin("add", plugin_add);
}
