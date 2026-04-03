/*
    Christian Quintero
    04/01/2026
    Systems Programming - Homework 4

    to run:
        1. make main
        2. ./main <source_directory> <destination_directory>
            ex) ./main ./ ./output/
        3. make clean

    NOTES::
        This program can only copy up to the MAX_FILE_COUNT number of files.
        See copy.h for more information.

        This program uses the long type for file sizes. If file sizes are larger than
        ~2GB, unexpected problems may occur.

    This program copies the contents of one directory to another directory using
    child processes.
*/
#include "copy.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    validate_args(argc, argv);

    char *source_dir = argv[1];
    char *dest_dir = argv[2];
    long file_sizes[MAX_FILE_COUNT] = {0};
    long median = 0;
    int file_count = 0;
    int process_idx = 0;

    get_file_sizes(source_dir, file_sizes, &file_count);
    median = calc_median(file_sizes, file_count);
    create_dest_dir(source_dir, dest_dir);
    process_idx = create_child_processes();

    if (process_idx == 0 || process_idx == 1)
    {
        // child block
        run_child(source_dir, dest_dir, median, process_idx);
    }
    else
    {
        // parent block
        run_parent();
    }
    return 0;
}