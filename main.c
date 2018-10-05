#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "interpreter.h"

#define CHUNK 200

char* read_input();

int main() {
    interpreter* interpreter;
    char* input;
    int result;

    while (1) {
        printf("> ");
        input = read_input();

        if (strlen(input) > 0) {
            interpreter = new_interpreter(input);
            result = expr(interpreter);
            if (!interpreter->error)
                printf("%d\n", result);
            delete_interpreter(interpreter);
        }
        free(input);
    }
    return 0;
}

char* read_input() {
    char* result = NULL;
    char buffer[CHUNK];
    size_t len_buffer = 0;
    size_t len_result = 0;

    do {
        fgets(buffer, CHUNK, stdin);

        // check for newlines
        buffer[strcspn(buffer, "\n")] = 0;

        // string sizes
        len_buffer = strlen(buffer);
        len_result += len_buffer;

        // allocate, initialize and concatenate new string
        result = realloc(result, len_result + 1);
        result[0] = '\0';
        strcat(result, buffer);
    } while (len_buffer == CHUNK-1 && buffer[CHUNK-2] != '\n');

    return result;
}
