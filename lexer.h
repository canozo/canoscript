#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct lexer {
    int pos;
    char* text;
    char current_char;
    const char* keywords[16];
    int error;
} lexer;

extern const lexer lexer_init;

lexer* new_lexer(char*);
void delete_lexer(lexer*);
char peek(lexer*);
int iskeyword(lexer*, char*);
void advance(lexer*);
token* get_next_token(lexer*);
void skip_comment(lexer*);
char* get_id_str(lexer*);
token* get_number_tok(lexer*);

#endif
