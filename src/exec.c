#include "exec.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int execute(char *tokens[]) {
    if (tokens == NULL || tokens[0] == NULL) {
        return 0;
    }

    char *input_file = NULL;
    char *output_file = NULL;
    char *argv[10];
    int arg_index = 0;

    for (int i = 0; tokens[i] != NULL; ) {
        if (strcmp(tokens[i], "<") == 0) {
            if (input_file != NULL) {
                fprintf(stderr, "redirection error: only one < is supported\n");
                return -1;
            }
            if (tokens[i + 1] == NULL ||
                strcmp(tokens[i + 1], "<") == 0 ||
                strcmp(tokens[i + 1], ">") == 0) {
                fprintf(stderr, "redirection error: missing input file\n");
                return -1;
            }

            input_file = tokens[i + 1];
            i += 2;
        }
        else if (strcmp(tokens[i], ">") == 0) {
            if (output_file != NULL) {
                fprintf(stderr, "redirection error: only one > is supported\n");
                return -1;
            }
            if (tokens[i + 1] == NULL ||
                strcmp(tokens[i + 1], "<") == 0 ||
                strcmp(tokens[i + 1], ">") == 0) {
                fprintf(stderr, "redirection error: missing output file\n");
                return -1;
            }

            output_file = tokens[i + 1];
            i += 2;
        }
        else {
            if (arg_index >= 9) {
                fprintf(stderr, "too many arguments\n");
                return -1;
            }
            argv[arg_index] = tokens[i];
            arg_index++;
            i++;
        }
    }

    argv[arg_index] = NULL;

    if (argv[0] == NULL) {
        fprintf(stderr, "missing command\n");
        return -1;
    }

    int child_process = fork();

    if (child_process < 0) {
        perror("fork");
        return -1;
    }
    else if (child_process == 0) {
        if (input_file != NULL) {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0) {
                perror(input_file);
                _exit(127);
            }

            if (dup2(fd_in, STDIN_FILENO) < 0) {
                perror("dup2");
                close(fd_in);
                _exit(127);
            }

            close(fd_in);
        }

        if (output_file != NULL) {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror(output_file);
                _exit(127);
            }

            if (dup2(fd_out, STDOUT_FILENO) < 0) {
                perror("dup2");
                close(fd_out);
                _exit(127);
            }

            close(fd_out);
        }

        execvp(argv[0], argv);
        perror(argv[0]);
        _exit(127);
    }
    else {
        int status;
        int pid = wait(&status);

        if (pid == -1) {
            perror("wait");
            return -1;
        }
        else if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                return -1;
            }
        }
        else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Child killed by signal %d\n", WTERMSIG(status));
            return -2;
        }
    }

    return 0;
}