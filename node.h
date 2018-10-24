#ifndef NODE_H
#define NODE_H

typedef enum n_type {
    N_INTEGER,
    N_FLOAT,
    N_BINARY_OP,
    N_UNARY_OP,
    N_ASSIGN,
    N_VARIABLE,
    N_COMPOUND,
    N_EMPTY
} n_type;

#include "token.h"
#include "vec.h"

typedef struct node {
    n_type type;
    struct node* left;
    token* token;
    struct node* right;
    vec* children;
} node;

node* new_node_binary_op(node*, token*, node*);
node* new_node_unary_op(token*, node*);
node* new_node_integer(token*);
node* new_node_float(token*);
node* new_node_assign(node*, token*, node*);
node* new_node_variable(token*);
node* new_node_compound(vec*);
node* new_node_empty();
void delete_node(node*);

#endif
