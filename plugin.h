#ifndef PLUGIN_H
#define PLUGIN_H

/*
This is pointer to a function
Function takes no arguments
function returns nothing
*/
typedef void (*plugin_func_t)(void);

// One command = one plugin

typedef struct 
{
    const char *name;   // command name
    const char *help;   // help text
    plugin_func_t run;  // function to execute
}Plugin;

#endif