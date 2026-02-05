// Christian Quintero
// 02/05/2026
// Systems Programming Practice
//
// this program uses waitpid() to make the parent
// process wait for the child process to terminate
//
// The child process will count while the parent waits
// for it to return then the parent returns
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

// prototypes
void run_child_process();

int main() {
    int status = -1;
    pid_t child_pid = -1;
    child_pid = fork();

    // verify the fork worked
    if(child_pid == -1) {
        printf("Failed to fork.\n");
        return -1;
    }
    else if(child_pid == 0) {
        // child block
        run_child_process();
        // signal the child process has ended successfully
        printf("I am the child process and I am returning now!\n");
        return 0;
    }
    else {
        // parent block
        printf("I am the parent process and I am waiting.\n");
        
        // child_pid - the child process' pid
        // &status - the exit code of the child process
        // 0 - block the parent until this specific child terminates
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

void run_child_process() {
    // the function has the child process
    // count to the limit then print 
    // what it counted to

    int limit = 100000;
    for(int i = 0; i < limit; i++) {
        if (i == limit - 1) {
            printf("Finished counting to %d\n", (i + 1));
        }
    }
}