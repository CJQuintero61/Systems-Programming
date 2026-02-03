// This program is a simple demo to get
// process id and parent process id
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("My process ID is: %ld\n", (long)(getpid()));
    printf("My parent process ID is: %ld\n", (long)(getppid()));
    
    return 0;
}