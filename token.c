#include <stdlib.h>
#include "token.h"

token* new_token(int type, char* value) {
    token* this = malloc(sizeof(*this));

    this->type = type;
    this->value = value;

    return this;
}

void delete_token(token* this) {
    if (this->type == T_INTEGER || this->type == T_UNKNOWN || this->type == T_KEYWORD || this->type == T_VARIABLE) {
        free(this->value);
    }
    free(this);
}
