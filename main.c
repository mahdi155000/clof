#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

#define MAX_ITEMS 5
#define MAX_STR   100
/*
typedef struct {
    int id;
    char title[MAX_STR];
    int value;
    char constant[MAX_STR];
    char comment[MAX_STR];
} Item;

Item M_L[MAX_ITEMS];
int item_count = 0;
*/ 

// Sample data
void load_defualt(void) {
    item_count = 3;
    M_L[0] = (Item){1, "Alpha", 5, "C_A", "First"};
    M_L[1] = (Item){2, "Bravo", 10, "C_B", "Second"};
    M_L[2] = (Item){3, "Charlie", 15, "C_C", "Third"};
}

void show_items() {
    printf("ID  Title     Val  Const  Comment\n");
    for (int i = 0; i < item_count; i++) {
        Item *it = &M_L[i];
        printf("%2d) %-8s %4d  %-6s  %s\n",
               it->id, it->title, it->value, it->constant, it->comment);
    }
    printf("Enter N to +1, -N to -1, Q to quit\n");
}

int main(void) {
    char buf[20];
    load_items("data.bin");
    if (item_count == 0) load_defualt();
    while (1) {
        show_items();
        printf("-> ");
        if (!fgets(buf, sizeof buf, stdin)) break;

        if (buf[0]=='q' || buf[0]=='Q') break;

        int n = atoi(buf);
        if (n == 0) {
            printf("Invalid input\n\n");
            continue;
        }

        int idx = abs(n) - 1;
        if (idx < 0 || idx >= item_count) {
            printf("Out of range\n\n");
            continue;
        }

        M_L[idx].value += (n > 0) ? 1 : -1;
        printf("\n");
    }

    printf("Bye!\n");
    save_items("data.bin");
    return 0;
}
