#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("Time out\n");
    alarm(2);
}
int main(int argc, char * argv[])
{
    int i;
    struct sigaction act;
    act.sa_handler = timeout;// 保存函数指针值
    // 调用sigemptyset将sa_mask所有位初始为0
    sigemptyset(&act.sa_mask); 
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);

    alarm(2); // 2S后产生SIGALRM信号

    for(i=0; i<3; i++)
    {
        put("wait...");
        sleep(100);
    }
    return 0;
}