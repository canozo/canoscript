#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include "bucket.h"

typedef struct map {
    // 256 variables max
    bucket* buckets[256];
    unsigned int size;
    int error;
} map;

map* new_map();
void delete_map(map*);
void map_set(map*, char*, int);
void map_remove(map*, char*);
int map_get(map*, char*);
int map_find(map*, char*);

void map_print(map*);

#endif
