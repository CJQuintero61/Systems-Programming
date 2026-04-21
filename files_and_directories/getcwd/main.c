/**
 * This program uses getcwd to get the working directory\
 * 
 * to run:
 * gcc -o main main.c
 * ./main
 * rm -f main
*/
#include <unistd.h>
#include <stdio.h>

// the maxium amount of characters a path can have
#define MAX_PATH 255

int main()
{
    char path[MAX_PATH];

    if (getcwd(path, MAX_PATH) == NULL)
    {
        perror("Failed to get the current working directory!\n");
        return 1;
    }

    printf("The current working directory is %s\n", path);
    return 0;
}


