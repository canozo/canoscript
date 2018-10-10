#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

typedef struct vec {
    void** data;
    unsigned int size;
    unsigned int capacity;
} vec;

vec* new_vec();
void delete_vec(vec*);
vec* vec_push(vec*, void*);
vec* vec_resize(vec*);
void* vec_get(vec*, unsigned int);
void* vec_remove(vec*, unsigned int);
void* vec_pop(vec*);
void vec_set(vec*, unsigned int, void*);

#endif
