/*
    Christian Quintero
    COSC 4348 - Systems Programming
    04/13/2026

    header file for signals.c
    This file contains common functions used by all 3 programs such as
    writing thier PID to a file and reading the other processes' PIDs.

    Code that is specific to a single process should be written
    in that processes file.
*/
#include <unistd.h>

/*
    constants
    MAX_PATH: 256 - the maximum file path able to be held in a buffer
*/
#define MAX_PATH 256

void save_pid(int process_idx, pid_t pid);
pid_t read_pid(int process_idx);
