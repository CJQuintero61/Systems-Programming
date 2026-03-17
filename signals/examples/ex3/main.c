// this does nothing until you press Ctrl+C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void handler(int sig)
{
    printf("\nCaught SIGINT\n");
    exit(0);
}

int main()
{
    // install the SIGINT handler
    if (signal(SIGINT, handler) == SIG_ERR)
        perror("signal error");

    pause();

    // wait for the receipt of a signal
    exit(0);
}