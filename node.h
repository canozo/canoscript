#ifndef NODE_H
#define NODE_H

#define N_NUMBER 0
#define N_BINARY_OP 1
#define N_UNARY_OP 2
#define N_ASSIGN 3
#define N_VARIABLE 4
#define N_COMPOUND 5
#define N_EMPTY 5

#include "token.h"

typedef struct node {
    int type;
    struct node* left;
    token* token;
    struct node* right;
    struct node* children[128];
    int child_count;
} node;

node* new_node_binary_op(node*, token*, node*);
node* new_node_unary_op(token*, node*);
node* new_node_number(token*);
node* new_node_assign(node*, token*, node*);
node* new_node_variable(token*);
node* new_node_compound();
node* new_node_empty();
void delete_node(node*);

#endif
