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

bucket* visit(interpreter* this, node* current_node) {
    // according to the node type, we call the correct visit function
    if (current_node->type == N_INTEGER) {
        return visit_number(this, current_node);

    } else if (current_node->type == N_REAL_NUMBER) {
        return visit_real_number(this, current_node);

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

bucket* visit_binary_op(interpreter* this, node* current_node) {
    // TODO all operations that were with integers are now with buckets
    // so now instead of returning a number we should return a new bucket
    // maybe we should manage all the new buckets here instead of in the map
    // that way we could also hold reference to every new bucket made
    // but that sounds like a bad idea
    // SO: we could also use buckets only for the purpose of sending data throug
    // the functions without actually implementing them inside the map
    if (current_node->token->type == T_PLUS) {
        return visit(this, current_node->left) + visit(this, current_node->right);

    } else if (current_node->token->type == T_MINUS) {
        return visit(this, current_node->left) - visit(this, current_node->right);

    } else if (current_node->token->type == T_MULTIPLY) {
        return visit(this, current_node->left) * visit(this, current_node->right);

    } else if (current_node->token->type == T_DIVIDE_FLOOR) {
        int divide_by = visit(this, current_node->right);
        if (divide_by == 0) {
            this->error = ERROR_DIVIDE_BY_ZERO;
            sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: division by 0\n", this->error);
            this->parser->error_count += 1;
            return -42;
        } else {
            return visit(this, current_node->left) / divide_by;
        }

    } else if (current_node->token->type == T_DIVIDE_REAL) {
        float divide_by = visit(this, current_node->right);
        if (divide_by == 0.0) {
            this->error = ERROR_DIVIDE_BY_ZERO;
            sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: division by 0\n", this->error);
            this->parser->error_count += 1;
            return -42;
        } else {
            return visit(this, current_node->left) / divide_by;
        }
    }
}

bucket* visit_unary_op(interpreter* this, node* current_node) {
    if (current_node->token->type == T_PLUS) {
        return +visit(this, current_node->right);

    } else if (current_node->token->type == T_MINUS) {
        return -visit(this, current_node->right);
    }
}

bucket* visit_number(interpreter* this, node* current_node) {
    // if it visits a number, return the value
    int thing = strtoint(current_node->token->value);
    return thing;
}

bucket* visit_compound(interpreter* this, node* current_node) {
    for (int i = 0; i < current_node->children->size; i++) {
        visit(this, vec_get(current_node->children, i));
    }
}

bucket* visit_assign(interpreter* this, node* current_node) {
    char* key = current_node->left->token->value;
    map_set(this->global_scope, key, visit(this, current_node->right));
}

bucket* visit_variable(interpreter* this, node* current_node) {
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

bucket* visit_empty(interpreter* this, node* current_node) {
    return 0;
}

void interpret(interpreter* this) {
    node* root = parse(this->parser);
    this->error = this->parser->error;

    if (this->error) {
        print_errors(this);
    }

    int result = visit(this, root);

    if (this->error) {
        print_errors(this);
    }

    print_global_scope(this);
}

void print_global_scope(interpreter* this) {
    if (this->print_mode) {
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
