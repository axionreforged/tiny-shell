#include "tokenize.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void tokenize(const char *line, char *tokens[]) {
    int line_index = 0;
    int token_index = 0;

    while (line[line_index] != '\0') {
        while (line[line_index] == ' ') {
            line_index++;
        }

        if (line[line_index] == '\0') {
            break;
        }

        if (token_index >= 9) {
            break;
        }

        if (line[line_index] == '>' || line[line_index] == '<') {
            tokens[token_index] = malloc(2);
            if (tokens[token_index] == NULL) {
                perror("malloc");
                break;
            }

            tokens[token_index][0] = line[line_index];
            tokens[token_index][1] = '\0';
            token_index++;
            line_index++;
            continue;
        }

        tokens[token_index] = malloc(101);
        if (tokens[token_index] == NULL) {
            perror("malloc");
            break;
        }

        int word_index = 0;

        while (line[line_index] != '\0' &&
               line[line_index] != ' ' &&
               line[line_index] != '>' &&
               line[line_index] != '<') {
            if (word_index < 100) {
                tokens[token_index][word_index] = line[line_index];
                word_index++;
            }
            line_index++;
        }

        tokens[token_index][word_index] = '\0';
        token_index++;
    }

    tokens[token_index] = NULL;
}

void print_tokens(char *tokens[]) {
    int i = 0;

    while (tokens[i] != NULL) {
        printf("tokens[%d] = %s\n", i, tokens[i]);
        i++;
    }
}

void free_memory_of_tokens(char *tokens[]) {
    int index = 0;

    while (tokens[index] != NULL) {
        free(tokens[index]);
        tokens[index] = NULL;
        index++;
    }
}