// demo of the fork function
#include <stdio.h>
#include <unistd.h>

int main() {
    // create a process
    fork();
    printf("My process ID is: %ld\n", (long)(getpid()));
    return 0;
}