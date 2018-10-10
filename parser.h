#ifndef PARSER_H
#define PARSER_H

#define ERROR_NULL_TOKEN 1
#define ERROR_UNEXPECTED_TYPE 2
#define ERROR_UNEXPECTED_TOKEN 3
#define ERROR_DIVIDE_BY_ZERO 4
#define ERROR_VARIABLE_NOT_DEF 5

#include "token.h"
#include "node.h"
#include "lexer.h"
#include "vec.h"

typedef struct parser {
    token* current_token;
    lexer* lexer;
    vec* node_references;
    vec* token_references;
    int error;
    const char* types[32];
    // 64 messages of 128 characters max:
    char error_messages[64][128];
    int error_count;
} parser;

extern const parser parser_init;

parser* new_parser(char*);
void delete_parser(parser*);
void add_node_reference(parser*, node*);
void add_token_reference(parser*, token*);
token* eat(parser*, int);
node* number_term(parser*);
node* math_parentheses(parser*);
node* math_multiply_divide(parser*);
node* math(parser*);
node* variable(parser*);
node* assignment_statement(parser*);
node* empty_statement(parser*);
node* statement(parser*);
vec* statement_list(parser*);
node* compound_statement(parser*);
node* program(parser*);
node* parse(parser*);

#endif
