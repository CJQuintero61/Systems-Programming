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
#include <fcntl.h>
#include "signals.h"

#define IDX 1               // program1 has a process index of 1
#define MAX_MESSAGE 256     // the maximum number of characters able to be stored in the message buffer

void read_message(char msg[], int size);
void send_message(pid_t pid, char msg[]);

int main()
{
    // save program1 pid
    save_pid(IDX, getpid());

    // read the pids from process 2 and 3
    pid_t pids[2];
    pids[0] = read_pid(2);
    pids[1] = read_pid(3);

    // process1 reads the message file
    char msg[MAX_MESSAGE];
    read_message(msg, sizeof(msg));

    // send the message
    send_message(pids[0], msg);

    return EXIT_SUCCESS;
}

void read_message(char msg[], int size)
{
    /*
        reads the message from message.txt and stores it in a buffer

        msg: char [] - the array to store the individual characters 0s and 1s from the message.txt file
        size: int - the size of the msg array
    */
    char* file = "message.txt";
    char buf[MAX_MESSAGE];
    int fd;
    int len;

    // open the message file
    if ((fd = open(file, O_RDONLY)) == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // read from the fd
    if ((len = read(fd, msg, size - 1)) <= 0)
    {
        perror("read");
        if ((close(fd)) != 0)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }

    // terminate the string
    msg[len] = '\0';

    // close fd
    if ((close(fd)) != 0)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

void send_message(pid_t pid, char msg[])
{
    
}