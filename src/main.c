#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "exec.h"
#include "input.h"
#include "tokenize.h"

int main(void) {
    while (1) {
        printf("prompt>");
        fflush(stdout);

        char line[100];
        if (fgets(line, sizeof line, stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n");
                break;
            }
            fprintf(stderr, "Input error\n");
            return 1;
        }

        char normalized_line[100];
        normalize(line, normalized_line);

        if (normalized_line[0] == '\0') {
            continue;
        }

        char *tokens[10];
        tokenize(normalized_line, tokens);

        if (tokens[0] == NULL) {
            free_memory_of_tokens(tokens);
            continue;
        }

        if (strcmp(tokens[0], "exit") == 0) {
            free_memory_of_tokens(tokens);
            return 0;
        }

        if (strcmp(tokens[0], "cd") == 0) {
            if (tokens[1] != NULL) {
                if (chdir(tokens[1]) < 0) {
                    perror("cd");
                }
            }
            free_memory_of_tokens(tokens);
            continue;
        }

        execute(tokens);
        free_memory_of_tokens(tokens);
    }

    return 0;
}