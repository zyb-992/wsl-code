// 利用信号处理技术消灭僵尸进程
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Remove proc id : %d \n", id);
        printf("child return : %d \n", WEXITSTATUS(status));
    }
}
int main(int argc, char * argv[])
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0)
    {
        puts("hi, I am child proc\n");
        sleep(10);
        return 12;
    }
    else
    {
        printf("child proc : %d \n", pid);
        pid = fork();
        if(pid == 0)
        {
            puts("Hi, I am next child proc\n");
            sleep(10);
            return 13;
        }
        else
        {
            int i;
            printf("Child proc id : %d\n", pid);
            for(i=0; i<5; i++)
            {
                puts("wait...");
                sleep(5);
            }
        }
    }
    return 0;
}