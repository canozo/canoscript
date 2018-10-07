#ifndef NODE_H
#define NODE_H

#define N_NUMBER 0
#define N_BINARY_OP 1

#include "token.h"

typedef struct node {
    int type;
    struct node* left;
    token* token;
    struct node* right;
} node;

node* new_node_binary_op(node*, token*, node*);
node* new_node_number(token*);
void delete_node(node*);

#endif
