#include <stdio.h>
#include "../../plugins_manager.h"

void plugin_add(void){
	printf("Add plugin executed!\n");
}


void plugin_register(void) {
	register_plugin("add", plugin_add);
}
