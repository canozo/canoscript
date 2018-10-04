#include <stdio.h>
#include <stdlib.h>
#include "token.h"


token* new_token(int type, char* value) {
    token* this = malloc(sizeof(token));

    this->type = type;
    this->value = value;

    return this;
}

