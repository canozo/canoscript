#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"
#include "token.h"

interpreter* new_interpreter(char* text) {
    interpreter* this = malloc(sizeof(interpreter));

    this->error = 0;
    this->print_mode = 1;
    this->parser = new_parser(text);

    return this;
}

void delete_interpreter(interpreter* this) {
    // TODO should probably keep reference count of all nodes
    delete_parser(this->parser);
    free(this);
}

int visit(interpreter* this, node* current_node) {
    // according to the node type, we call the correct visit function
    if (current_node->type == N_NUMBER) {
        return visit_number(this, current_node);

    } else if (current_node->type == N_BINARY_OP) {
        return visit_binary_op(this, current_node);
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
            // sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: division by 0\n");
            this->parser->error_count += 1;

            return -42;
        } else {
            return visit(this, current_node->left) / divide_by;
        }
    } else {
        this->error = ERROR_UNKNOWN_OPERATOR;
        // sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: unknown binary operator %d\n", current_node->token->type);
        this->parser->error_count += 1;
        return -42;
    }
}

int visit_number(interpreter* this, node* current_node) {
    // if it visits a number, return the value
    int thing = strtoint(current_node->token->value);
    return thing;
}

int interpret(interpreter* this) {
    node* root = parse(this->parser);
    this->error = this->parser->error;

    if (!this->error) {
        return visit(this, root);
    } else if (this->print_mode) {
        // TODO fix sprintf on errors
        // print_errors(this);
        return -42;
    }
}

void print_errors(interpreter* this) {
    for (int i = 0; i < this->parser->error_count; i++) {
        printf("%s", this->parser->error_messages[i]);
    }
}

int strtoint(char* string) {
    return (int)strtol(string, NULL, 10);
}
