#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>

#define MAX_ITEMS 10

typedef struct {
    int id;
    char title[50];
    int value;
    char constant[50];
    char comment[50];
} Item;

extern Item M_L[MAX_ITEMS];
extern int item_count;

void save_items(const char *filename);
void load_items(const char *filename);

#endif // STORAGE_H
