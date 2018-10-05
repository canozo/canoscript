#ifndef INTERPRETER_H
#define INTERPRETER_H

#define ERROR_NULL_TOKEN 1
#define ERROR_UNEXPECTED_TYPE 2
#define ERROR_UNEXPECTED_TOKEN 3

#include "token.h"
#include "lexer.h"

typedef struct interpreter {
    token* current_token;
    lexer* lexer;
    token* token_references[2048];
    int ref_pos;
    int error;
} interpreter;

interpreter* new_interpreter(char*);
void delete_interpreter(interpreter*);
void add_reference(interpreter*, token*);
void eat(interpreter*, int);
int mult_expr(interpreter*);
int expr(interpreter*);

#endif
