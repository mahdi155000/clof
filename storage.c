#include "storage.h"

Item M_L[MAX_ITEMS];
int item_count = 0;

void save_items(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fwrite(&item_count, sizeof item_count, 1, f);
    fwrite(M_L, sizeof(Item), item_count, f);
    fclose(f);
}

void load_items(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return;
    fread(&item_count, sizeof item_count, 1, f);
    if (item_count > MAX_ITEMS) item_count = MAX_ITEMS;
    fread(M_L, sizeof(Item), item_count, f);
    fclose(f);
}
