#include <stdlib.h>
#include "vec.h"

vec* new_vec() {
    vec* this = malloc(sizeof(*this));

    this->size = 0;
    this->capacity = 8;
    this->data = malloc(sizeof(void*) * this->capacity);

    return this;
}

void delete_vec(vec* this) {
    free(this->data);
    free(this);
}

void vec_push(vec* this, void* data) {
    if (this->size+1 > this->capacity) {
        vec_resize(this);
    }

    this->data[this->size] = data;
    this->size += 1;
}

void vec_resize(vec* this) {
    this->capacity *= 2;
    this->data = realloc(this->data, sizeof(void*) * this->capacity);
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
