#ifndef STORAGE_H
#define STORAGE_H

#define MAX_ITEMS 100
#define MAX_STR 100

typedef struct {
    int id;
    char title[MAX_STR];
    int value;
    char constant[MAX_STR];
    char comment[MAX_STR];
} Item;

extern Item M_L[MAX_ITEMS];      // <--- declare, not define
extern int item_count;           // <--- declare, not define

void save_items(const char *filename);
void load_items(const char *filename);

#endif

