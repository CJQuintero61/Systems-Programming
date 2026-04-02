/*
    Christian Quintero
    04/01/2026
    Systems Programming - Homework 4
    copy.c

    Implementation file for copy.h
*/
#include <stdio.h>
#include <unistd.h>
#include "copy.h"

void validate_args(int argc)
{
    /*
        This function validates the argument count only.
        The arg vector with source and destination directories are validated
        when trying to open those locations.

        :params:
        argc: int - the argument count
    */

    if (argc != 3)
    {
        fprintf(stderr, "Invalid argument count: expected 3, got %d\n", argc);
        fprintf(stderr, "The arguments should be: ./main <source_directory> <destination_directory>\n");
        exit(1);
    }
}

void create_child_processes(pid_t* pids)
{
    /*
        This function creates 2 child processes as required by the assignemnt
        directions.

        :params
        pids: pid_t* - a pointer to the pids array in main
    */
    int child_pid = -1;

    for (int i = 0; i < CHILDREN; i++)
    {
        child_pid = fork();

        if (child_pid == 0)
        {
            // child block
            // prevent children from calling fork
            break;
        }
        else if (child_pid == -1)
        {
            perror("Fork call failed\n");
            exit(1);
        }
        else
        {
            // parent block
            // the parent stores the child pids
            pids[i] = child_pid;
        }
    }
}