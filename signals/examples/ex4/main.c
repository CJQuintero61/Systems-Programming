#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// one hanlder for both signals
static void sig_usr(int signo)
{
    if (signo == SIGUSR1)
        printf("Received SIGUSR1\n");
    else
    {
        if (signo == SIGUSR2)
            printf("Received SIGUSR2\n");
        else    printf("Received signal %d\n", signo);
    }
}

int main()
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        printf("Can't catch SIGUSR1");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
     printf("Can't catch SIGUSR2\n");

    raise(SIGUSR1);
}