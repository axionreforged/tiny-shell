#ifndef TOKENIZE_H
#define TOKENIZE_H

#include "config.h"

int tokenize(const char* line, char *tokens[]);
void print_tokens(char *tokens[]);
void free_memory_of_tokens(char *tokens[]);
enum pipe_status{NO_LEFT_COMMAND, NO_RIGHT_COMMAND, MORE_THAN_ONE_PIPE, PIPE_VALID, NO_PIPE, PARSE_ERROR};
enum pipe_status parser(const char *line, char *tokens[], char *cmd1[], char *cmd2[]);

#endif