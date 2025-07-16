#include <stdio.h>
#include <string.h>
#include "../../storage.h"
#include "../../plugins_manager.h"

// Plugin functionality
void plugin_add(Item *M_L, int *item_count) {
	if (*item_count >= MAX_ITEMS) {
		printf("cannot add item: limit reached.\n");
	}

	Item new_item;
	new_item.id = *item_count + 1;

	printf("Enter title: ");
    fgets(new_item.title, MAX_STR, stdin);
    new_item.title[strcspn(new_item.title, "\n")] = 0;

	printf("Enter value: ");
    scanf("%d", &new_item.value);
    getchar();

	printf("Enter constant: ");
    fgets(new_item.constant, MAX_STR, stdin);
    new_item.constant[strcspn(new_item.constant, "\n")] = 0;
 
	printf("Enter comment: ");
    fgets(new_item.comment, MAX_STR, stdin);
    new_item.comment[strcspn(new_item.comment, "\n")] = 0;

	M_L[*item_count] = new_item;
	(*item_count)++;
}

// The plugin_register function called by dlopen in main program
void plugin_register(void) {
    register_plugin("add", plugin_add);
}
