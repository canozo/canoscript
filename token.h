#ifndef TOKEN_H
#define TOKEN_H

#define T_EOF 0
#define T_NUMBER_INT 1
#define T_NUMBER_REAL 2
#define T_PLUS 3
#define T_MINUS 4
#define T_MULTIPLY 5
#define T_DIVIDE_FLOOR 6
#define T_DIVIDE_REAL 7
#define T_PARENTHESES_OPEN 8
#define T_PARENTHESES_CLOSE 9
#define T_UNKNOWN 10
#define T_ASSIGN 11
#define T_KEYWORD 12
#define T_VARIABLE 13
#define T_SEMICOLON 14

typedef struct token {
    int type;
    char* value;
} token;

token* new_token(int, char*);
void delete_token(token*);

#endif
