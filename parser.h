#ifndef PARSER_H
#define PARSER_H

typedef enum error_type {
    NO_ERROR,
    ERROR_NULL_TOKEN,
    ERROR_UNEXPECTED_TYPE,
    ERROR_UNEXPECTED_TOKEN,
    ERROR_DIVIDE_BY_ZERO,
    ERROR_VARIABLE_NOT_DEF
} error_type;

#include "token.h"
#include "node.h"
#include "lexer.h"
#include "vec.h"

#define MAX_TYPES 32
#define MAX_MSGS 64
#define MAX_CHARS 128

typedef struct parser {
    token* current_token;
    lexer* lexer;
    vec* node_references;
    vec* token_references;
    error_type error;
    const char* types[T_ENUM_SIZE];
    char error_messages[MAX_MSGS][MAX_CHARS];
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
node* compound_statement(parser*);
node* program(parser*);
node* parse(parser*);

#endif
