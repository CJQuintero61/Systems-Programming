// prints process information using fork
// you must give an argument for the number of times to call fork
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    // validate the correct number of args
    if (argc != 2) {
        printf("Arg count must be exactly 2. Number of args provided: %d\n", argc);
        return 1;
    }

    // convert the argument from a character to an integer
    int iterations = atoi(argv[1]);
    pid_t child_process;

    // calls fork the amount of times entered and prints the 
    // process id, parent id, and child id
    for(int i = 0; i < iterations; i++) {

        child_process = fork();
        if(child_process == -1) {
            printf("Error calling fork");
            return 1;
        }
        
        printf("Process ID: %d. Parent ID: %d. Child ID: %d\n", getpid(), getppid(), child_process);
    }



    return 0;
}