/**
 * Christian Quintero
 * February 14, 2026
 * Collatz Conjecture Algorithm
 * COSC 4348 - Systems Programming - Assignment 2
 * 
 * This program uses multiprocessing to implement the Collatz
 * Conjecture Algorithm.
 * 
 * To run:
 * 1. make Collatz_Quintero
 * 2. ./Collatz_Quintero <num processes> <lower bound> <upper bound>
 * Ex) ./Collatz_Quintero 5 10 100
 * 3. make clean
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void validate_arguments(int argc, char** argv);
void create_file();


int main(int argc, char** argv)
{
    validate_arguments(argc, argv);

    int number_of_processes = atoi(argv[1]);
    int wait_status = -1;                       /* wait status for the parent */
    int status = -1;                            /* the status for the child process */
    int exit_code = -1;                         /* the exit code for a child process */
    pid_t child_pid = -1;
    pid_t process_ids[number_of_processes];     /* array to hold all child process ids */

    printf("Creating %d processes:\n", number_of_processes);

    /* create the processes and store their ids */
    for (int i = 0; i < number_of_processes; i++)
    {
        child_pid = fork();
        if (child_pid == -1)
        {
            fprintf(stderr, "Fork failed: there was an error calling fork()\n");
            exit(EXIT_FAILURE);
        }
        else if (child_pid == 0)
        {
            /* child process block */
            
            /* create an output file for this specific child process */
            create_file();

            exit(EXIT_SUCCESS);
        }

        process_ids[i] = child_pid;
    }

    /* make the parent process wait on all child processes */
    for (int i = 0; i < number_of_processes; i++)
    {
        wait_status = waitpid(process_ids[i], &status, 0);
        if (wait_status == -1)
        {
            fprintf(stderr, "Wait failed: the call to waitpid() failed\n");
            exit(EXIT_FAILURE);
        }

        /* check that the child exited normally and that it exited with return code 0*/
        if (WIFEXITED(status))
        {
            exit_code = WEXITSTATUS(status);
            printf("Process %d completed with exit code %d\n", process_ids[i], exit_code);
        }
        else
        {
            fprintf(stderr, "Process failed: the child process for id: %d did not exit normally\n", process_ids[i]);
            exit(EXIT_FAILURE);
        }
    }


    return EXIT_SUCCESS;
}

/**
 * validate the correct number of arguments were entered and check the range
 * on the number of processes to make
 * 
 * argv[0] - program name
 * argv[1] - the number of child processes to make (aka the number of fork calls)
 * argv[2] - the lower bound for the range of numbers
 * argv[3] - the upper bound for the range of numbers
 */
void validate_arguments(int argc, char** argv)
{
    int expected_argc = 4;      /* there must be exactly 4 args          */
    int min_processes = 1;      /* at least 1 child process must be made */
    int max_processes = 10;     /* up to 10 child processes can be made  */

    /* validate argc matches the expected arg count */
    if (argc != expected_argc)
    {
        fprintf(stderr, "Invalid argument count: expected 4 arguments, got %d\n", argc);
        exit(EXIT_FAILURE);
    }
    
    /* convert args to ints */
    int number_of_processes = atoi(argv[1]);
    int lower_bound = atoi(argv[2]);
    int upper_bound = atoi(argv[3]);

    /* validate the range of number of processes to make */
    if (!(number_of_processes >= min_processes && number_of_processes <= max_processes))
    {
        fprintf(stderr, "Invalid argument: number of child processes to make must be between %d-%d, got %d\n",
            min_processes, max_processes, number_of_processes);
        exit(EXIT_FAILURE);
    }

    /* validate the bounds */
    if (lower_bound <= 0)
    {
        fprintf(stderr, "Invalid argument: lower bound must be at least 1, got %d\n", lower_bound);
        exit(EXIT_FAILURE);
    }

    if (!(upper_bound > lower_bound))
    {
        fprintf(stderr, "Invalid argument: upper bound must be greater than lower bound\nupper bound: %d, lower bound: %d\n", upper_bound, lower_bound);
        exit(EXIT_FAILURE);
    }
}

/**
 * This function is only called by child processes to make their
 * own file to write their output to. The number of files made
 * will be equal to the number of child processes made.
 * Ex) 10 processes made will make 10 output files
 */
void create_file()
{
    /* format the output file name using snprintf to format properly */
    char filename[256];
    snprintf(filename, sizeof(filename), "results_%d_%d.dat", getpid(), getppid());

    /**
     * this has a lot to unpack so I felt I should specify the args
     * and for my future reference
     * 
     * filename - the file to open
     * O_CREAT - create the file if it doesn't exist
     * O_WRONLY - open in write only mode (we don't need to read anything in this program)
     * O_TRUNC - truncate/delete any data on the file. This is useful on
     *           multiple runs without having to run `make clean` every time.
     * S_IRWXU - when O_CREAT is used, you MUST provide the file permissions for
     *           it. This sets ownership to the current user with Read/Write/Execute permissions
     */
    int fildes = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

    if (fildes == -1)
    {
        fprintf(stderr, "File error: there was an error creating or opening the file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if(close(fildes) == -1)
    {
        fprintf(stderr, "File error: there was an error closing the file %s\n", filename);
        exit(EXIT_FAILURE);
    }
}