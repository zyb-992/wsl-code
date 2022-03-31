#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
    int status;
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        return 3;
    }
    else
    {
        printf("child pid : %d\n", pid);
        pid = fork();
        if(pid == 0) // 父进程有2个子进程
            exit(7);
        else
        {
            printf("child PID : %d\n", pid);
            printf("father pid : %d\n", getpid());
            wait(&status);
            if(WIFEXITED(status))
                printf("Child send one : %d\n", WEXITSTATUS(status));

            wait(&status);
            if(WIFEXITED(status))
                printf("Child send two : %d\n", WEXITSTATUS(status));
            sleep(30);
        }
    }
    return 0;
}