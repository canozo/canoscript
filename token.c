#include <stdio.h>
#include <stdlib.h>
#include "token.h"

token* new_token(int type, char* value) {
    token* this = malloc(sizeof(token));

    this->type = type;
    this->value = value;

    return this;
}

void delete_token(token* this) {
    if (this->type == T_INTEGER || this->type == T_UNKNOWN) {
        free(this->value);
    }
    free(this);
}
