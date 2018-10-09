#include "vec.h"

vec* new_vec(size_t data_size) {
    vec* this = malloc(sizeof(vec));

    this->data_size = data_size;
    this->size = 0;
    this->capacity = 2;
    this->data = malloc(this->data_size * this->capacity);

    return this;
}

void delete_vec(vec* this) {
    free(this->data);
    free(this);
}

vec* vec_append(vec* this, void* data) {
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
