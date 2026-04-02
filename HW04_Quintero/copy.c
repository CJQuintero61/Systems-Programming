/*
    Christian Quintero
    04/01/2026
    Systems Programming - Homework 4
    copy.c

    Implementation file for copy.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include "copy.h"

void validate_args(int argc, char *argv[])
{
    /*
        This function validates the argument count and
        ensures the arg vector strings aren't empty.

        :params:
        argc: int - the argument count
        argv: char *[] - the argument vector full of strings
    */

    if (argc != 3)
    {
        fprintf(stderr, "Invalid argument count: expected 3, got %d\n", argc);
        fprintf(stderr, "The arguments should be: ./main <source_directory> <destination_directory>\n");
        exit(1);
    }

    if (argv[1] == NULL || strlen(argv[1]) == 0)
    {
        fprintf(stderr, "Invalid argument: the source path argv[1] is an empty string\n");
        exit(1);
    }

    if (argv[2] == NULL || strlen(argv[2]) == 0)
    {
        fprintf(stderr, "Invalid argument: the destination path argv[2] is an empty string\n");
        exit(1);
    }
}

void get_file_sizes(const char *path, long file_sizes[], int *idx)
{
    /*
        This function stores the file sizes into the file_sizes array.
        Recursion is used on subdirectories.

        :params:
        path: char * - the source directory string
        file_sizes: long [] - an array to hold file sizes
        idx: int * - an integer pointer to manage the recusion
    */
    DIR *dirp;
    struct dirent *direntp;
    struct stat statbuf;
    char full_path[MAX_PATH];
    int retval;
    errno = 0;      // set to 0 to check if readdir threw an error

    dirp = opendir(path);
    if (dirp == NULL) { perror("opendir"); exit(1); }   // using a 1 liner saves a lot of space

    // parse the directory
    while((direntp = readdir(dirp)) != NULL)
    {
        // skip over the current directory and parent directory
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
        { continue; }
        
        // construct the full path as: path/filename
        retval = snprintf(full_path, sizeof(full_path), "%s/%s", path, direntp->d_name);
        if (retval < 0 || retval >= sizeof(full_path)) { perror("snprintf"); exit(1); }

        // get the stat structure for this directory entry
        retval = stat(full_path, &statbuf);
        if (retval == -1) { perror("stat"); exit(1); }

        if (S_ISREG(statbuf.st_mode))
        {
            // array bound checking
            if (*idx >= MAX_FILE_COUNT)
            {
                fprintf(stderr, "File count exceeds the MAX_FILE_COUNT of %d\n", MAX_FILE_COUNT);
                if (closedir(dirp) == -1) { perror("closedir"); exit(1); }
                exit(1);
            }

            // for regular files, simply add the file size to the array
            file_sizes[*idx] = statbuf.st_size;
            (*idx)++;
        }
        else if (S_ISDIR(statbuf.st_mode))
        {
            // for subdirectories, use recursion to get file sizes
            get_file_sizes(full_path, file_sizes, idx);
        }
    }

    // if an error was caused by readdir we need to close the
    // dir and check that closedir succeeded
    if (errno != 0)
    {
        if (closedir(dirp) == -1) { perror("closedir"); exit(1); }
        perror("readdir");
        exit(1);
    }

    // for regular execution, simply close dir and check it
    if (closedir(dirp) == -1) { perror("closedir"); exit(1); }
}

int compare(const void *a, const void *b)
{
    /*
        This is the compare function for using qsort()
        to sort the file sizes in ascending order

        :params:
        a: void * - a pointer to an element in the array
        b: void * - a pointer to the element after the a element in the array

        :returns:
        int
            0  - if a and b are the same, so the order does not matter
            -1 - if a should go before b
            1  - if b should go before a
    */

    // cast void * to long * and dereference them
    long x = *(long *)a;
    long y = *(long *)b;

    if (x < y) return -1;   // a then b
    if (x > y) return 1;    // b then a
    return 0;               // a and b are the same, so order doesn't matter
}

long calc_median(long file_sizes[], int file_count)
{
    /*
        This function calculates the median file size
        in the file sizes array

        :params:
        file_sizes: long [] - the array of file sizes
        file_count: int - the number of file sizes in the array

        :returns:
        long - the median file size
    */
    long median = 0;

    // use quick sort to sort the array in ascending order
    qsort(file_sizes, file_count, sizeof(long), compare);

    if (file_count % 2 == 1)
    {
        // for an odd number of files, the median is
        // just the middle element
        median = file_sizes[file_count / 2];
    }
    else
    {
        // for even number of files, the median is
        // the the average of the 2 middle values
        median = (file_sizes[file_count / 2 - 1] + file_sizes[file_count / 2]) / 2;
    }

    return median;
}

void create_child_processes(pid_t *pids)
{
    /*
        This function creates 2 child processes as required by the assignemnt
        directions.

        :params
        pids: pid_t* - a pointer to the pids array in main
    */
    pid_t child_pid = -1;

    for (int i = 0; i < CHILDREN; i++)
    {
        child_pid = fork();

        if (child_pid == 0)
        {
            // after creating a child process,
            // run the child function and exit after
            run_child();
            printf("Child pid: %d is exiting\n", getpid());
            exit(0);
        }
        else if (child_pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else
        {
            // the parent stores the child pids then
            // returns to main and runs the parent process function
            pids[i] = child_pid;
        }
    }
}

void run_parent()
{
    /*
        This function is only ran by the parent and
        causes the parent to wait for both children
    */
    pid_t retval;

    for (int i = 0; i < CHILDREN; i++)
    {
        // wait for children in any order
        retval = waitpid(-1, NULL, 0);

        if (retval == -1)
        {
            perror("waitpid");
            exit(1);
        }
    }
    printf("Parent pid: %d is exiting\n", getpid());
}

void run_child()
{
    /*
        This function is ran by child processes only
    */

    for (int i = 0; i < 10000; i++)
    {

    }
}

void print_file_sizes(const long file_sizes[], int file_count)
{
    /*
        Prints the file sizes stored in the file sizes array.
        To test, run "ls -la" and the program, then
        compare the printed sizes to the output of ls -la
    */
    for (int i = 0; i < file_count; i++)
    {
        printf("File size: %ld\n", file_sizes[i]);
    }
}