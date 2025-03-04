#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process\n");
        char *args[] = {"ls", "-l", NULL};
        execvp(args[0], args);
    } else {
        // Parent process
        wait(NULL);
        printf("Child process terminated\n");
    }

    return 0;
}
