#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

const parser parser_init = {
    .types = {
        "T_EOF",
        "T_NUMBER_INT",
        "T_NUMBER_REAL",
        "T_PLUS",
        "T_MINUS",
        "T_MULTIPLY",
        "T_DIVIDE_FLOOR",
        "T_DIVIDE_REAL",
        "T_PARENTHESES_OPEN",
        "T_PARENTHESES_CLOSE",
        "T_UNKNOWN",
        "T_ASSIGN",
        "T_KEYWORD",
        "T_VARIABLE",
        "T_SEMICOLON"
    }
};

parser* new_parser(char* text) {
    parser* this = malloc(sizeof(*this));
    memcpy(this, &parser_init, sizeof(*this));

    this->error = 0;
    this->error_count = 0;
    this->lexer = new_lexer(text);
    this->current_token = get_next_token(this->lexer);
    this->node_references = new_vec();
    this->token_references = new_vec();

    if (this->lexer->error) {
        this->error = ERROR_UNEXPECTED_TOKEN;
        sprintf(this->error_messages[this->error_count], "error code %d: unknown token found: \"%s\"\n", this->error, this->current_token->value);
        this->error_count += 1;
    }

    add_token_reference(this, this->current_token);

    return this;
}

void delete_parser(parser* this) {
    unsigned int i;
    // delete all node references
    for (i = 0; i < this->node_references->size; i++) {
        delete_node(vec_get(this->node_references, i));
    }

    // delete all token references
    for (i = 0; i < this->token_references->size; i++) {
        delete_token(vec_get(this->token_references, i));
    }

    delete_vec(this->node_references);
    delete_vec(this->token_references);

    delete_lexer(this->lexer);
    free(this);
}

void add_node_reference(parser* this, node* node_ref) {
    vec_push(this->node_references, node_ref);
}


void add_token_reference(parser* this, token* token_ref) {
    vec_push(this->token_references, token_ref);
}

token* eat(parser* this, int type) {
    token* eaten = NULL;

    if (this->current_token != NULL && this->current_token->type == type) {
        // printf("DEBUG eating %s\n", this->types[type]);
        eaten = this->current_token;
        this->current_token = get_next_token(this->lexer);
        add_token_reference(this, this->current_token);

        if (this->lexer->error) {
            this->error = ERROR_UNEXPECTED_TOKEN;
            sprintf(this->error_messages[this->error_count], "error code %d: unknown token found: \"%s\"\n", this->error, this->current_token->value);
            this->error_count += 1;
            eaten = NULL;
        }
    } else if (this->current_token == NULL) {
        this->error = ERROR_NULL_TOKEN;
        sprintf(this->error_messages[this->error_count], "error code %d: current token is NULL\n", this->error);
        this->error_count += 1;
    } else {
        this->error = ERROR_UNEXPECTED_TYPE;
        sprintf(this->error_messages[this->error_count], "error code %d: type %s doesn't match with expected type %s, token: \"%s\"\n", this->error, this->types[this->current_token->type], this->types[type], this->current_token->value);
        this->error_count += 1;
    }

    return eaten;
}

node* number_term(parser* this) {
    // number: T_NUMBER_INT
    //       | T_NUMBER_REAL

    node* result = NULL;
    token* eaten;

    if (this->current_token->type == T_NUMBER_INT) {
        eaten = eat(this, T_NUMBER_INT);
        result = new_node_integer(eaten);
        add_node_reference(this, result);

    } else {
        eaten = eat(this, T_NUMBER_REAL);
        result = new_node_real_number(eaten);
        add_node_reference(this, result);
    }

    return result;
}

node* math_parentheses(parser* this) {
    // math_parentheses : T_PLUS math_parentheses
    //                  | T_MINUS math_parentheses
    //                  | T_PARENTHESES_OPEN math T_PARENTHESES_OPEN
    //                  | T_NUMBER_INT
    //                  | T_NUMBER_REAL
    //                  | variable

    node* result;
    token* eaten;

    if (this->current_token->type == T_PLUS) {
        eaten = eat(this, T_PLUS);
        result = new_node_unary_op(eaten, math_parentheses(this));

    } else if (this->current_token->type == T_MINUS) {
        eaten = eat(this, T_MINUS);
        result = new_node_unary_op(eaten, math_parentheses(this));

    } else if (this->current_token->type == T_PARENTHESES_OPEN) {
        eat(this, T_PARENTHESES_OPEN);
        result = math(this);
        eat(this, T_PARENTHESES_CLOSE);

    } else if (this->current_token->type == T_NUMBER_INT || this->current_token->type == T_NUMBER_REAL) {
        result = number_term(this);

    } else {
        result = variable(this);
    }

    return result;
}

node* math_multiply_divide(parser* this) {
    // math_multiply_divide : math_parentheses ((T_MULTIPLY | T_DIVIDE_FLOOR | T_DIVIDE_REAL) math_parentheses)*

    node* result = math_parentheses(this);
    token* eaten;

    while (this->current_token->type == T_MULTIPLY || this->current_token->type == T_DIVIDE_FLOOR || this->current_token->type == T_DIVIDE_REAL) {
        if (this->current_token->type == T_MULTIPLY) {
            eaten = eat(this, T_MULTIPLY);
        } else if (this->current_token->type == T_DIVIDE_FLOOR) {
            eaten = eat(this, T_DIVIDE_FLOOR);
        } else {
            eaten = eat(this, T_DIVIDE_REAL);
        }

        result = new_node_binary_op(result, eaten, math_parentheses(this));
        add_node_reference(this, result);

        if (this->error) {
            break;
        }
    }

    return result;
}

node* math(parser* this) {
    // math : math_multiply_divide ((T_PLUS | T_MINUS) math_multiply_divide)*

    node* result = math_multiply_divide(this);
    token* eaten;

    while (this->current_token->type == T_PLUS || this->current_token->type == T_MINUS) {
        if (this->current_token->type == T_PLUS) {
            eaten = eat(this, T_PLUS);
        } else {
            eaten = eat(this, T_MINUS);
        }

        result = new_node_binary_op(result, eaten, math_multiply_divide(this));
        add_node_reference(this, result);

        if (this->error) {
            break;
        }
    }

    return result;
}

node* variable(parser* this) {
    // variable : T_VARIABLE

    node* result = new_node_variable(eat(this, T_VARIABLE));
    add_node_reference(this, result);
    return result;
}

node* assignment_statement(parser* this) {
    // assignment_statement : variable T_ASSIGN math

    node* left = variable(this);
    token* assign_tok = eat(this, T_ASSIGN);
    node* right = math(this);

    // build node
    node* result = new_node_assign(left, assign_tok, right);
    add_node_reference(this, result);

    return result;
}

node* empty_statement(parser* this) {
    // empty_statement :

    node* result = new_node_empty();
    add_node_reference(this, result);
    return result;
}

node* statement(parser* this) {
    // statement : assignment_statement
    //           | empty_statement

    node* result;

    if (this->current_token->type == T_VARIABLE) {
        result = assignment_statement(this);
    } else {
        result = empty_statement(this);
    }

    return result;
}

node* compound_statement(parser* this) {
    // compound_statement : statement
    //                    | statement SEMI compound_statement

    vec* nodes = new_vec();
    vec_push(nodes, statement(this));

    while (this->current_token->type == T_SEMICOLON) {
        eat(this, T_SEMICOLON);
        vec_push(nodes, statement(this));
    }

    node* result = new_node_compound(nodes);
    add_node_reference(this, result);

    return result;
}

node* program(parser* this) {
    // program : compound_statement T_EOF

    node* result = compound_statement(this);
    eat(this, T_EOF);
    return result;
}

node* parse(parser* this) {
    return program(this);
}
