#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
    int status;
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        sleep(20);
        return 20;
    }
    else{
        printf("child pid : %d\n", getpid());
        while(!waitpid(-1, &status, WNOHANG)) // 没有子进程退出返回0 
        {
            sleep(3); // 一直睡眠知道发现子进程退出
            puts("sleep 3s\n");
        }

        if(WIFEXITED(status))
            printf("child send %d\n",WEXITSTATUS(status));
    }
    return 0;
}