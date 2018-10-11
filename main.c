#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "interpreter.h"
#include "map.h"

#define CHUNK 32

void run_program();
void run_tests();
char* read_input();
char* read_file(char*);

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        run_tests();
    } else {
        run_program();
    }

    return 0;
}

void run_program() {
    interpreter* interpreter;
    char* input = read_file("program.cs");

    interpreter = new_interpreter(input);
    interpret(interpreter);
    delete_interpreter(interpreter);
    free(input);
}

void run_tests() {
    printf("Starting tests!\n");

    char* input = read_file("tests/test_math.cs");
    interpreter* interpreter = new_interpreter(input);
    int result;

    interpreter->print_mode = 0;
    assert(!interpreter->error);
    interpret(interpreter);

    // 5 = 5
    result = map_get(interpreter->global_scope, "a");
    assert(result == 5);

    // (5) = 5
    result = map_get(interpreter->global_scope, "b");
    assert(result == 5);

    // 3 / 2 = 1
    result = map_get(interpreter->global_scope, "c");
    assert(result == 1);

    // 7 + 3 * (10 / (12 / (3 + 1) - 1)) = 22
    result = map_get(interpreter->global_scope, "d");
    assert(result == 22);

    // 7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8) = 10
    result = map_get(interpreter->global_scope, "e");
    assert(result == 10);

    // 7 + (((3 + 2))) = 12
    result = map_get(interpreter->global_scope, "f");
    assert(result == 12);

    // -(2) = -2
    result = map_get(interpreter->global_scope, "g");
    assert(result == -2);

    // -+-2 = 2
    result = map_get(interpreter->global_scope, "h");
    assert(result == 2);

    delete_interpreter(interpreter);
    free(input);

    // division by 0 error
    interpreter = new_interpreter("a = 25 / (5 - 3 - 2);");
    interpreter->print_mode = 0;
    assert(!interpreter->error);
    interpret(interpreter);
    assert(interpreter->error);
    delete_interpreter(interpreter);

    // undefined variable error
    interpreter = new_interpreter("a = 2 + x;");
    interpreter->print_mode = 0;
    assert(!interpreter->error);
    interpret(interpreter);
    assert(interpreter->error);
    delete_interpreter(interpreter);

    printf("Passed all the tests!\n");
}

char* read_input() {
    char* result = malloc(1);
    result[0] = '\0';
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
        strcat(result, buffer);
    } while (len_buffer == CHUNK-1 && buffer[CHUNK-2] != '\n');

    return result;
}

char* read_file(char* filename) {
    char* result = NULL;
    FILE* file = fopen(filename, "r");
    if (file) {
        fseek(file, 0L, SEEK_END);
        long len_buffer = ftell(file);
        result = malloc(len_buffer + 1);
        fseek(file, 0L, SEEK_SET);

        size_t len_new = fread(result, sizeof(char), len_buffer, file);
        result[len_new] = '\0';
    }
    fclose(file);

    return result;
}
