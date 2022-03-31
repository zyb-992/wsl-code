#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        puts("HI, i am son");
    }
    else
    {
        printf("child process id : %d \n", pid);
        sleep(30);
    }

    if(pid == 0)
    {
        puts("end child process");
    }
    else
        puts("end parent process");
    return 0;
}