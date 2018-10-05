#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

lexer* new_lexer(char* text) {
    lexer* this = malloc(sizeof(lexer));

    this->error = 0;
    this->pos = 0;
    this->text = text;
    this->current_char = text[0];

    return this;
}

void delete_lexer(lexer* this) {
    free(this);
}

token* get_next_token(lexer* this) {
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

        if (this->current_char == '(') {
            advance(this);
            return new_token(T_PARENTH_OPEN, "(");
        }

        if (this->current_char == ')') {
            advance(this);
            return new_token(T_PARENTH_CLOSE, ")");
        }

        // at this point we found a token/character that we don't recognize
        this->error = 1;
        break;
    }
    // at this point, we have reached the end of file
    return new_token(T_EOF, "\0");
}

void advance(lexer* this) {
    this->pos += 1;
    if (this->pos > strlen(this->text)) {
        this->current_char = '\0';
    } else {
        this->current_char = this->text[this->pos];
    }
}

char* get_integer_str(lexer* this) {
    char* result = NULL;
    char temp[10];
    size_t length = 0;

    while (this->current_char != '\0' && isdigit(this->current_char) && length < 10) {
        temp[length] = this->current_char;
        length += 1;
        advance(this);
    }

    // null char at the end
    temp[length] = '\0';
    length += 1;

    // allocate, initialize and concatenate new string
    result = malloc(length);
    result[0] = '\0';
    strcat(result, temp);

    return result;
}

int strtoint(char* string) {
    return (int)strtol(string, NULL, 10);
}
