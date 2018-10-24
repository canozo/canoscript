#include <stdlib.h>
#include "bucket.h"

bucket* new_bucket() {
    bucket* this = malloc(sizeof(*this));
    return this;
}

void delete_bucket(bucket* this) {
    if (this->type == B_STRING) {
        free(this->string_value);
    }
    free(this);
}

void bucket_set_integer(bucket* this, char* key, int value) {
    this->key = key;
    this->type = B_INTEGER;
    this->integer_value = value;
    this->real_value = 0;
    this->string_value = NULL;
}

void bucket_set_float(bucket* this, char* key, float value) {
    this->key = key;
    this->type = B_FLOAT;
    this->integer_value = 0;
    this->real_value = value;
    this->string_value = NULL;
}

void bucket_set_string(bucket* this, char* key, char* value) {
    this->key = key;
    this->type = B_STRING;
    this->integer_value = 0;
    this->real_value = 0;
    this->string_value = value;
}
