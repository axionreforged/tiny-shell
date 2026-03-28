#include <stdio.h>
#include <string.h>
#include "input.h"
#include "tokenize.h"

int main(void){
    while(1){
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
        if (normalized_line[0] == '\0') continue;
        printf("%s\n", normalized_line);
        if (strcmp(normalized_line, "exit()") == 0) return 0;

        char *tokens[10];
        tokenize(normalized_line, tokens);
        print_tokens(tokens);
    }
    return 0;
}