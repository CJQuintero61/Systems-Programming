/*
    Christian Quintero
    04/01/2026
    Systems Programming - Homework 4
    copy.h

    Header file for copy.c
*/
#include <unistd.h>

/*
    CHILDREN: 2 - the number of child processes to make

    MAX_FILE_COUNT: 100 - to simplify this assignment, I decided to define this
        instead of passing through the directory more times than I already do
        and allocating an array for the number of files. Unknown behavior might
        occur if you try to copy a directory with more than 100 files

    PARENT_FLAG: 5 - the value the parent process will return from
        create_child_process()

    BUFFER_SIZE: 4096 - the size of the buffer when performing the copy operation
*/
#define CHILDREN 2
#define MAX_FILE_COUNT 100
#define PARENT_FLAG 5
#define BUFFER_SIZE 4096

void validate_args(int argc, char *argv[]);
void get_file_sizes(const char *path, long file_sizes[], int *file_count);
int compare(const void *a, const void *b);
long calc_median(long file_sizes[], int file_count);

