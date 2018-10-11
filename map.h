#ifndef MAP_H
#define MAP_H

#include "bucket.h"

typedef struct map {
    // 256 variables max
    bucket* buckets[256];
    int size;
    int error;
} map;

map* new_map();
void delete_map(map*);
void map_set_integer(map*, char*, int);
void map_set_real_num(map*, char*, float);
void map_set_string(map*, char*, char*);
void map_remove(map*, char*);
int map_get(map*, char*);
int map_find(map*, char*);

void map_print(map*);

#endif
