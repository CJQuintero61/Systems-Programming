/*
    Christian Quintero
    COSC 4348 - Systems Programming
    04/13/2026

    implementation file for signals.c
    This file contains common functions used by all 3 programs such as
    writing thier PID to a file and reading the other processes' PIDs.

    Code that is specific to a single process should be written
    in that processes file.
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include "signals.h"

void save_pid(int process_idx, pid_t pid)
{
    /*
        this function saves a processes pid to a file, creating it
        if it doesn't exist and truncating it if it does

        :params:
        process_idx: int - the process index such as 1, 2, or 3 to create the file as P1, P2, or P3
        pid: pid_t - this process's pid to write to the file
    */
    char file[MAX_PATH];
    char pid_buffer[32];
    int fd;
    int len;

    // format the file name as P1, P2, or P3 and check for errors
    if ((snprintf(file, sizeof(file), "P%d.txt", process_idx)) < 0)
    {
        perror("snprintf");
        exit(EXIT_FAILURE);
    }

    // get file descriptor for this file
    if ((fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU)) == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // format the pid into a string to be able to write it to a file
    if ((len = snprintf(pid_buffer, sizeof(pid_buffer), "%d\n", pid)) < 0)
    {
        perror("snprintf");
        exit(EXIT_FAILURE);
    }

    // write the pid to a file
    if ((write(fd, pid_buffer, len)) != len)
    {
        perror("write");
        // close the file after failing to write, and check close() for errors
        if ((close(fd)) != 0)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }

    // close fd
    if ((close(fd)) != 0)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

pid_t read_pid(int process_idx)
{
    /*
        this function reads a pid from a file using the passed
        process index
        Ex) a process calls this with 2, so it will open P2.txt and read
        process 2's pid from it and return it to the calling process

        :params:
        process_idx: int - the process index to use for reading and returning

        :returns:
        pid_t - the pid for that process index
    */
    char file[MAX_PATH];
    char pid_buffer[32];
    int fd;
    int len;

    // format the file name as P1, P2, or P3 and check for errors
    if ((snprintf(file, sizeof(file), "P%d.txt", process_idx)) < 0)
    {
        perror("snprintf");
        exit(EXIT_FAILURE);
    }

    // get file descriptor for this file
    if ((fd = open(file, O_RDONLY)) == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // read from the fd
    if ((len = read(fd, pid_buffer, sizeof(pid_buffer) - 1)) <= 0)
    {
        perror("read");
        if ((close(fd)) != 0)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }

    // null terminate the string
    pid_buffer[len] = '\0';

    // close fd
    if ((close(fd)) != 0)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }

    // convert the string to a pid_t type
    return (pid_t) atoi(pid_buffer);
}