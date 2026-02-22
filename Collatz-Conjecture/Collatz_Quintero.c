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

int validate_arguments(int argc, char** argv);
int create_file();
int run_child_processes(int my_index, int process_count, int range_start, int range_end);
int calc_process_index(int process_count);
int calc_two_thirds_range(int total_range);
int run_parent_process(pid_t pids[], int process_count);
void compute_sequence();


int main(int argc, char** argv)
{
    /* validate all args */
    if (validate_arguments(argc, argv) != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    int process_count = atoi(argv[1]);
    int range_start = atoi(argv[2]);
    int range_end = atoi(argv[3]);
    int my_index = -1;
    pid_t child_pid = -1;
    pid_t process_ids[process_count];

    printf("Creating %d processes:\n", process_count);

    /* create the processes and store their ids */
    for (int i = 0; i < process_count; i++)
    {
        child_pid = fork();
        if (child_pid == -1)
        {
            fprintf(stderr, "Fork failed: there was an error calling fork()\n");
            return EXIT_FAILURE;
        }
        else if (child_pid == 0)
        {
            /* prevent children from calling fork */

            /* store the index for each child */
            my_index = i;

            /* run the child processes */
            if (run_child_processes(my_index, process_count, range_start, range_end) != 0)
            {
                return EXIT_FAILURE;
            }
            else
            {
                return EXIT_SUCCESS;
            }
        }
        else
        {
            /* parent block */
            process_ids[i] = child_pid;
        }
    }

    if (run_parent_process(process_ids, process_count) != 0)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

/**
 * validate the correct number of arguments were entered and check the range
 * on the number of processes to make
 * 
 * returns: int - EXIT_SUCESS (0) if all arguments are valid
 *                EXIT_FAILURE (1) if any of the arguments are invalid
 * 
 * argv[0] - program name
 * argv[1] - the number of child processes to make (aka the number of fork calls)
 * argv[2] - the lower bound for the range of numbers
 * argv[3] - the upper bound for the range of numbers
 */
int validate_arguments(int argc, char** argv)
{
    int expected_argc = 4;      /* there must be exactly 4 args          */
    int min_processes = 1;      /* at least 1 child process must be made */
    int max_processes = 10;     /* up to 10 child processes can be made  */

    /* validate argc matches the expected arg count */
    if (argc != expected_argc)
    {
        fprintf(stderr, "Invalid argument count: expected 4 arguments, got %d\n", argc);
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    /* validate the bounds */
    if (lower_bound <= 0)
    {
        fprintf(stderr, "Invalid argument: lower bound must be at least 1, got %d\n", lower_bound);
        return EXIT_FAILURE;
    }

    if (!(upper_bound > lower_bound))
    {
        fprintf(stderr, "Invalid argument: upper bound must be greater than lower bound\nupper bound: %d, lower bound: %d\n", upper_bound, lower_bound);
        return EXIT_FAILURE;
    }

    /* all args are valid */
    return EXIT_SUCCESS;
}

/**
 * This function is only called by child processes to make their
 * own file to write their output to. The number of files made
 * will be equal to the number of child processes made.
 * Ex) 10 processes made will make 10 output files
 * 
 * returns int - EXIT_SUCCESS (0) if there are no erorrs, else EXIT_FAILURE (1)
 */
int create_file()
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
        return EXIT_FAILURE;
    }

    if(close(fildes) == -1)
    {
        fprintf(stderr, "File error: there was an error closing the file %s\n", filename);
        return EXIT_FAILURE;
    }

    /* return 0 if there were no errors */
    return EXIT_SUCCESS;
}

/**
 * This function holds the control blocks that are responsible for
 * the child processes and the parent process.
 * 
 * params:
 * pids[]: pid_t - an array with all child process pids
 * child_pid: pid_t - the return code from calling fork(). Used to separate the parent from the children.
 * my_index: int - the index in the array for this child
 * process_count: int - the number of processes made
 * range_start: int - the first number of the range to compute a sequence for
 * range_end: int - the last number to compute the sequence for
 * 
 * returns: int - 0 on success, 1 on failures and errors
 */
int run_child_processes(int my_index, int process_count, int range_start, int range_end)
{
    int total_range = range_end - range_start + 1;  /* total # of different sequences that need to be calculated */
    int first_half_index = calc_process_index(process_count);   /* index of the last process in the first half */
    int two_thirds = calc_two_thirds_range(total_range);        /* the first 2/3rds of values in the range */
    int one_third = total_range - two_thirds;                   /* the last 1/3rd of values in the range */
    int first_half_count = first_half_index + 1;                /* the actual number of processes in the first half */
    int second_half_count = process_count - first_half_count;   /* the actual number of processes in the second half */

    /* to track the first and last value of the range of sequences a process will compute */
    int start = -1;
    int end = -1;

    /* the number of sequences each process will compute in their block */
    int first_block = -1;
    int second_block = -1;

    /* needed to 0 base within the second half of processes */
    int local_index = -1;   

    /* for children that will compute the first 2/3rds of sequences */
    if (my_index <= first_half_index)
    {
        first_block = two_thirds / (first_half_index + 1);                  /* number of sequences per process */
        start = range_start + (my_index * first_block);                     /* first number of sequence */
        end = start + first_block - 1;                                      /* last number of sequence */
        printf("I will compute %d to %d\n", start, end);
    }
    /* for children that will compute the last 1/3rd of sequences */
    else
    {
        local_index  = my_index - first_half_count;                         /* to 0 base index in the second half */
        second_block = one_third / second_half_count;                       /* the number of sequences per process */
        start = range_start + two_thirds + (local_index * second_block);    /* starting value of sequence */
        end   = start + second_block - 1;                                   /* last value of sequence */

        printf("I will compute %d to %d\n", start, end);
    }

    /* all children create their own output file and compute their own sequences */
    if (create_file() != 0)
    {
        /* catch any failures to create, open, write, and close the files */
        return EXIT_FAILURE;
    }
    else
    {
        /* no errors creating, opening, writing, or closing the files */
        //printf("I am child %d and I am computing the sequence for %d to %d\n", getpid(), start, end);
        return EXIT_SUCCESS;
    }

}

/**
 * calculates the index of the last process tht will compute 2/3rds of the 
 * sequences
 * Ex) for 10 processes, we return 4 because processes 0, 1, 2, 3, 4 make the lower half
 * and 5 - 9 make the upper half, so 4 is the index of the last process in the first half.
 * 
 * Ex) for 9 processes, we return 4 because processes 0, 1, 2, 3, 4 make the lower half
 * and 5 - 8 make the upper half so again 4 is the index of the last process in the first half.
 * In odd cases, the extra process goes to the first half to compute 2/3rds.
 * 
 * param process_count: int - the total number of child processes
 * 
 * returns int - the index of the last that will compute 2/3rds of the sequences
 */
int calc_process_index(int process_count)
{
    /* for even # of processes, divide by 2 and subtract 1 due to 0 based indexing */
    if (process_count % 2 == 0)
    {
        return ((process_count / 2) - 1);
    }
    /* for odd # of processes, due to truncation we can leave it as is */
    else
    {
        return (process_count / 2);
    }
}

/**
 * calculates the stopping value for the numbers in the 
 * first 2/3rds of the range.
 * 
 * Ex) for 30 different sequences to compute, we return 20.
 * For 31 sequences, return 21.
 * For 32 sequences, return 22.
 * 
 * param total_range: int - the total number of different sequences
 * 
 * returns int - the stopping value for the 2/3rds of the range
 */
int calc_two_thirds_range(int total_range)
{
    if (total_range % 3 == 0)
    {
        return 2 * (total_range / 3);
    }
    else if (total_range % 3 == 1)
    {
        return (2 * (total_range / 3)) + 1;
    }
    else
    {
        return (2 * (total_range / 3)) + 2;
    }
}

int run_parent_process(pid_t pids[], int process_count)
{

}