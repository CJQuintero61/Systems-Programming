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
int ack_count = 0;

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
    int bit_count = 0;

    // reset the acknowledgement count before sending signals
    ack_count = 0; 
    
    // parse the message
    while (msg[i] != '\0')
    {
        // send a 0 to p2
        if (msg[i] == '0')
        {
            kill(pid2, SIGUSR1);
        }
        // send a 1 to p2
        else if (msg[i] == '1')
        {
            kill(pid2, SIGUSR2);
        }
        // I used SIGALRM as the signal to send on spaces
        else if (msg[i] == ' ')
        {
            kill(pid2, SIGALRM);
        }

        // increment bit count to ensure we pause the same number
        // of times we send a signal
        bit_count++;

        // wait for acknowledgement from p3
        pause();

        i++;
    }

    // wait for all acknowledgements before killing p2 and p3
    while (ack_count < bit_count)
    {
        // keep waiting for remaining signals
        pause();  
    }

    // terminate process 2 and 3
    kill(pid2, SIGTERM);
    kill(pid3, SIGTERM);

}

void wait_for_signal(int sig)
{
    /*
        this is the handler function for when p1 is signaled
        with SIGUSR1

        the ack_count is incremented everytime p1 is signaled to track
        the amount of acknowledgements we received to compare with
        the number of bits being sent

        this needs to be reset before sending any bits because
        the acknowledgements that p2 and p3 are ready will increment
        this

        without doing the ack_count < bit_count while loop in send_message(),
        p1 would terminate p2 and p3 before printing all the bits.
    */
    ack_count++;
}