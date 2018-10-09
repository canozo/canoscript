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
#define T_ASSIGN 9
#define T_KEYWORD 10
#define T_VARIABLE 11
#define T_SEMICOLON 12
#define T_BEGIN 13
#define T_END 14

typedef struct token {
    int type;
    char* value;
} token;

token* new_token(int, char*);
void delete_token(token*);

#endif
