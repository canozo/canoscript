#ifndef BUCKET_H
#define BUCKET_H

#define B_INTEGER 0
#define B_REAL_NUM 1
#define B_STRING 2

typedef struct bucket {
    char* key;
    int type;
    int integer_value;
    float real_value;
    char* string_value;
} bucket;

bucket* new_bucket();
void delete_bucket(bucket*);
void bucket_set_integer(bucket*, char*, int);
void bucket_set_real_num(bucket*, char*, float);
void bucket_set_string(bucket*, char*, char*);

#endif
