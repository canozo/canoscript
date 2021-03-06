#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "node.h"
#include "parser.h"
#include "map.h"
#include "bucket.h"
#include "vec.h"

typedef struct interpreter {
    parser* parser;
    int print_mode;
    error_type error;
    map* global_scope;
    vec* bucket_references;
} interpreter;

interpreter* new_interpreter(char*);
void delete_interpreter(interpreter*);
void add_bucket_reference(interpreter*, bucket*);
bucket* visit(interpreter*, node*);
bucket* visit_binary_op(interpreter*, node*);
bucket* visit_unary_op(interpreter*, node*);
bucket* visit_number(interpreter*, node*);
void visit_compound(interpreter*, node*);
void visit_assign(interpreter*, node*);
bucket* visit_variable(interpreter*, node*);
void visit_empty(interpreter*, node*);
void interpret(interpreter*);
void print_global_scope(interpreter*);
void print_errors(interpreter*);
int strtoint(char*);
float strtofloat(char*);

#endif
