#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

const lexer lexer_init = {
    .keywords = {
        "begin",
        "end"
    }
};

lexer* new_lexer(char* text) {
    lexer* this = malloc(sizeof(*this));
    memcpy(this, &lexer_init, sizeof(*this));

    this->error = 0;
    this->pos = 0;
    this->text = text;
    this->current_char = text[0];

    return this;
}

void delete_lexer(lexer* this) {
    free(this);
}

token* get_next_token(lexer* this) {
    char* word = NULL;

    while (this->current_char != '\0') {
        if (this->current_char == ' ' || this->current_char == '\n') {
            advance(this);
            continue;
        }

        if (this->current_char == '/' && peek(this) == '/') {
            skip_comment(this);
            continue;
        }

        if (isdigit(this->current_char)) {
            return new_token(T_INTEGER, get_integer_str(this));
        }

        if (this->current_char == '+') {
            advance(this);
            return new_token(T_PLUS, "+");
        }

        if (this->current_char == '-') {
            advance(this);
            return new_token(T_MINUS, "-");
        }

        if (this->current_char == '*') {
            advance(this);
            return new_token(T_MULTIPLY, "*");
        }

        if (this->current_char == '/') {
            advance(this);
            return new_token(T_DIVIDE, "/");
        }

        if (this->current_char == '(') {
            advance(this);
            return new_token(T_PARENTHESES_OPEN, "(");
        }

        if (this->current_char == ')') {
            advance(this);
            return new_token(T_PARENTHESES_CLOSE, ")");
        }

        if (this->current_char == ';') {
            advance(this);
            return new_token(T_SEMICOLON, ";");
        }

        if (this->current_char == '=') {
            advance(this);
            return new_token(T_ASSIGN, "=");
        }

        if (isalpha(this->current_char)) {
            // it can either be a reserved keyword or a new identifier
            word = get_id_str(this);

            if (iskeyword(this, word)) {
                return new_token(T_KEYWORD, word);
            } else {
                return new_token(T_VARIABLE, word);
            }
        }

        // at this point we found a token/character that we don't recognize
        this->error = 1;
        char* unknown = malloc(2);
        unknown[0] = this->current_char;
        unknown[1] = '\0';
        return new_token(T_UNKNOWN, unknown);
    }
    // at this point, we have reached the end of file
    return new_token(T_EOF, "\0");
}

int iskeyword(lexer* this, char* word) {
    for (int i = 0; i < 16; i++) {
        if (this->keywords[i] == NULL) {
            break;
        }

        if (strcasecmp(word, this->keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char peek(lexer* this) {
    // peek one character ahead
    int peek_pos = this->pos + 1;
    if (peek_pos > strlen(this->text)) {
        return '\0';
    } else {
        return this->text[peek_pos];
    }
}

void advance(lexer* this) {
    this->pos += 1;
    if (this->pos > strlen(this->text)) {
        this->current_char = '\0';
    } else {
        this->current_char = this->text[this->pos];
    }
}

void skip_comment(lexer* this) {
    char last;
    while (this->current_char != '\0') {
        last = this->current_char;
        advance(this);
        if (last == '\n') {
            break;
        }
    }
}

char* get_id_str(lexer* this) {
    char* result = NULL;
    char temp[32];
    size_t length = 0;

    while (this->current_char != '\0' && (isalpha(this->current_char) || this->current_char == '_') && length < 32) {
        temp[length] = this->current_char;
        length += 1;
        advance(this);
    }

    // null char at the end
    temp[length] = '\0';
    length += 1;

    // allocate, initialize and concatenate new string
    result = malloc(length);
    result[0] = '\0';
    strcat(result, temp);

    return result;
}

char* get_integer_str(lexer* this) {
    char* result = NULL;
    char temp[10];
    size_t length = 0;

    while (this->current_char != '\0' && isdigit(this->current_char) && length < 10) {
        temp[length] = this->current_char;
        length += 1;
        advance(this);
    }

    // null char at the end
    temp[length] = '\0';
    length += 1;

    // allocate, initialize and concatenate new string
    result = malloc(length);
    result[0] = '\0';
    strcat(result, temp);

    return result;
}
