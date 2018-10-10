#ifndef VEC_H
#define VEC_H

typedef struct vec {
    void** data;
    unsigned int size;
    unsigned int capacity;
} vec;

vec* new_vec();
void delete_vec(vec*);
void vec_push(vec*, void*);
void vec_resize(vec*);
void* vec_get(vec*, unsigned int);
void* vec_remove(vec*, unsigned int);
void* vec_pop(vec*);
void vec_set(vec*, unsigned int, void*);

#endif
