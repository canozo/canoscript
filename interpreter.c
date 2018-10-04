#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "interpreter.h"


interpreter* new_interpreter(char* text) {
    interpreter* this = malloc(sizeof(interpreter));

    this->pos = 0;
    this->text = text;
    this->current_char = text[0];
    this->current_token = NULL;

    return this;
}


token* get_next_token(interpreter* this) {
    while (this->current_char != '\0') {
        if (this->current_char == ' ') {
            advance(this);
            continue;
        }

        if (isdigit(this->current_char)) {
            return new_token(T_INTEGER, get_integer_str(this));
        }

        if (this->current_char == '+') {
            advance(this);
            return new_token(T_PLUS, "+");
        }

        if (this->current_char == '-') {
            advance(this);
            return new_token(T_MINUS, "-");
        }

        if (this->current_char == '*') {
            advance(this);
            return new_token(T_MULTIPLY, "*");
        }

        if (this->current_char == '/') {
            advance(this);
            return new_token(T_DIVIDE, "/");
        }

        // at this point we have an error
        exit(-1);
    }
    // at this point, we have reached the end of file
    return new_token(T_EOF, '\0');
}


void advance(interpreter* this) {
    this->pos += 1;
    if (this->pos > strlen(this->text)) {
        this->current_char = '\0';
    } else {
        this->current_char = this->text[this->pos];
    }
}


void eat(interpreter* this, int type) {
    if (this->current_token != NULL && this->current_token->type == type) {
        this->current_token = get_next_token(this);
    } else {
        exit(-1);
    }
}


int number_term(interpreter* this) {
    token* term = this->current_token;
    eat(this, T_INTEGER);
    return strtoint(term->value);
}


int expr(interpreter* this) {
    this->current_token = get_next_token(this);

    token* tok;
    int result = number_term(this);

    while (this->current_token->type == T_PLUS || this->current_token->type == T_MINUS) {
        if (this->current_token->type == T_PLUS) {
            eat(this, T_PLUS);
            result += number_term(this);
        } else {
            eat(this, T_MINUS);
            result -= number_term(this);
        }
    }

    return result;
}


char* get_integer_str(interpreter* this) {
    char* result = NULL;
    char temp[10];
    size_t length = 0;

    while (this->current_char != '\0' && isdigit(this->current_char)) {
        temp[length] = this->current_char;
        length += 1;
        advance(this);
    }

    // null char
    temp[length] = '\0';
    length += 1;

    // pass to the char*
    result = malloc(length);
    strcat(result, temp);

    return result;
}


int strtoint(char* string) {
    return (int)strtol(string, NULL, 10);
}

