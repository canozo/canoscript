#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "token.h"


typedef struct interpreter {
    int pos;
    char* text;
    char current_char;
    token* current_token;
} interpreter;


interpreter* new_interpreter(char*);
token* get_next_token(interpreter*);
void advance(interpreter*);
void eat(interpreter*, int);
int expr(interpreter*);
char* get_integer_str(interpreter*);
int strtoint(char*);

#endif
