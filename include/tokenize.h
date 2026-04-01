#ifndef TOKENIZE_H
#define TOKENIZE_H

void tokenize(const char* line, char *tokens[]);
void print_tokens(char *tokens[]);
void free_memory_of_tokens(char *tokens[]);

#endif