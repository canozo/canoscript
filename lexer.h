#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct lexer {
    int pos;
    char* text;
    char current_char;
    int error;
} lexer;

lexer* new_lexer(char*);
void delete_lexer(lexer*);
void advance(lexer*);
token* get_next_token(lexer*);
char* get_integer_str(lexer*);
int strtoint(char*);

#endif
