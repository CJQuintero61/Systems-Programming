/**
 * Christian Quintero
 * February 14, 2026
 * Collatz Conjecture Algorithm
 * COSC 4348 - Systems Programming - Assignment 2
 * 
 * This program uses mutlithreading to implement the Collatz
 * Conjecture Algorithm.
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void validate_arguments(int argc, char** argv);

int main(int argc, char** argv)
{
    validate_arguments(argc, argv);

    int number_of_processes = atoi(argv[1]);
    pid_t child_process = -1;
    pid_t process_ids[number_of_processes];     /* array to hold all child process ids */

    for (int i = 0; i < number_of_processes; i++)
    {
        child_process = fork();
        if (child_process == -1)
        {
            printf("Fork failed: there was an error calling fork()\n");
            exit(EXIT_FAILURE);
        }
        else if (child_process == 0)
        {
            /* prevent the child processes from calling fork */
            break;
        }

        process_ids[i] = child_process;
    }

    return 0;
}

/**
 * validate the correct number of arguments were entered and check the range
 * on the number of processes to make
 * 
 * argv[0] - program name
 * argv[1] - the number of child processes to make (aka the number of fork calls)
 * argv[2] - the lower bound for the range of numbers to choose from
 * argv[3] - the upper bound for the range of numbers to choose from
 */
void validate_arguments(int argc, char** argv)
{
    int expected_argc = 4;      /* there must be exactly 4 args          */
    int min_processes = 1;      /* at least 1 child process must be made */
    int max_processes = 10;     /* up to 10 child processes can be made  */

    /* validate argc matches the expected arg count */
    if (argc != expected_argc)
    {
        printf("Invalid argument count: expected 4 arguments, got %d\n", argc);
        exit(EXIT_FAILURE);
    }
    
    /* convert args to ints */
    int number_of_processes = atoi(argv[1]);
    int lower_bound = atoi(argv[2]);
    int upper_bound = atoi(argv[3]);

    /* validate the range of number of processes to make */
    if (!(number_of_processes >= min_processes && number_of_processes <= max_processes))
    {
        printf("Invalid argument: number of child processes to make must be between %d-%d, got %d\n",
            min_processes, max_processes, number_of_processes);
        exit(EXIT_FAILURE);
    }

    /* validate the bounds */
    if (lower_bound <= 0)
    {
        printf("Invalid argument: lower bound must be at least 1, got %d\n", lower_bound);
        exit(EXIT_FAILURE);
    }

    if (upper_bound <= 0)
    {
        printf("Invalid argument: upper bound must be at least 1, got %d\n", lower_bound);
        exit(EXIT_FAILURE);
    }

    if (upper_bound < lower_bound)
    {
        printf("Invalid argument: upper bound must be greater than lower bound.\n");
        printf("upper bound: %d, lower bound: %d\n", upper_bound, lower_bound);
        exit(EXIT_FAILURE);
    }
}