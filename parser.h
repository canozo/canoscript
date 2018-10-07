#ifndef PARSER_H
#define PARSER_H

#define ERROR_NULL_TOKEN 1
#define ERROR_UNEXPECTED_TYPE 2
#define ERROR_UNEXPECTED_TOKEN 3
#define ERROR_DIVIDE_BY_ZERO 4

#include "token.h"
#include "node.h"
#include "lexer.h"

typedef struct parser {
    token* current_token;
    lexer* lexer;
    node* node_references[2048];
    int node_ref_pos;
    token* token_references[2048];
    int token_ref_pos;
    int error;
    const char* types[10];
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
node* expr_parentheses(parser*);
node* expr_multiply_divide(parser*);
node* expr(parser*);
node* parse(parser*);

#endif
