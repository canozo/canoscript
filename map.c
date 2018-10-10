#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

map* new_map() {
    map* this = malloc(sizeof(*this));

    this->size = 0;
    this->error = 0;

    return this;
}

void delete_map(map* this) {
    for (int i = 0; i < this->size; i++) {
        delete_bucket(this->buckets[i]);
    }
    free(this);
}

void map_set(map* this, char* key, int value) {
    int pos = map_find(this, key);
    if (pos == -1) {
        // make a new bucket for it
        this->buckets[this->size] = new_bucket();
    }

    // change the bucket content
    bucket* buck = this->buckets[this->size];
    bucket_set(buck, key, value);
    this->size += 1;
}

void map_remove(map* this, char* key) {
    int pos = map_find(this, key);
    if (pos != -1) {
        delete_bucket(this->buckets[pos]);
        for (int i = pos + 1; i < this->size; i++) {
            this->buckets[pos-1] = this->buckets[i];
        }
    }
}

int map_get(map* this, char* key) {
    int pos = map_find(this, key);
    if (pos == -1) {
        // error because key doesn't exist
        this->error = 1;
        return -42;
    }

    return this->buckets[pos]->value;
}

int map_find(map* this, char* key) {
    for (int i = 0; i < this->size; i++) {
        if (strcmp(key, this->buckets[i]->key) == 0) {
            return i;
        }
    }
    return -1;
}

void map_print(map* this) {
    for (int i = 0; i < this->size; i++) {
        printf("var: %s, val: %d\n", this->buckets[i]->key, this->buckets[i]->value);
    }
}
