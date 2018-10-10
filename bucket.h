#ifndef BUCKET_H
#define BUCKET_H

typedef struct bucket {
    char* key;
    int value;
} bucket;

bucket* new_bucket();
void delete_bucket(bucket*);
void bucket_set(bucket*, char*, int);

#endif
