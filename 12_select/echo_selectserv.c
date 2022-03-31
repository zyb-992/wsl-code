#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char * argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    struct timeval timeout;
    fd_set reads, cpy_reads; //read相当于一个数组 存储各个文件描述符的状态0/1

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error\n");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error\n");  
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    // select
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;


    while(1)
    {
        cpy_reads = reads; // 相当于temp
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;
        if(fd_num == 0) // 超时
            continue;
        
        for(i=0; i<fd_max+1; i++)// 循环检测
        {
            if(FD_ISSET(i, &cpy_reads)) //判断i是否注册
            {
                // i = serv_sock 表示该描述符接口位服务端套接字 接收客户端连接请求
                if(i == serv_sock)
                {
                    adr_sz = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &adr_sz);
                    if(clnt_sock == -1)
                        error_handling("accept() error\n");
                    
                    FD_SET(clnt_sock, &reads);
                    if(fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client : %d \n", clnt_sock);
                }
                // 非服务端套接字 为客户端 实现echo
                else    //read message
                {
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0 )
                    {
                        FD_CLR(i, &reads); //当没有数据缓冲时 将此位置0
                        // 客户端在终端获取输入 echo_clnt中的fgets
                        close(i);
                        printf("Close client :%d \n", i);
                    }
                    else
                        write(i, buf, str_len);// echo 写回给客户端
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}