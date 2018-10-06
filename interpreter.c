#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "interpreter.h"

const interpreter interpreter_init = {
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

interpreter* new_interpreter(char* text) {
    interpreter* this = malloc(sizeof(interpreter));
    memcpy(this, &interpreter_init, sizeof(interpreter));

    this->error = 0;
    this->test_mode = 0;
    this->ref_pos = 0;
    this->lexer = new_lexer(text);
    this->current_token = get_next_token(this->lexer);

    if (this->lexer->error) {
        this->error = ERROR_UNEXPECTED_TOKEN;
        if (!this->test_mode) {
            printf("error code %d: unknown token found: \"%s\"\n", this->error, this->current_token->value);
        }
    }

    add_reference(this, this->current_token);

    return this;
}

void delete_interpreter(interpreter* this) {
    for (int i = 0; i < this->ref_pos; i++) {
        delete_token(this->token_references[i]);
    }
    delete_lexer(this->lexer);
    free(this);
}

void add_reference(interpreter* this, token* token_ref) {
    this->token_references[this->ref_pos] = token_ref;
    this->ref_pos += 1;
}

void eat(interpreter* this, int type) {
    if (this->current_token != NULL && this->current_token->type == type) {
        this->current_token = get_next_token(this->lexer);
        add_reference(this, this->current_token);

        if (this->lexer->error) {
            this->error = ERROR_UNEXPECTED_TOKEN;
            if (!this->test_mode) {
                printf("error code %d: unknown token found: \"%s\"\n", this->error, this->current_token->value);
            }
        }
    } else if (this->current_token == NULL) {
        this->error = ERROR_NULL_TOKEN;
        if (!this->test_mode) {
            printf("error code %d: current token is NULL\n", this->error);
        }
    } else {
        this->error = ERROR_UNEXPECTED_TYPE;
        if (!this->test_mode) {
            printf("error code %d: type %s doesn't match with expected type %s, token: \"%s\"\n", this->error, this->types[this->current_token->type], this->types[type], this->current_token->value);
        }
    }
}

int number_term(interpreter* this) {
    token* term = this->current_token;
    eat(this, T_INTEGER);

    if (!this->error) {
        return strtoint(term->value);
    } else {
        return -1;
    }
}

int expr_parentheses(interpreter* this) {
    int result;

    if (this->current_token->type == T_PARENTHESES_OPEN) {
        eat(this, T_PARENTHESES_OPEN);
        result = expr(this);
        eat(this, T_PARENTHESES_CLOSE);
    } else {
        result = number_term(this);
    }

    return result;
}

int expr_multiply_divide(interpreter* this) {
    int divide_by;
    int result = expr_parentheses(this);

    while (this->current_token->type == T_MULTIPLY || this->current_token->type == T_DIVIDE) {
        if (this->current_token->type == T_MULTIPLY) {
            eat(this, T_MULTIPLY);
            result *= expr_parentheses(this);
        } else {
            eat(this, T_DIVIDE);
            divide_by = expr_parentheses(this);

            // check division by 0
            if (divide_by == 0) {
                this->error = ERROR_DIVIDE_BY_ZERO;
                if (!this->test_mode) {
                    printf("error code %d: can't divide by 0\n", this->error);
                }
                break;
            } else {
                result /= divide_by;
            }
        }

        if (this->error) {
            break;
        }
    }

    return result;
}

int expr(interpreter* this) {
    int result = expr_multiply_divide(this);

    while (this->current_token->type == T_PLUS || this->current_token->type == T_MINUS) {
        if (this->current_token->type == T_PLUS) {
            eat(this, T_PLUS);
            result += expr_multiply_divide(this);
        } else {
            eat(this, T_MINUS);
            result -= expr_multiply_divide(this);
        }

        if (this->error) {
            break;
        }
    }

    return result;
}
