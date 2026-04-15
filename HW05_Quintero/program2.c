/*
    Christian Quintero
    COSC 4348 - Systems Programming
    04/13/2026

    program2.c
    This program will
        1. save its PID to P2.txt
        2. read PIDs from P1.txt and P3.txt
        3. receive a bit from P1
        4. send a bit to P3

    This program is to learn more about using signals for process
    communication. Communication is only to be done through
    signals and no other means.

    To run:
        make all
        ./program1 & ./program2 & ./program3 >> output.txt
        make clean
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "signals.h"

pid_t pid1 = 0;
pid_t pid3 = 0;

void forward_bit(int sig);

int main()
{
    // save program2 pid
    save_pid(2, getpid());

    // give time for the others to make their pid file
    sleep(1);

    // register signals
    signal(SIGUSR1, forward_bit);
    signal(SIGUSR2, forward_bit);
    signal(SIGALRM, forward_bit);

    // read the pids from process 1 and 3
    pid1 = read_pid(1);
    pid3 = read_pid(3);

    // tell p1 that p2 is ready to accept bits
    kill(pid1, SIGUSR1);

    // wait for signals or for process 1 to terminate process 2
    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;
}

void forward_bit(int sig)
{
    /*
        this function is used to forward the signal/bit from
        process 1 to process 3

        :params:
        sig: int - the signal from process 1.
    */

    // forward the signal to process 3
    kill(pid3, sig);
}