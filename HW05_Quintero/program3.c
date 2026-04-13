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
#include "signals.h"

int main()
{
    int process_idx = 3;    // program3 has a process index of 3
    pid_t pid = getpid();

    save_pid(3, pid);

    return EXIT_SUCCESS;
}