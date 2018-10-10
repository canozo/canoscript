#include <stdlib.h>
#include "bucket.h"

bucket* new_bucket() {
    bucket* this = malloc(sizeof(*this));
    return this;
}

void delete_bucket(bucket* this) {
    free(this);
}

void bucket_set(bucket* this, char* key, int value) {
    this->key = key;
    this->value = value;
}
