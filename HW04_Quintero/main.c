/*
    Christian Quintero
    04/01/2026
    Systems Programming - Homework 4

    to run:
        1. make main
        2. ./main <source_directory> <destination_directory>
        3. make clean

    This program copies the contents of one directory to another directory using
    child processes.
*/
#include "copy.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
    pid_t pids[CHILDREN];

    validate_args(argc);
    create_child_processes(pids);
    return 0;
}