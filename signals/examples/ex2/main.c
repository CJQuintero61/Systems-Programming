#include <unistd.h>

int main()
{
    // set an alarm and do nothing
    alarm(10);
    for(;;);
}