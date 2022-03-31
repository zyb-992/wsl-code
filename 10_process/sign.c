#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void timeout(int sig) // 信号处理器
{
    if(sig == SIGALRM)
        puts("Time out\n");
    alarm(2);
}
void keyControl(int sig)
{
    if(sig == SIGINT)
    {
        puts("CTRL+C pressed");
    }
}
 
int main(int argc, char * argv[])
{
    int i;
    // 当指定函数地址时 当指定的信号发生后 调用该函数
    // void (*signal(int signo, void (*func)(int)))(int)
    // 两个函数指针 signal是函数指针 func也是函数指针
    // func指针指向的是参数为int没有返回值的函数 
    // signal指针指向的是参数为int型和一个func函数指针 
    // 理解：将第一个参数信号发生时传入函数指针第二个参数中
    signal(SIGALRM, timeout); // 传入的函数名即函数的首地址
    signal(SIGINT, keyControl);
    alarm(2);

    for(i=0; i<3; i++){
        puts("wait...\n");
        sleep(100);
    }
    return 0;
}