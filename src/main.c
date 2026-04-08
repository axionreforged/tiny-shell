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

        char line[MAX_LINE];
        if (fgets(line, sizeof line, stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n");
                break;
            }
            fprintf(stderr, "Input error\n");
            return 1;
        }

        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            int c;
            while ((c = fgetc(stdin)) != '\n' && c != EOF);
            fprintf(stderr, "Input too long\n");
            continue;
        }

        char normalized_line[MAX_LINE];
        normalize(line, normalized_line);

        if (normalized_line[0] == '\0') {
            continue;
        }

        char *tokens[MAX_TOKENS];
        char *cmd1[MAX_TOKENS];
        char *cmd2[MAX_TOKENS];
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
        if (status_parser == NO_PIPE || status_parser == PIPE_VALID) {
            execute(tokens, status_parser, cmd1, cmd2);
        } else {
            if (status_parser == NO_LEFT_COMMAND)
                fprintf(stderr, "No left command for pipe\n");
            else if (status_parser == NO_RIGHT_COMMAND)
                fprintf(stderr, "No right command for pipe\n");
            else if (status_parser == MORE_THAN_ONE_PIPE)
                fprintf(stderr, "More than one pipe\n");
        
            free_memory_of_tokens(tokens);
            continue;
        }

        free_memory_of_tokens(tokens);
    }
    return 0;
}