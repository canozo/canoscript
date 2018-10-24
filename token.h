#ifndef TOKEN_H
#define TOKEN_H

typedef enum t_type {
    T_EOF,
    T_NUMBER_INT,
    T_NUMBER_REAL,
    T_PLUS,
    T_MINUS,
    T_MULTIPLY,
    T_DIVIDE_FLOOR,
    T_DIVIDE_REAL,
    T_PARENTHESES_OPEN,
    T_PARENTHESES_CLOSE,
    T_ASSIGN,
    T_KEYWORD,
    T_VARIABLE,
    T_SEMICOLON,
    T_UNKNOWN,
    T_ENUM_SIZE
} t_type;

typedef struct token {
    t_type type;
    char* value;
} token;

token* new_token(t_type, char*);
void delete_token(token*);

#endif
