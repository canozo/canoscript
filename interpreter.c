#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"
#include "token.h"

interpreter* new_interpreter(char* text) {
    interpreter* this = malloc(sizeof(*this));

    this->error = 0;
    this->print_mode = 1;
    this->parser = new_parser(text);
    this->global_scope = new_map();

    return this;
}

void delete_interpreter(interpreter* this) {
    delete_parser(this->parser);
    delete_map(this->global_scope);
    free(this);
}

int visit(interpreter* this, node* current_node) {
    // according to the node type, we call the correct visit function
    if (current_node->type == N_NUMBER) {
        return visit_number(this, current_node);

    } else if (current_node->type == N_BINARY_OP) {
        return visit_binary_op(this, current_node);

    } else if (current_node->type == N_UNARY_OP) {
        return visit_unary_op(this, current_node);

    } else if (current_node->type == N_ASSIGN) {
        return visit_assign(this, current_node);

    } else if (current_node->type == N_VARIABLE) {
        return visit_variable(this, current_node);

    } else if (current_node->type == N_COMPOUND) {
        return visit_compound(this, current_node);

    } else if (current_node->type == N_EMPTY) {
        return visit_empty(this, current_node);
    }
}

int visit_binary_op(interpreter* this, node* current_node) {
    int divide_by;
    if (current_node->token->type == T_PLUS) {
        return visit(this, current_node->left) + visit(this, current_node->right);

    } else if (current_node->token->type == T_MINUS) {
        return visit(this, current_node->left) - visit(this, current_node->right);

    } else if (current_node->token->type == T_MULTIPLY) {
        return visit(this, current_node->left) * visit(this, current_node->right);

    } else if (current_node->token->type == T_DIVIDE) {
        divide_by = visit(this, current_node->right);

        if (divide_by == 0) {
            this->error = ERROR_DIVIDE_BY_ZERO;
            sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: division by 0\n", this->error);
            this->parser->error_count += 1;
            return -42;
        } else {
            return visit(this, current_node->left) / divide_by;
        }
    }
}

int visit_unary_op(interpreter* this, node* current_node) {
    if (current_node->token->type == T_PLUS) {
        return +visit(this, current_node->right);

    } else if (current_node->token->type == T_MINUS) {
        return -visit(this, current_node->right);
    }
}

int visit_number(interpreter* this, node* current_node) {
    // if it visits a number, return the value
    int thing = strtoint(current_node->token->value);
    return thing;
}

int visit_compound(interpreter* this, node* current_node) {
    for (int i = 0; i < current_node->children->size; i++) {
        visit(this, vec_get(current_node->children, i));
    }
}

int visit_assign(interpreter* this, node* current_node) {
    char* key = current_node->left->token->value;
    map_set(this->global_scope, key, visit(this, current_node->right));
}

int visit_variable(interpreter* this, node* current_node) {
    char* key = current_node->token->value;
    int value = map_get(this->global_scope, key);

    if (this->global_scope->error) {
        this->error = ERROR_VARIABLE_NOT_DEF;
        sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: variable %s has not been defined\n", this->error, key);
        this->parser->error_count += 1;
        return -42;
    } else {
        return value;
    }
}

int visit_empty(interpreter* this, node* current_node) {
    return 0;
}

int interpret(interpreter* this) {
    // TODO change some of this functions to void, int is useless now
    node* root = parse(this->parser);
    this->error = this->parser->error;

    if (this->error) {
        print_errors(this);
        return -42;
    }

    int result = visit(this, root);

    if (this->error) {
        print_errors(this);
        return -42;
    }

    print_global_scope(this);

    return result;
}

void print_global_scope(interpreter* this) {
    if (this->print_mode) {
        printf("GLOBAL SCOPE:\n");
        map_print(this->global_scope);
    }
}

void print_errors(interpreter* this) {
    if (this->print_mode) {
        for (int i = 0; i < this->parser->error_count; i++) {
            printf("%s", this->parser->error_messages[i]);
        }
    }
}

int strtoint(char* string) {
    return (int)strtol(string, NULL, 10);
}
