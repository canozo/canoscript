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
    this->bucket_references = new_vec();

    return this;
}

void delete_interpreter(interpreter* this) {
    // delete bucket references
    for (unsigned int i = 0; i < this->bucket_references->size; i++) {
        delete_bucket(vec_get(this->bucket_references, i));
    }

    delete_parser(this->parser);
    delete_map(this->global_scope);
    free(this);
}

void add_bucket_reference(interpreter* this, bucket* bucket) {
    vec_push(this->bucket_references, bucket);
}

bucket* visit(interpreter* this, node* current_node) {
    // according to the node type, we call the correct visit function
    if (current_node->type == N_INTEGER || current_node->type == N_REAL_NUMBER) {
        return visit_number(this, current_node);

    } else if (current_node->type == N_BINARY_OP) {
        return visit_binary_op(this, current_node);

    } else if (current_node->type == N_UNARY_OP) {
        return visit_unary_op(this, current_node);

    } else if (current_node->type == N_ASSIGN) {
        visit_assign(this, current_node);

    } else if (current_node->type == N_VARIABLE) {
        return visit_variable(this, current_node);

    } else if (current_node->type == N_COMPOUND) {
        visit_compound(this, current_node);

    } else if (current_node->type == N_EMPTY) {
        visit_empty(this, current_node);
    }
}

bucket* visit_binary_op(interpreter* this, node* current_node) {
    bucket* left = visit(this, current_node->left);
    bucket* right = visit(this, current_node->right);

    if (this->error) {
        return NULL;
    }

    bucket* result = new_bucket();
    add_bucket_reference(this, result);

    int real_mode = left->type == B_REAL_NUM || right->type == B_REAL_NUM;

    float left_value = left->type == B_REAL_NUM ? left->real_value : left->integer_value;
    float right_value = right->type == B_REAL_NUM ? right->real_value : right->integer_value;

    if (current_node->token->type == T_PLUS) {
        if (real_mode) {
            bucket_set_real_num(result, NULL, left_value + right_value);
        } else {
            bucket_set_integer(result, NULL, left_value + right_value);
        }

    } else if (current_node->token->type == T_MINUS) {
        if (real_mode) {
            bucket_set_real_num(result, NULL, left_value - right_value);
        } else {
            bucket_set_integer(result, NULL, left_value - right_value);
        }

    } else if (current_node->token->type == T_MULTIPLY) {
        if (real_mode) {
            bucket_set_real_num(result, NULL, left_value * right_value);
        } else {
            bucket_set_integer(result, NULL, left_value * right_value);
        }

    } else if (current_node->token->type == T_DIVIDE_FLOOR) {
        if (right_value == 0) {
            this->error = ERROR_DIVIDE_BY_ZERO;
            sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: division by 0\n", this->error);
            this->parser->error_count += 1;
        } else {
            bucket_set_integer(result, NULL, left_value / right_value);
        }

    } else if (current_node->token->type == T_DIVIDE_REAL) {
        if (right_value == 0) {
            this->error = ERROR_DIVIDE_BY_ZERO;
            sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: division by 0\n", this->error);
            this->parser->error_count += 1;
        } else {
            bucket_set_real_num(result, NULL, left_value / right_value);
        }
    }
    
    return result;
}

bucket* visit_unary_op(interpreter* this, node* current_node) {
    bucket* result = visit(this, current_node->right);

    if (current_node->token->type == T_MINUS) {
        result->integer_value = -result->integer_value;
        result->real_value = -result->real_value;
    }

    return result;
}

bucket* visit_number(interpreter* this, node* current_node) {
    bucket* result = new_bucket();
    add_bucket_reference(this, result);

    if (current_node->type == N_REAL_NUMBER) {
        bucket_set_real_num(result, NULL, strtoint(current_node->token->value));
    } else {
        bucket_set_integer(result, NULL, strtofloat(current_node->token->value));
    }

    return result;
}

void visit_compound(interpreter* this, node* current_node) {
    for (int i = 0; i < current_node->children->size; i++) {
        visit(this, vec_get(current_node->children, i));
    }
}

void visit_assign(interpreter* this, node* current_node) {
    bucket* right = visit(this, current_node->right);

    if (!this->error) {
        char* key = current_node->left->token->value;

        if (right->type == B_REAL_NUM) {
            map_set_real_num(this->global_scope, key, right->real_value);
        } else {
            map_set_integer(this->global_scope, key, right->integer_value);
        }
    }
}

bucket* visit_variable(interpreter* this, node* current_node) {
    bucket* result = map_get(this->global_scope, current_node->token->value);

    if (this->global_scope->error) {
        this->error = ERROR_VARIABLE_NOT_DEF;
        sprintf(this->parser->error_messages[this->parser->error_count], "error code %d: variable %s has not been defined\n", this->error, current_node->token->value);
        this->parser->error_count += 1;
    }

    return result;
}

void visit_empty(interpreter* this, node* current_node) {
}

void interpret(interpreter* this) {
    node* root = parse(this->parser);
    this->error = this->parser->error;

    if (this->error) {
        print_errors(this);
    } else {
        // no errors parsing
        visit(this, root);

        if (this->error) {
            print_errors(this);
        } else {
            // no errors interpreting
            print_global_scope(this);
        }
    }
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

float strtofloat(char* string) {
    return strtof(string, NULL);
}
