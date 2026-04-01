#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int execute(char *tokens[]){
    if(tokens == NULL || tokens[0] == NULL) return 1;
    int child_process = fork();
    if(child_process < 0){
        fprintf(stderr, "Fork failed\n");
        return -1;
    }
    else if (child_process == 0){
        execvp(tokens[0], tokens);

        //needs error handling why this fails
        exit(1);
    }
    else {
        int status;
        int pid = wait(&status);
        if (pid == -1) {
            perror("wait");
            return -1;
        } else if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                fprintf(stderr, "Child exited with error code %d\n", WEXITSTATUS(status));
                return -1;
            }
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Child killed by signal %d\n", WTERMSIG(status));
            return -2;
        }
    }
    return 0;
}


