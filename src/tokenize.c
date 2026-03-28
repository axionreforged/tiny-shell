#include "input.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void tokenize(const char* line, char *tokens[]){
    char normalized_line[100];
    normalize(line, normalized_line);
    char c = normalized_line[0];
    int line_index = 0;
    int token_index = -1;
    int word_index = 0;
    bool new_word = true;
    while(c != '\0'){
        if(c == ' ') new_word = true;
        else{
            if(new_word){
                word_index = 0;
                token_index++;
                tokens[token_index] = malloc(11);
            }
            new_word = false;
            tokens[token_index][word_index] = c;
            word_index++;
            tokens[token_index][word_index] = '\0';
        }
        line_index++;
        c = normalized_line[line_index];
    }
    tokens[token_index + 1] = NULL;
}

void print_tokens(char *tokens[]) {
    int i = 0;

    while (tokens[i] != NULL) {
        printf("tokens[%d] = %s\n", i, tokens[i]);
        i++;
    }
}