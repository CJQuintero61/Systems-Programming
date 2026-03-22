/**
 * Christian Quintero
 * Systems Programming Spring 2026
 * HW3 problem 2
 * March 17, 2026
 * 
 * To run:
 *  make main
 *  ./main input.txt output.txt 500
 *  make clean
 * 
 * This program 
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void validate_args(int argc);
double copyfile(char* source, char* dest, int bytes);

int main(int argc, char* argv[])
{
    validate_args(argc);

    char* source_file = argv[1];
    char* destination_file = argv[2];
    int num_bytes = atoi(argv[3]);          // number of bytes to read at a time
    double total_time_taken = 0;

    total_time_taken = copyfile(source_file, destination_file, num_bytes);

    printf("The total time taken was: %f\n", total_time_taken);

    return 0;
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

double copyfile(char* source, char* dest, int bytes)
{
    /**
     * this function copies the specified number of bytes from 1 file to another file
     * and returns the time taken to do so
     * 
     * :params:
     * source: char* - the source file to read
     * dest: char* - the destination file to copy to
     * bytes: int - the number of bytes to read at a time
     * 
     * :returns:
     * time_t - the time taken to perform the copy
    */

    // start the clock
    clock_t start = clock();
    clock_t end = 0;

    // open source in read only mode
    int source_fildes = open(source, O_RDONLY);
    
    // open destination in write mode and truncate
    // or create if doesn't exist and set read write execute permissions
    int dest_fildes = open(dest, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);

    if (source_fildes == -1)
    {
        fprintf(stderr, "Could not open source file path at: %s\n", source);
        exit(1);
    }

    if (dest_fildes == -1)
    {
        fprintf(stderr, "Could not open destination file path at: %s\n", dest);
        exit(1);
    }

    char buffer[bytes];
    ssize_t bytes_read;
    int status = 0;

    // read loop
    while ((bytes_read = read(source_fildes, buffer, sizeof(buffer))) > 0)
    {
        status = write(dest_fildes, buffer, bytes_read);

        if (status == -1)
        {
            perror("Error writing to file destination\n");
            exit(1);
        }
    }

    end = clock();

    // cast to double and convert to seconds
    double time_taken = (double) (end - start) / CLOCKS_PER_SEC;

    return time_taken;
}