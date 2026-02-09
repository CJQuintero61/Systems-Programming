/*
    Christian Quintero
    02/05/2026
    Systems Programming Practice

    This program uses exec to have a child process
    run its own shell and run "ls -lah"
    while the parent waits.
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    int status = -1;
    pid_t child_pid = -1;

    child_pid = fork();
    if(child_pid == -1) {
        printf("Fork failed.\n");
        return -1;
    }
    else if(child_pid == 0) {
        // child block

        // execl MUST be ended with NULL after all arguments
        // and execl only returns if it fails so we
        // can catch failures right after the execl command

        // note that execl REPLACES the current child's process 
        // with that new process. It will not create a new process.
        // In other words, the child process IS the bash shell
        execl("/bin/bash", "bash", "-c", "ls -lah", NULL);
        printf("I am the child process and my exec failed.\n");
        return -1;
    }
    else {
        // parent block
        waitpid(child_pid, &status, 0);

        if(WIFEXITED(status)) {
            printf("The child process exited normally!\n");
        }
        else {
            printf("The child process did not exit normally.\n");
        }
    }

    return 0;
}
