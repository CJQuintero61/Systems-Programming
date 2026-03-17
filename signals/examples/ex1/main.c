#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int main()
{
    pid_t pid;

    // child sleeps until SIGKILL signal received, then dies
    if ((pid = fork()) == 0)
    {
        pause(); // wait for signal to arrive
        printf("Control should never reach here!\n");
        exit(1);
    }

    // parent sends a SIGKILL signal to a child
    kill(pid, SIGKILL);
    printf("Child process is terminated!\n");
    exit(0);
}