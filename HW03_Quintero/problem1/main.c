/**
 * Christian Quintero
 * Systems Programming Spring 2026
 * HW3 problem 1
 * March 17, 2026
 * 
 * To run:
 *  make run
 * 
 * This program uses a manual exit status check and the built in C macros
 * to evaluate the exit status of child processes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>

// the number of child processes to make
#define N 6

void My_Exit_Status_Check(pid_t pid, int status)
{
    /**
     * To explain my thought process,
     * the bin array holds the 16 bits of the status variable converted
     * to binary. Indexes [0, 7] are at the most significant part of the string
     * and hold the status value, index 8 is the core dump flag,
     * and indexes [9, 15] hold the signal value.
     * 
     * This can be confusing because the reference ppt slide
     * has bit 0 on the right and bit 15 on the left with bit
     * 7 (moving right to left) being the core dump flag.
     * 
     * The main execution flow is
     *  1. convert the status to a 16 bit binary string
     *  2. check the core dump flag for normal/abnormal termination
     *  3. convert the bits on the proper side of the core dump flag to decimal
     */

    int bin[16] = {0};      // 2 bytes of bits, all initialized to 0
    int r = 0;              // remainder
    int idx = 15;           // to track the bits in the array starting from the least significant bit
    int value = 0;          // to hold the exit status
    int flag = 8;           // the index of the core dump flag in the array
    int is_normal = 0;      // flag value to tell if a processes exited normally or not

    // convert the status to binary
    while (status != 0)
    {
        // validate the array index range before computing things
        if (idx < 0 || idx > 15)
        {
            exit(1);
        }

        r = status % 2; // get remainder
        bin[idx] = r;   // store the bit
        idx = idx - 1;  // move left in the array towards the most significant bit

        status = status / 2;
    }

    // check the core dump flag
    // which is index 8 in the bin array
    // if the core dump flag was set, then some signal termination occurred
    idx = flag;
    if (bin[idx] == 1)
    {
        // add up the bits to get the exit value
        // of the exit status
        value = 0;

        // start at the least significant bit
        idx = 15;

        // moving R->L in the bit string, if a 1 is found
        // convert it to decimal using powers of 2 then
        // add it to the exit status value
        for (int i = 0; i < 7; i++)
        {
            if (bin[idx] == 1)
            {
                value = value + pow(2, i);
            }

            // move to next significant bit
            idx--;
        }

        // set the flag to 0, since the child terminated abnormally
        is_normal = 0;
    }
    // no core dump flag set, so no signal occurred
    else
    {
        // thus we add the bits to the left of the flag
        value = 0;

        // start at least significant bit to the left of the flag
        idx = flag - 1;

        // moving R->L in the bit string, convert to decimal
        // and add
        for (int i = 0; i < 8; i++)
        {
            if (bin[idx] == 1)
            {
                value = value + pow(2, i);
            }

            idx--;
        }

        // set the flag to true because the child exited normally
        is_normal = 1;
    }

    if (is_normal)
    {
        printf("The exit status for child %d using the manual status check was a Normal Termination with exit status: %d\n", pid, value);
    }
    else
    {
        printf("The exit status for child %d using the manual status check was an Abnormal Termination due to signal number: %d\n", pid, value);
    }
    
}

void C_Macros_Exit_Status_Check(pid_t pid, int status)
{
    // much easier

    // if it exited normally, print the exit status
    if (WIFEXITED(status))
    {
        printf("The exit status for child %d using C macros was a Normal Termination with exit status: %d\n", pid, WEXITSTATUS(status));
    }
    // if abnormally terminated due to a signal, print the singal number that caused the termination
    else if (WIFSIGNALED(status))
    {
        printf("The exit status for child %d using C macros was an Abormal Termination due to signal number: %d\n", pid, WTERMSIG(status));
    }
}


int main() 
{
    unsigned int status, i;
    pid_t pid;

    /* Parent creates N children */
    for(i = 0; i < N; i++)                       
		if((pid = fork()) == 0)  /* child */
		{
			if(i == 3)
	    		exit(i/0);
			exit(100 + i);                          
		}
    /* Parent reaps N children in no particular order */
    while ((pid = waitpid(-1, &status, 0)) > 0) 
	{
		My_Exit_Status_Check(pid, status);
		C_Macros_Exit_Status_Check(pid, status);
        printf("\n");
	}

    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)                          
	   perror("waitpid error");

    exit(0);
}