#ifndef TOKEN_H
#define TOKEN_H

#define T_EOF 0
#define T_INTEGER 1
#define T_PLUS 2
#define T_MINUS 3
#define T_MULTIPLY 4
#define T_DIVIDE 5
#define T_PARENTHESES_OPEN 6
#define T_PARENTHESES_CLOSE 7
#define T_UNKNOWN 8

typedef struct token {
    int type;
    char* value;
} token;

token* new_token(int, char*);
void delete_token(token*);

#endif
