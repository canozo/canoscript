#include <stdlib.h>
#include "node.h"

node* new_node_binary_op(node* left, token* token, node* right) {
    node* this = malloc(sizeof(node));

    this->type = N_BINARY_OP;
    this->left = left;
    this->token = token;
    this->right = right;

    return this;
}

node* new_node_unary_op(token* token, node* right) {
    node* this = malloc(sizeof(node));

    this->type = N_UNARY_OP;
    this->left = NULL;
    this->token = token;
    this->right = right;

    return this;
}


node* new_node_number(token* token) {
    node* this = malloc(sizeof(node));

    this->type = N_NUMBER;
    this->left = NULL;
    this->token = token;
    this->right = NULL;

    return this;
}

void delete_node(node* this) {
    free(this);
}
