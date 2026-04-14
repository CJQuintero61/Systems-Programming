/*
    Christian Quintero
    COSC 4348 - Systems Programming
    04/13/2026

    program3.c
    This program will
        1. save its PID to P3.txt
        2. read PIDs from P1.txt and P2.txt
        3. receive a bit from P2
        4. dispaly the received bit

    This program is to learn more about using signals for process
    communication. Communication is only to be done through
    signals and no other means.

    To run:
        make all
        ./program1 & ./program2 & ./program3
        make clean
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "signals.h"

pid_t pid1 = 0;
pid_t pid2 = 0;

void print_bit(int sig);

int main()
{
    // save program3 pid
    save_pid(3, getpid());

    // give time for the others to make their pid file
    sleep(1);

    // register signals
    signal(SIGUSR1, print_bit);
    signal(SIGUSR2, print_bit);
    signal(SIGALRM, print_bit);
    
    // read the pids from process 1 and 2
    pid1 = read_pid(1);
    pid2 = read_pid(2);

    // tell p1 that p3 is ready to accept bits
    kill(pid1, SIGUSR1);

    // wait for signals or for process 1 to terminate process 3
    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;
}

void print_bit(int sig)
{
    /*
        prints a bit depending on the signal received (from process 2)
        0 for SIGUSR1
        1 for SIGUSR2
        a space for SIGALRM

        :params:
        sig: int - the signal received
    */

    // im not sure why, but without flushing stdout here, the 0s and 1s just don't print
    fflush(stdout);

    if (sig == SIGUSR1)
    {
        printf("0");
    }
    else if (sig == SIGUSR2)
    {
        printf("1");
    }
    else if (sig == SIGALRM)
    {
        printf(" ");
    }

    // let p1 know p3 is done printing
    kill(pid1, SIGUSR1);
}