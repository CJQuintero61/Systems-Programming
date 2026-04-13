/*
    Christian Quintero
    COSC 4348 - Systems Programming
    04/13/2026

    program1.c
    This program will
        1. save its PID to P1.txt
        2. read PIDs from P2.txt and P3.txt
        3. read the message from message.txt
        4. send the message to P2 one bit at a time using signals
        5. terminte program2 and 3 once completed

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
    int process_idx = 1;    // program1 has a process index of 1
    pid_t pid = getpid();

    save_pid(1, pid);

    return EXIT_SUCCESS;
}