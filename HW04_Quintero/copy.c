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
#include <fcntl.h>
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

int create_child_processes()
{
    /*
        This function creates 2 child processes as required by the assignemnt
        directions.

        :returns:
        int - a flag value index to distinguish child processes and the parent
            child processes return either 0 or 1, and the parent
            returns its own flag;
    */
    pid_t child_pid = -1;

    for (int i = 0; i < CHILDREN; i++)
    {
        child_pid = fork();

        if (child_pid == 0)
        {
            // the children will return either 0 or 1
            return i;
        }
        else if (child_pid == -1)
        {
            perror("fork");
            exit(1);
        }
    }

    // flag value to distinguish parent
    return PARENT_FLAG;
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
        retval = wait(NULL);
        if (retval == -1) { perror("waitpid"); exit(1); }
    }
}

void run_child(const char *source, const char *dest, long median, int child_idx)
{
    /*
        This function is ran by child processes only
        to distinguish what files each child should copy.

        This works using recursion to work with subdirectories and
        the child_idx to distinguish each child

        This uses similar code to get_file_sizes

        :params:
        source: const char * - the source directory path
        dest: const char * - the destination directory path
        median: long - the median file size in the source directory
        child_idx: int - the child process index
    */
    DIR *dirp;
    struct dirent *direntp;
    struct stat statbuf;
    char full_src_path[MAX_PATH];
    char full_dst_path[MAX_PATH];
    int retval;
    errno = 0;      // set to 0 to check if readdir threw an error

    dirp = opendir(source);
    if (dirp == NULL) { perror("opendir"); exit(1); }

    // parse the directory
    while((direntp = readdir(dirp)) != NULL)
    {
        // skip over the current directory and parent directory
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
        { continue; }
        
        // construct the full source file path as: path/filename
        retval = snprintf(full_src_path, sizeof(full_src_path), "%s/%s", source, direntp->d_name);
        if (retval < 0 || retval >= sizeof(full_src_path)) { perror("snprintf"); exit(1); }

        // construct the full destination path as: path/filename
        retval = snprintf(full_dst_path, sizeof(full_dst_path), "%s/%s", dest, direntp->d_name);
        if (retval <0 || retval >= sizeof(full_dst_path)) { perror("snprintf"); exit(1); }

        // get the stat structure for this directory entry
        retval = stat(full_src_path, &statbuf);
        if (retval == -1) { perror("stat"); exit(1); }

        // make the output directory if it doesn't already exist
        if (mkdir(dest, statbuf.st_mode) == -1)
        {
            // don't need to catch the error for directory already exists
            if (errno != EEXIST)
            {
                // catch other mkdir errors
                perror("mkdir"); exit(1);
            }
        }

        if (S_ISREG(statbuf.st_mode))
        {   
            // the first child copies files that are less than the median
            if (child_idx == 0 && statbuf.st_size < median)
            {
                copy_file(full_src_path, full_dst_path);
            }
            // the second child copies files that are greater than or equal to the median
            else if (child_idx == 1 && statbuf.st_size >= median)
            {
                copy_file(full_src_path, full_dst_path);
            }
            else { continue; }
        }
        else if (S_ISDIR(statbuf.st_mode))
        {
            // for subdirectories, use recursion to copy files in subdirectories
            run_child(full_src_path, dest, median, child_idx);
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

void copy_file(const char *src, const char *dst)
{
    /*
        Performs a copy file operation from the source to the destination

        :params:
        src: const char * - the source file to copy
        dst: const char * - the destiantion to copy to
    */
    struct stat statbuf;                // stat buffer to get permissions
    ssize_t bytes_read, bytes_written;  // to hold size of bytes read/written
    char buf[BUFFER_SIZE];              // buffer to hold bytes
    int src_fd, dst_fd;                 // file descriptors for source and destination

    // get the stat structure from the file
    if (stat(src, &statbuf) == -1) { perror("stat"); exit(1); }

    // open the source file
    src_fd = open(src, O_RDONLY);
    if (src_fd == -1) { perror("open"); exit(1); }

    // create/open the destination file with the same permissions as the original
    dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, statbuf.st_mode);
    if (dst_fd == -1) { perror("open"); exit(1); }

    // loop to read and write bytes
    while ((bytes_read = read(src_fd, buf, bytes_read)) > 0)
    {
        bytes_written = write(dst_fd, buf, bytes_read);
        if (bytes_written == -1) { perror("write"); exit(1); }
    }

    if (bytes_written == -1) { perror("read"); exit(1); }

    // close file descriptors
    if (close(src_fd) == -1) { perror("close"); exit(1); }
    if (close(dst_fd) == -1) { perror("close"); exit(1); }
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