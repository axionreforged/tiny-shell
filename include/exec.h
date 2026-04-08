#ifndef EXEC_H
#define EXEC_H

#include "tokenize.h"
int execute(char *tokens[], enum pipe_status status, char *cmd1[], char *cmd2[]);

#endif