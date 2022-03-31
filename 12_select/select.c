#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#define BUF_SIZE 30

int main(int argc, char * argv[])
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    FD_ZERO(&reads);
    FD_SET(0, &reads); // 将文件描述符0对应位设为1

    while(1)
    {
        // 因为调用select后 除了发生变化的文件描述符对应位外 剩下所有位将初始化位0
        temps = reads; //复制  select函数的通用方法
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, 0, 0, &timeout);
        if(result == -1)
        {
            puts("select() error\n");
            break;
        }
        else if(result==0)
        {
            puts("Time-out");
        }
        else
        {
            if(FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console : %s", buf);
            }
        }
    }
    return 0;
}