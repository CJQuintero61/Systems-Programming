#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
    static int countdown = 10;
    printf("%d\n", countdown);
    if(--countdown > 0)
        alarm(1);   // next SIGALRM will be delivered in 1 second
    else
    {
        printf("Happy New Year!\n");
        exit(0);
    }
}

int main()
{
    signal(SIGALRM, handler); // install SIGALRM handler
    alarm(1); // next SIGALRM will be delivered in 1s

    while(1)
    {
        ;   // signal handler returns control here each time
    }

    exit(0);
}