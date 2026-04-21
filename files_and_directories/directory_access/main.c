/**
 * Demonstrate how to use opendir, readdir, closedir
 */
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    struct dirent *direntp;
    DIR *dirp;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s directory_path\n", argv[0]);
        return 1;
    }

    // open the directory
    if ((dirp = opendir(argv[1])) == NULL)
    {
        perror("Failed to open directory.\n");
        return 1;
    }

    // read all file names from the directory
    while ((direntp = readdir(dirp)) != NULL)
        printf("%s\n", direntp->d_name);

    // close the directory
    // if a signal error occurs (EINTR) we retry to close the dir
    // until it succeeds or fails for a different reason
    while ((closedir(dirp) == -1) && (errno == EINTR)) ;
    
    return 0;
}
