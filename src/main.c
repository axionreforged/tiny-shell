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
        char *cmd1[10];
        char *cmd2[10];
        enum pipe_status status_parser = parser(normalized_line, tokens, cmd1, cmd2);

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
        if(status_parser == NO_PIPE) execute(tokens, 0, cmd1, cmd2);

        else if(status_parser == PIPE_VALID) execute(tokens, 1, cmd1, cmd2);

        else if(status_parser == NO_LEFT_COMMAND){
            free_memory_of_tokens(tokens);
            fprintf(stderr, "No left command for pipe\n");
            continue;
        }
        else if(status_parser == NO_RIGHT_COMMAND){
            free_memory_of_tokens(tokens);
            fprintf(stderr, "No right command for pipe\n");
            continue;
        }
        else if(status_parser == MORE_THAN_ONE_PIPE){
            free_memory_of_tokens(tokens);
            fprintf(stderr, "More than one pipe\n");
            continue;
        }
        free_memory_of_tokens(tokens);
    }
    return 0;
}