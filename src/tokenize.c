#include "tokenize.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int tokenize(const char *line, char *tokens[]) {
    int line_index = 0;
    int token_index = 0;

    while (line[line_index] != '\0') {
        while (line[line_index] == ' ') {
            line_index++;
        }

        if (line[line_index] == '\0') {
            break;
        }

        if (token_index >= MAX_TOKENS - 1) {
            break;
        }

        if (line[line_index] == '>' || line[line_index] == '<' || line[line_index] == '|') {
            tokens[token_index] = malloc(2);
            if (tokens[token_index] == NULL) {
                perror("malloc");
                tokens[token_index] = NULL;
                free_memory_of_tokens(tokens);
                return -1;
            }

            tokens[token_index][0] = line[line_index];
            tokens[token_index][1] = '\0';
            token_index++;
            line_index++;
            continue;
        }

        tokens[token_index] = malloc(MAX_LINE + 1);
        if (tokens[token_index] == NULL) {
            perror("malloc");
            tokens[token_index] = NULL;
            free_memory_of_tokens(tokens);
            return -1;
        }

        int word_index = 0;

        while (line[line_index] != '\0' &&
               line[line_index] != ' ' &&
               line[line_index] != '>' &&
               line[line_index] != '<' &&
               line[line_index] != '|') {
            if (word_index < MAX_LINE) {
                tokens[token_index][word_index] = line[line_index];
                word_index++;
            }
            line_index++;
        }

        tokens[token_index][word_index] = '\0';
        token_index++;
    }

    tokens[token_index] = NULL;
    return 0;
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

int check_for_pipe(char *tokens[], int starting_index){
    int index = starting_index;
    while(tokens[index] != NULL){
        if(strcmp(tokens[index], "|") == 0) return index;
        index++;
    }
    return -1;
}

enum pipe_status validate_pipe(char *tokens[], int index_of_pipe){
    if(index_of_pipe == 0) return NO_LEFT_COMMAND;
    else if(tokens[index_of_pipe + 1] == NULL) return NO_RIGHT_COMMAND;
    else if(check_for_pipe(tokens, index_of_pipe + 1) > -1) return MORE_THAN_ONE_PIPE;
    else return PIPE_VALID;
}

void split_on_pipe(char *tokens[], int index_of_pipe, char *cmd1[], char *cmd2[]){
    int index = 0;
    while(index < index_of_pipe){
        cmd1[index] = tokens[index];
        index++;
    }
    cmd1[index] = NULL;
    index++;

    int cmd2_index = 0;
    while(tokens[index] != NULL){
        cmd2[cmd2_index] = tokens[index];
        index++;
        cmd2_index++;
    }
    cmd2[cmd2_index] = NULL;
}

enum pipe_status parser(const char *line, char *tokens[], char *cmd1[], char *cmd2[]){
    if (tokenize(line, tokens) < 0) {
        return PARSE_ERROR;
    }
    int index_of_pipe = check_for_pipe(tokens, 0);
    cmd1[0] = NULL;
    cmd2[0] = NULL;
    if(index_of_pipe > -1){
        enum pipe_status validation = validate_pipe(tokens, index_of_pipe);
        if(validation == PIPE_VALID) split_on_pipe(tokens, index_of_pipe, cmd1, cmd2);
        return validation;
    }
    return NO_PIPE;
}