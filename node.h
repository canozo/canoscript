#ifndef NODE_H
#define NODE_H

#define N_INTEGER 0
#define N_REAL_NUMBER 1
#define N_BINARY_OP 2
#define N_UNARY_OP 3
#define N_ASSIGN 4
#define N_VARIABLE 5
#define N_COMPOUND 6
#define N_EMPTY 7

#include "token.h"
#include "vec.h"

typedef struct node {
    int type;
    struct node* left;
    token* token;
    struct node* right;
    vec* children;
} node;

node* new_node_binary_op(node*, token*, node*);
node* new_node_unary_op(token*, node*);
node* new_node_integer(token*);
node* new_node_real_number(token*);
node* new_node_assign(node*, token*, node*);
node* new_node_variable(token*);
node* new_node_compound(vec*);
node* new_node_empty();
void delete_node(node*);

#endif
