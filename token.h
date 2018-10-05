#ifndef TOKEN_H
#define TOKEN_H

#define T_EOF 1
#define T_INTEGER 2
#define T_PLUS 3
#define T_MINUS 4
#define T_MULTIPLY 5
#define T_DIVIDE 6

typedef struct token {
    int type;
    char* value;
} token;

token* new_token(int, char*);
void delete_token(token*);

#endif
