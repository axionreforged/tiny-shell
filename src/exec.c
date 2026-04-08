#include "exec.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

static int prepare_for_execution(char *tokens[], char *argv[], char **input_file, char **output_file) {
    int arg_index = 0;

    for (int i = 0; tokens[i] != NULL; ) {
        if (strcmp(tokens[i], "<") == 0) {
            if (*input_file != NULL) {
                fprintf(stderr, "redirection error: only one < is supported\n");
                return -1;
            }
            if (tokens[i + 1] == NULL ||
                strcmp(tokens[i + 1], "<") == 0 ||
                strcmp(tokens[i + 1], ">") == 0 ||
                strcmp(tokens[i + 1], "|") == 0) {
                fprintf(stderr, "redirection error: missing input file\n");
                return -1;
            }

            *input_file = tokens[i + 1];
            i += 2;
        }
        else if (strcmp(tokens[i], ">") == 0) {
            if (*output_file != NULL) {
                fprintf(stderr, "redirection error: only one > is supported\n");
                return -1;
            }
            if (tokens[i + 1] == NULL ||
                strcmp(tokens[i + 1], "<") == 0 ||
                strcmp(tokens[i + 1], ">") == 0 ||
                strcmp(tokens[i + 1], "|") == 0) {
                fprintf(stderr, "redirection error: missing output file\n");
                return -1;
            }

            *output_file = tokens[i + 1];
            i += 2;
        }
        else if (strcmp(tokens[i], "|") == 0) {
            fprintf(stderr, "pipe error: unexpected |\n");
            return -1;
        }
        else {
            if (arg_index >= MAX_TOKENS - 1) {
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

    return 0;
}

static void run_command_in_child(char *tokens[], int inherited_stdin, int inherited_stdout) {
    char *argv[MAX_TOKENS];
    char *input_file = NULL;
    char *output_file = NULL;

    int prep_status = prepare_for_execution(tokens, argv, &input_file, &output_file);
    if (prep_status < 0) {
        _exit(127);
    }

    if (inherited_stdin >= 0) {
        if (dup2(inherited_stdin, STDIN_FILENO) < 0) {
            perror("dup2");
            if (inherited_stdin != STDIN_FILENO) {
                close(inherited_stdin);
            }
            _exit(127);
        }
    }

    if (inherited_stdout >= 0) {
        if (dup2(inherited_stdout, STDOUT_FILENO) < 0) {
            perror("dup2");
            if (inherited_stdout != STDOUT_FILENO) {
                close(inherited_stdout);
            }
            _exit(127);
        }
    }

    if (inherited_stdin >= 0 && inherited_stdin != STDIN_FILENO) {
        close(inherited_stdin);
    }
    if (inherited_stdout >= 0 && inherited_stdout != STDOUT_FILENO) {
        close(inherited_stdout);
    }

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

static int wait_for_child(pid_t child_pid) {
    int status;
    pid_t pid = waitpid(child_pid, &status, 0);

    if (pid == -1) {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) != 0) {
            return -1;
        }
    }
    else if (WIFSIGNALED(status)) {
        fprintf(stderr, "Child killed by signal %d\n", WTERMSIG(status));
        return -2;
    }

    return 0;
}

int execute_single_command(char *tokens[]) {
    int child_process = fork();

    if (child_process < 0) {
        perror("fork");
        return -1;
    }
    else if (child_process == 0) {
        run_command_in_child(tokens, -1, -1);
    }

    return wait_for_child(child_process);
}

int execute_pipe(char *cmd1[], char *cmd2[]) {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    pid_t left_pid = fork();
    if (left_pid < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }
    else if (left_pid == 0) {
        close(pipefd[0]);
        run_command_in_child(cmd1, -1, pipefd[1]);
    }

    pid_t right_pid = fork();
    if (right_pid < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        wait_for_child(left_pid);
        return -1;
    }
    else if (right_pid == 0) {
        close(pipefd[1]);
        run_command_in_child(cmd2, pipefd[0], -1);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    int left_status = wait_for_child(left_pid);
    int right_status = wait_for_child(right_pid);

    if (left_status < 0) {
        return left_status;
    }
    if (right_status < 0) {
        return right_status;
    }

    return 0;
}

int execute(char *tokens[], enum pipe_status status, char *cmd1[], char *cmd2[]) {
    if (tokens == NULL || tokens[0] == NULL) {
        return 0;
    }

    if (status == NO_PIPE) {
        return execute_single_command(tokens);
    }

    return execute_pipe(cmd1, cmd2);
}