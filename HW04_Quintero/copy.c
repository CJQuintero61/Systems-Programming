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
    size_t len;

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

    len = strlen(argv[1]);
    if (argv[1][len - 1] != '/')
    {
        fprintf(stderr, "Invalid argument: source path must end with '/'\n");
        exit(1);
    }

    len = strlen(argv[2]);
    if (argv[2][len - 1] != '/')
    {
        fprintf(stderr, "Invalid argument: destination path must end with '/'\n");
        exit(1);
    }
}

void get_file_sizes(const char *path, long file_sizes[], int *file_count)
{
    /*
        This function stores the file sizes into the file_sizes array.

        :params:
        path: char * - the source directory string
        file_sizes: long [] - an array to hold file sizes
        file_count: int * - pointer to the file count variable in main
    */
    DIR *dirp;
    struct dirent *direntp;
    struct stat statbuf;
    char full_path[PATH_MAX];
    int retval;
    errno = 0;  // set to 0 to check if readdir ended due to an error

    if ((dirp = opendir(path)) == NULL)
    { 
        perror("opendir");
        exit(1);
    }

    // parse the directory
    while((direntp = readdir(dirp)) != NULL)
    {
        // skip over current and root directories
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
        {
            continue;
        }

        // to get the path of the file, we need to format
        // the given directory with the direntp->d_name 
        // to create something like "./file.txt"
        retval = snprintf(full_path, sizeof(full_path), "%s/%s", path, direntp->d_name);
        if (retval < 0 || retval >= sizeof(full_path))
        {
            perror("snprintf");
            exit(1);
        }

        // after making the full path, we can get the stat structure for a file entry
        retval = stat(full_path, &statbuf);
        if (retval == -1)
        {
            perror("stat");
            exit(1);
        }

        // for regular files
        if (S_ISREG(statbuf.st_mode))
        {
            // check the array bound
            if (*file_count < MAX_FILE_COUNT)
            {
                // add the file size to the array and increment the count pointer
                file_sizes[*file_count] = statbuf.st_size;
                (*file_count)++;
            }
            else
            {
                fprintf(stderr, "The directory file count exceeded the MAX_FILE_COUNT of %d\n", MAX_FILE_COUNT);
                exit(1);
            }
        }
    } // end while

    // the man pages say to set errno to 0 and check it later
    // to see if readdir failed. If this runs, then readdir failed
    if (errno != 0)
    {
        // close (and catch closedir errors) after readdir failing
        if (closedir(dirp) == -1)
        {
            perror("closedir");
            exit(1);
        }
        perror("readdir");
        exit(1);
    }

    if (closedir(dirp) == -1)
    {
        perror("closedir");
        exit(1);
    }
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

    // ensure files were found
    if (file_count <= 0)
    {
        fprintf(stderr, "No files found in the given directory\n");
        exit(1);
    }

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

void create_dest_dir(const char *src_path, const char *dst_path)
{
    /*
        this function creates the destination directory if it
        does not already exist

        :params:
        src_path: const char * - the source directory path, used to get permissions
        dst_path: const char * - the destination directory path
    */
    struct stat statbuf;

    // get the stat struct of the source path to copy permissions
    // to the destination path
    if (stat(src_path, &statbuf) == -1)
    {
        perror("stat");
        exit(1);
    }

    if (mkdir(dst_path, statbuf.st_mode) == -1)
    {
        // if the destination directory already exists, just ignore the error
        if (errno == EEXIST)
        {
            return;
        }
        // else, catch other mkdir errors
        perror("mkdir");
        exit(1);
    }
}

int create_child_processes()
{
    /*
        this function creates the 2 child processes.

        :returns:
        int - a flag value used to identify processes
            0 - the process index of first child
            1 - the process index of the second child
            PARENT_FLAG - the process index of the parent
    */
    pid_t child_pid;

    for (int i = 0; i < CHILDREN; i++)
    {
        if ((child_pid = fork()) == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (child_pid == 0)
        {
            // child block
            return i;
        }
        else
        {
            // parent block
        }
    }
    return PARENT_FLAG;
}

void run_parent()
{
    /*
        this function is called by the parent where it simply
        waits for the children to terminte
    */

    // wait for both children in no particular order
    for (int i = 0; i < CHILDREN; i++)
    {
        wait(NULL);
    }
}

void run_child(const char *src_dir, const char *dst_dir, long median, int process_idx)
{
    /*
        this function is ran by child processes to read entries from a
        directory to then call the copy_file function

        This code is similar to what the parent runs to get the files sizes.

        :params:
        src_dir: const char * - the source directory path
        dst_dir: const char * - the destintion directory path
        median: long - the median file size of the source directory
        process_idx: int - the process index to identify each process. It is either 0 or 1.
    */
    DIR *dirp;
    struct dirent *direntp;
    struct stat statbuf;
    char full_src_path[PATH_MAX];
    char full_dst_path[PATH_MAX];
    int retval;
    errno = 0;

    if ((dirp = opendir(src_dir)) == NULL)
    {
        perror("opendir");
        exit(1);
    }

    while((direntp = readdir(dirp)) != NULL)
    {
        // skip over current and root directories
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
        {
            continue;
        }

        // create the full source file path
        retval = snprintf(full_src_path, sizeof(full_src_path), "%s/%s", src_dir, direntp->d_name);
        if (retval < 0 || retval >= sizeof(full_src_path))
        {
            perror("snprintf");
            exit(1);
        }

        // create the full destination file path
        retval = snprintf(full_dst_path, sizeof(full_dst_path), "%s/%s", dst_dir, direntp->d_name);
        if (retval < 0 || retval >= sizeof(full_dst_path))
        {
            perror("snprintf");
            exit(1);
        }

        // after making the full path, we can get the stat structure for a file entry
        retval = stat(full_src_path, &statbuf);
        if (retval == -1)
        {
            perror("stat");
            exit(1);
        }

        // for regular files
        if (S_ISREG(statbuf.st_mode))
        {
            // assign the first child to copy files less than the median
            if (process_idx == 0 && statbuf.st_size < median)
            {
                copy_file(full_src_path, full_dst_path, statbuf.st_mode);
            }
            // assign the second child to copy files greater than or equal to the median
            else if (process_idx == 1 && statbuf.st_size >= median)
            {
                copy_file(full_src_path, full_dst_path, statbuf.st_mode);
            }
            // if a process finds a file, and it's not in its half, just continue
            // parsing the directory
            else
            {
                continue;
            }
        }
    } // end while

    // setting errno to 0 before calling readdir is a way
    // to check if readdir failed
    if (errno != 0)
    {
        // close (and catch closedir errors) after readdir failing
        if (closedir(dirp) == -1)
        {
            perror("closedir");
            exit(1);
        }
        perror("readdir");
        exit(1);
    }

    if (closedir(dirp) == -1)
    {
        perror("closedir");
        exit(1);
    }
}

void copy_file(const char *src_file, const char *dst_file, mode_t permissions)
{
    /*
        this function copies a file from the source file path
        to the destination file path.

        :params:
        src_file: const char * - the fully complete source file path such as "./folder/textfile.txt"
        dst_file: const char * - the fully complete destination file path such as the example above
        permissions: mode_t - the permissions of the original file
    */

    int src_fd, dst_fd;
    ssize_t bytes_read, bytes_written;
    char buf[BUFFER_SIZE];

    // open source file for reading
    if ((src_fd = open(src_file, O_RDONLY)) == -1)
    {
        perror("open");
        exit(1);
    }

    // open destination file for writing
    if ((dst_fd = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, permissions)) == -1)
    {
        perror("open");
        exit(1);
    }

    // continue reading from source
    while ((bytes_read = read(src_fd, buf, BUFFER_SIZE)) > 0)
    {   
        // write to destiantion
        bytes_written = write(dst_fd, buf, bytes_read);
        if (bytes_written == -1)
        {
            perror("write");
            exit(1);
        }
    }

    // check if read failed
    if (bytes_read == -1)
    {
        perror("read");
        exit(1);
    }

    // close both file descriptors
    if (close(src_fd) == -1)
    {
        perror("close");
        exit(1);
    }

    if (close(dst_fd) == -1)
    {
        perror("close");
        exit(1);
    }
}