// This program is a simple demo to get
// process id and parent process id
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("My process ID is: %d\n", (long)(getpid()));
    printf("My parent process ID is: %d\n", (long)(getppid()));
    
    return 0;
}