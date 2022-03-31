#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char * message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char * argv[])
{
    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr;
    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1 )
        error_handling("socket() error\n");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error\n");
    
    pid = fork();
    if(pid == -1)
    {
        printf("fork() error\n");
        exit(1);
    }
    else if(pid == 0)
        // 子进程负责写数据
        write_routine(sock, buf);

    else
        // 父进程负责读数据
        read_routine(sock, buf);// buf相当于传入数组首地址
    close(sock);
    return 0;
}

void read_routine(int sock, char *buf)
{
    while(1)
    {
        int str_len = read(sock, buf, BUF_SIZE);
        if(str_len == 0 )
            return;
        
        buf[str_len] = 0;
        printf("Message from server : %s\n", buf);
    }
}

void write_routine(int sock, char * buf)
{
    while(1)
    {
        fgets(buf, BUF_SIZE, stdin); // 用户输入
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
        {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf)); // 将buf(存储用户输入的数据)传入sock
    }
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}