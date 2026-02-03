// This program is a simple demo to get
// process id and parent process id
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("My process ID is: %ld\n", (long)(getpid()));
    printf("My parent process ID is: %ld\n", (long)(getppid()));
    
    pid_t effective_uid = geteuid();
    pid_t effective_gid = getegid();
    pid_t uid = getuid();
    pid_t gid = getgid();

    printf("Effective UID: %ld\n", (long)(effective_uid));
    printf("Effective GID: %ld\n", (long)(effective_gid));
    printf("UID: %ld\n", (long)(uid));
    printf("GID: %ld\n", (long)(gid));
    return 0;
}