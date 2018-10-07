#ifndef PARSER_H
#define PARSER_H

#define ERROR_NULL_TOKEN 1
#define ERROR_UNEXPECTED_TYPE 2
#define ERROR_UNEXPECTED_TOKEN 3
#define ERROR_DIVIDE_BY_ZERO 4
#define ERROR_UNKNOWN_OPERATOR 5

#include "token.h"
#include "node.h"
#include "lexer.h"

typedef struct parser {
    token* current_token;
    lexer* lexer;
    token* token_references[2048];
    int ref_pos;
    int error;
    const char* types[10];
    int error_count;
    char* error_messages[32];
} parser;

extern const parser parser_init;

parser* new_parser(char*);
void delete_parser(parser*);
void add_reference(parser*, token*);
token* eat(parser*, int);
node* number_term(parser*);
node* expr_parentheses(parser*);
node* expr_multiply_divide(parser*);
node* expr(parser*);
node* parse(parser*);

#endif
