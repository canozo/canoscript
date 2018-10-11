#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "node.h"
#include "parser.h"
#include "map.h"
#include "bucket.h"

typedef struct interpreter {
    parser* parser;
    int print_mode;
    int error;
    map* global_scope;
} interpreter;

interpreter* new_interpreter(char*);
void delete_interpreter(interpreter*);
bucket* visit(interpreter*, node*);
bucket* visit_binary_op(interpreter*, node*);
bucket* visit_unary_op(interpreter*, node*);
bucket* visit_number(interpreter*, node*);
bucket* visit_real_number(interpreter*, node*);
bucket* visit_compound(interpreter*, node*);
bucket* visit_assign(interpreter*, node*);
bucket* visit_variable(interpreter*, node*);
bucket* visit_empty(interpreter*, node*);
void interpret(interpreter*);
void print_global_scope(interpreter*);
void print_errors(interpreter*);
int strtoint(char*);

#endif
