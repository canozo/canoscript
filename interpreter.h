#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "node.h"
#include "parser.h"

typedef struct interpreter {
    parser* parser;
    int print_mode;
    int error;
} interpreter;

interpreter* new_interpreter(char*);
void delete_interpreter(interpreter*);
int visit(interpreter*, node*);
int visit_binary_op(interpreter*, node*);
int visit_unary_op(interpreter*, node*);
int visit_number(interpreter*, node*);
int interpret(interpreter*);
void print_errors(interpreter*);
int strtoint(char*);

#endif
