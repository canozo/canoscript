#ifndef MAP_H
#define MAP_H

#include "bucket.h"

#define MAX_VARIABLES 256

typedef struct map {
    bucket* buckets[MAX_VARIABLES];
    int size;
    int error;
} map;

map* new_map();
void delete_map(map*);
void map_set_integer(map*, char*, int);
void map_set_float(map*, char*, float);
void map_set_string(map*, char*, char*);
void map_remove(map*, char*);
bucket* map_get(map*, char*);
int map_find(map*, char*);

void map_print(map*);

#endif
