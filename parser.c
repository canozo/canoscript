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
        "T_UNKNOWN",
        "T_ASSIGN",
        "T_KEYWORD",
        "T_VARIABLE"
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
    // number: T_INTEGER
    node* result = NULL;
    token* term = this->current_token;
    eat(this, T_INTEGER);

    if (!this->error) {
        result = new_node_number(term);
        add_node_reference(this, result);
    }

    return result;
}

node* math_parentheses(parser* this) {
    // math_parentheses: T_PLUS math_parentheses
    //                 | T_MINUS math_parentheses
    //                 | T_PARENTHESES_OPEN math T_PARENTHESES_OPEN
    //                 | T_INTEGER
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
    } else {
        result = number_term(this);
    }

    return result;
}

node* math_multiply_divide(parser* this) {
    // math_multiply_divide: math_parentheses ((T_MULTIPLY | T_DIVIDE) math_parentheses)*
    node* result = math_parentheses(this);
    token* eaten;

    while (this->current_token->type == T_MULTIPLY || this->current_token->type == T_DIVIDE) {
        if (this->current_token->type == T_MULTIPLY) {
            eaten = eat(this, T_MULTIPLY);
        } else {
            eaten = eat(this, T_DIVIDE);
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
    // math: math_multiply_divide ((T_PLUS | T_MINUS) math_multiply_divide)*
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

node* statement(parser* this) {
    // statement: compound_statement
    //          | assignment_statement
    //          | empty
    node* result;

    return result;
}

node** statement_list(parser* this) {
    // statement_list: statement
    //               | statement SEMI statement_list
    // TODO use vector
    // TODO add arr reference before or after returning
    // TODO set the size somewhere before returning
    this->sl_size = 0;
    node** results = malloc(sizeof(node));
    results[0] = statement(this);

    while (this->current_token->type == T_SEMICOLON) {
        eat(this, T_SEMICOLON);
        this->sl_size += 1;
        results = realloc(results, this->sl_size);
        results[this->sl_size - 1] = statement(this);
    }

    return results;
}

node* compound_statement(parser* this) {
    // compound_statement: statement_list
    node** nodes = statement_list(this);
    node* result = new_node_compound();
    add_node_reference(this, result);

    for (int i = 0; i < this->sl_size; i++) {
        result->children[result->child_count] = nodes[i];
        result->child_count += 1;
    }

    return result;
}

node* program(parser* this) {
    // program: T_BEGIN compound_statement T_EOF T_END
    eat(this, T_BEGIN);
    node* result = compound_statement(this);
    eat(this, T_END);
    eat(this, T_EOF);
    return result;
}

node* parse(parser* this) {
    return math(this);
}
