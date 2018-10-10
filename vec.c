#include "vec.h"
#include <stdio.h>

vec* new_vec(size_t data_size) {
    vec* this = malloc(sizeof(*this));

    this->data_size = data_size;
    this->size = 0;
    this->capacity = 2;
    this->data = malloc(this->data_size * this->capacity);

    return this;
}

void delete_vec(vec* this) {
    printf("DEBUG vector pointer: %p\n", this);
    // TODO error here
    free(this->data);
    free(this);
}

vec* vec_push(vec* this, void* data) {
    if (this->size+1 > this->capacity) {
        this = vec_resize(this);
    }

    this->data[this->size] = data;
    this->size += 1;

    return this;
}

vec* vec_resize(vec* this) {
    this->capacity *= 2;
    this = realloc(this, this->data_size * this->capacity);
    return this;
}

void* vec_get(vec* this, unsigned int pos) {
    if (pos >= this->size) {
        return NULL;
    } else {
        return this->data[pos];
    }
}

void* vec_remove(vec* this, unsigned int pos) {
    if (pos >= this->size) {
        return NULL;
    }

    void* result = this->data[pos];

    for (unsigned int i = pos+1; i < this->size; i++) {
        this->data[i-1] = this->data[i];
    }

    return result;
}

void* vec_pop(vec* this) {
    if (this->size == 0) {
        return NULL;
    }

    void* result = this->data[this->size-1];
    this->size -= 1;
    return result;
}

void vec_set(vec* this, unsigned int pos, void* data) {
    if (pos < this->size) {
        this->data[pos] = data;
    }
}
