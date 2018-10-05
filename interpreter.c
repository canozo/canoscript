#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "interpreter.h"

interpreter* new_interpreter(char* text) {
    interpreter* this = malloc(sizeof(interpreter));

    this->ref_pos = 0;
    this->lexer = new_lexer(text);
    this->current_token = get_next_token(this->lexer);

    add_reference(this, this->current_token);

    return this;
}

void delete_interpreter(interpreter* this) {
    for (int i = 0; i < this->ref_pos; i++) {
        delete_token(this->token_references[i]);
    }
    delete_lexer(this->lexer);
    free(this);
}

void add_reference(interpreter* this, token* token_ref) {
    this->token_references[this->ref_pos] = token_ref;
    this->ref_pos += 1;
}

void eat(interpreter* this, int type) {
    if (this->current_token != NULL && this->current_token->type == type) {
        this->current_token = get_next_token(this->lexer);
    } else {
        printf("error: type %d doesn't match with type %d\n", this->current_token->type, type);
        exit(-1);
    }
}

int number_term(interpreter* this) {
    token* term = this->current_token;
    eat(this, T_INTEGER);
    return strtoint(term->value);
}

int mult_expr(interpreter* this) {
    int result = number_term(this);

    while (this->current_token->type == T_MULTIPLY || this->current_token->type == T_DIVIDE) {
        if (this->current_token->type == T_MULTIPLY) {
            eat(this, T_MULTIPLY);
            result *= number_term(this);
        } else {
            eat(this, T_DIVIDE);
            result /= number_term(this);
        }
    }

    return result;
}

int expr(interpreter* this) {
    int result = mult_expr(this);

    while (this->current_token->type == T_PLUS || this->current_token->type == T_MINUS) {
        if (this->current_token->type == T_PLUS) {
            eat(this, T_PLUS);
            result += mult_expr(this);
        } else {
            eat(this, T_MINUS);
            result -= mult_expr(this);
        }
    }

    return result;
}
