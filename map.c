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

void map_set_integer(map* this, char* key, int value) {
    int pos = map_find(this, key);
    if (pos == -1) {
        // make a new bucket for it
        this->buckets[this->size] = new_bucket();

    } else if (this->buckets[pos]->type == B_STRING) {
        // if the last value on the bucket was a string, realloc it
        delete_bucket(this->buckets[pos]);
        this->buckets[this->size] = new_bucket();
    }

    // change the bucket content depending on the type
    if (pos == -1) {
        bucket_set_integer(this->buckets[this->size], key, value);
        this->size += 1;
    } else {
        bucket_set_integer(this->buckets[pos], key, value);
    }
}

void map_set_real_num(map* this, char* key, float value) {
    int pos = map_find(this, key);
    if (pos == -1) {
        // make a new bucket for it
        this->buckets[this->size] = new_bucket();

    } else if (this->buckets[pos]->type == B_STRING) {
        // if the last value on the bucket was a string, realloc it
        delete_bucket(this->buckets[pos]);
        this->buckets[this->size] = new_bucket();
    }

    // change the bucket content depending on the type
    if (pos == -1) {
        bucket_set_real_num(this->buckets[this->size], key, value);
        this->size += 1;
    } else {
        bucket_set_real_num(this->buckets[pos], key, value);
    }
}

void map_set_string(map* this, char* key, char* value) {
    int pos = map_find(this, key);
    if (pos == -1) {
        // make a new bucket for it
        this->buckets[this->size] = new_bucket();

    } else if (this->buckets[pos]->type == B_STRING) {
        // if the last value on the bucket was a string, realloc it
        delete_bucket(this->buckets[pos]);
        this->buckets[this->size] = new_bucket();
    }

    // change the bucket content depending on the type
    if (pos == -1) {
        bucket_set_string(this->buckets[this->size], key, value);
        this->size += 1;
    } else {
        bucket_set_string(this->buckets[pos], key, value);
    }
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

bucket* map_get(map* this, char* key) {
    int pos = map_find(this, key);
    if (pos == -1) {
        // error because key doesn't exist
        this->error = 1;
        return NULL;
    }

    return this->buckets[pos];
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
    printf("GLOBAL SCOPE:\n");
    for (int i = 0; i < this->size; i++) {
        if (this->buckets[i]->type == B_INTEGER) {
            printf("var: %s, val: %d\n", this->buckets[i]->key, this->buckets[i]->integer_value);

        } else if (this->buckets[i]->type == B_REAL_NUM) {
            printf("var: %s, val: %.6f\n", this->buckets[i]->key, this->buckets[i]->real_value);

        } else if (this->buckets[i]->type == B_STRING) {
            printf("var: %s, val: \"%s\"\n", this->buckets[i]->key, this->buckets[i]->string_value);
        }
    }
}
