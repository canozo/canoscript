#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

const parser parser_init = {
    .types = {
        "T_EOF",
        "T_INTEGER",
        "T_PLUS",
        "T_MINUS",
        "T_MULTIPLY",
        "T_DIVIDE",
        "T_PARENTHESES_OPEN",
        "T_PARENTHESES_CLOSE",
        "T_UNKNOWN CAUGTH"
    }
};

parser* new_parser(char* text) {
    parser* this = malloc(sizeof(parser));
    memcpy(this, &parser_init, sizeof(parser));

    this->error = 0;
    this->error_count = 0;
    this->token_ref_pos = 0;
    this->lexer = new_lexer(text);
    this->current_token = get_next_token(this->lexer);

    if (this->lexer->error) {
        this->error = ERROR_UNEXPECTED_TOKEN;
        sprintf(this->error_messages[this->error_count], "error code %d: unknown token found: \"%s\"\n", this->error, this->current_token->value);
        this->error_count += 1;
    }

    add_token_reference(this, this->current_token);

    return this;
}

void delete_parser(parser* this) {
    int i;
    for (i = 0; i < this->node_ref_pos; i++) {
        delete_node(this->node_references[i]);
    }
    for (i = 0; i < this->token_ref_pos; i++) {
        delete_token(this->token_references[i]);
    }
    delete_lexer(this->lexer);
    free(this);
}

void add_node_reference(parser* this, node* node_ref) {
    this->node_references[this->node_ref_pos] = node_ref;
    this->node_ref_pos += 1;
}


void add_token_reference(parser* this, token* token_ref) {
    this->token_references[this->token_ref_pos] = token_ref;
    this->token_ref_pos += 1;
}

token* eat(parser* this, int type) {
    token* eaten = NULL;

    if (this->current_token != NULL && this->current_token->type == type) {
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
    node* result = NULL;
    token* term = this->current_token;
    eat(this, T_INTEGER);

    if (!this->error) {
        result = new_node_number(term);
        add_node_reference(this, result);
    }

    return result;
}

node* expr_parentheses(parser* this) {
    node* result;

    if (this->current_token->type == T_PARENTHESES_OPEN) {
        eat(this, T_PARENTHESES_OPEN);
        result = expr(this);
        eat(this, T_PARENTHESES_CLOSE);
    } else {
        result = number_term(this);
    }

    return result;
}

node* expr_multiply_divide(parser* this) {
    node* result = expr_parentheses(this);
    token* eaten;

    while (this->current_token->type == T_MULTIPLY || this->current_token->type == T_DIVIDE) {
        if (this->current_token->type == T_MULTIPLY) {
            eaten = eat(this, T_MULTIPLY);
        } else {
            eaten = eat(this, T_DIVIDE);
        }

        result = new_node_binary_op(result, eaten, expr_parentheses(this));
        add_node_reference(this, result);

        if (this->error) {
            break;
        }
    }

    return result;
}

node* expr(parser* this) {
    node* result = expr_multiply_divide(this);
    token* eaten;

    while (this->current_token->type == T_PLUS || this->current_token->type == T_MINUS) {
        if (this->current_token->type == T_PLUS) {
            eaten = eat(this, T_PLUS);
        } else {
            eaten = eat(this, T_MINUS);
        }

        result = new_node_binary_op(result, eaten, expr_multiply_divide(this));
        add_node_reference(this, result);

        if (this->error) {
            break;
        }
    }

    return result;
}

node* parse(parser* this) {
    return expr(this);
}
