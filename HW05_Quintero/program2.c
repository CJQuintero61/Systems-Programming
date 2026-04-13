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
        ./program1 & ./program2 & ./program3
        make clean
*/
#include <stdio.h>
#include <stdlib.h>
#include "signals.h"

#define IDX 2   // program2 has a process index of 2

int main()
{
    pid_t pid = getpid();
    pid_t pids[2];          // array to hold the other pids

    // save program2 pid
    save_pid(IDX, pid);
    sleep(1);

    // read the pids from process 1 and 3
    pids[0] = read_pid(1);
    pids[1] = read_pid(3);

    return EXIT_SUCCESS;
}