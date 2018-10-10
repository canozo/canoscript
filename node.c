#include <stdlib.h>
#include "node.h"

node* new_node_binary_op(node* left, token* token, node* right) {
    node* this = malloc(sizeof(*this));

    this->type = N_BINARY_OP;
    this->left = left;
    this->token = token;
    this->right = right;
    this->children = NULL;

    return this;
}

node* new_node_unary_op(token* token, node* right) {
    node* this = malloc(sizeof(*this));

    this->type = N_UNARY_OP;
    this->left = NULL;
    this->token = token;
    this->right = right;
    this->children = NULL;

    return this;
}


node* new_node_number(token* token) {
    node* this = malloc(sizeof(*this));

    this->type = N_NUMBER;
    this->left = NULL;
    this->token = token;
    this->right = NULL;

    return this;
}

node* new_node_assign(node* left, token* token, node* right) {
    node* this = malloc(sizeof(*this));

    this->type = N_ASSIGN;
    this->left = left;
    this->token = token;
    this->right = right;
    this->children = NULL;

    return this;
}

node* new_node_variable(token* token) {
    node* this = malloc(sizeof(*this));

    this->type = N_VARIABLE;
    this->left = NULL;
    this->token = token;
    this->right = NULL;
    this->children = NULL;

    return this;
}

node* new_node_compound(vec* children) {
    node* this = malloc(sizeof(*this));

    this->type = N_COMPOUND;
    this->left = NULL;
    this->token = NULL;
    this->right = NULL;
    this->children = children;

    return this;
}

node* new_node_empty() {
    node* this = malloc(sizeof(*this));

    this->type = N_EMPTY;
    this->left = NULL;
    this->token = NULL;
    this->right = NULL;
    this->children = NULL;

    return this;
}


void delete_node(node* this) {
    if (this->type == N_COMPOUND) {
        // TODO mem fix this
        // delete_vec(this->children);
    }
    free(this);
}
