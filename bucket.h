#ifndef BUCKET_H
#define BUCKET_H

typedef enum b_type {
    B_INTEGER,
    B_FLOAT,
    B_STRING
} b_type;

typedef struct bucket {
    char* key;
    b_type type;
    int integer_value;
    float real_value;
    char* string_value;
} bucket;

bucket* new_bucket();
void delete_bucket(bucket*);
void bucket_set_integer(bucket*, char*, int);
void bucket_set_float(bucket*, char*, float);
void bucket_set_string(bucket*, char*, char*);

#endif
