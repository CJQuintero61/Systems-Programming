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
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "signals.h"

#define MAX_MESSAGE 256     // the maximum number of characters able to be stored in the message buffer
pid_t pid2 = 0;
pid_t pid3 = 0;

void read_message(char msg[], int size);
void send_message(char msg[]);
void wait_for_signal(int sig);

int main()
{
    // save program1 pid
    save_pid(1, getpid());

    // give time for the others to make their pid file
    sleep(1);

    // register signal
    signal(SIGUSR1, wait_for_signal);

    // read the pids from process 2 and 3
    pid2 = read_pid(2);
    pid3 = read_pid(3);

    // process1 reads the message file
    char msg[MAX_MESSAGE];
    read_message(msg, sizeof(msg));

    // wait for p2 and p3 to signal they are ready
    pause();

    // send the message to pid2
    send_message(msg);

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

void send_message(char msg[])
{
    /*
        process 1 only sends messages to process 2

        :params:
        msg: char[] - the message of 0s and 1s in an array
    */
    int i = 0;
    while (msg[i] != '\0')
    {
        if (msg[i] == '0')
        {
            // send a 0 to process 2
            kill(pid2, SIGUSR1);

            // wait for p3 to finish printing
            pause();
        }
        else if (msg[i] == '1')
        {
            // send a 1 to process 2
            kill(pid2, SIGUSR2);

            // wait for p3 to finish printing
            pause();
        }

        // skip spaces
        i++;
    }

    sleep(1);

    // terminate process 2 and 3
    kill(pid2, SIGTERM);
    kill(pid3, SIGTERM);

}

void wait_for_signal(int sig)
{
    // do nothing
}