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
void read_childproc(int sig);

int main(int argc, char * argv[])
{
    int serv_sock, clnt_sock;
    pid_t pid;
    struct sockaddr_in serv_addr, clnt_addr;

    struct sigaction act;
    socklen_t addr_size;
    int str_len, state;
    char buf[BUF_SIZE];

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1); // 非0表示异常退出
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error\n");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1])); // 

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error\n");
    
    if(listen(serv_sock,5) == -1)
        error_handling("listen() error\n");

    while(1) // 循环接受多个客户端
    {
        addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_size);
        if(clnt_sock == -1)
        { 
            error_handling("accept() error\n");
            continue;
        }
        else 
            puts("new client connected ..\n");
        pid =fork();
        if(pid == -1)
        {
            printf("pid == -1 exit!\n");
            close(clnt_sock);
            continue;
        }
        else if(pid == 0)
        {
            // 子进程将客户端套接字的缓冲区输入到buf中
            close(serv_sock); // 复制了父进程的文件描述符 不需要 将其关闭
            while((str_len = read(clnt_sock, buf, BUF_SIZE))!=0)
                write(clnt_sock, buf, str_len); // 实现echo
            
            close(clnt_sock);
            puts("client disconnected..\n");
            return 0;// 子进程结束 发出SIGCHLD信号
        }
         else
            close(clnt_sock);
    }
    close(serv_sock); // 只有父进程执行
    return 0;
}

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
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}