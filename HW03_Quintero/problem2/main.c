/**
 * Christian Quintero
 * Systems Programming Spring 2026
 * HW3 problem 2
 * March 17, 2026
 * 
 * To run:
 *  make main
 *  ./main source_file destination_file number_bytes
 *  make clean
 * 
 * This program 
 */
#include <stdio.h>

void validate_args(int argc);

int main(int argc, char* argv[])
{
    validate_args(argc);

    char* source_file = argv[1];
    char* destination_file = argv[2];
    int num_bytes = atoi(argv[3]);          // number of bytes to read at a time
}

void validate_args(int argc)
{
    if (argc != 4)
    {
        fprintf(stderr, "Expected 4 command line arguments, got %d.\
           \nPlease enter arguments in the order of: program_name, source_file, destination_file, number_bytes.\n", argc);
        exit(1);
    }
}