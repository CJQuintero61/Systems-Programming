// a more formal program for handling forks
#include <stdio.h>
#include <unistd.h>

int main() {
    // a variable to hold the child's process id
    pid_t child_pid = -1;

    // fork will return -1 if fork failed, and 0 otherwise
    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed!\n");
        return 1;
    }
    else if (child_pid == 0) {
        // child process code
        printf("I am a child process and my PID is: %d\n", getpid());
    }
    else {
        // parent process code
        printf("I am a parent process and my PID is: %d\n", getpid());
    }
    return 0;
}