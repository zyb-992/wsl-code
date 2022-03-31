#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
void error_handling(char * message);

// 根据IP地址得到域名
int main(int argc, char * argv[])
{
    struct sockaddr_in addr;
    int i;
    struct hostent * host;
    if(argc != 2)
    {
        printf("Usage : %s <IP>\n", argv[0]);
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]); // IP地址
    host = gethostbyaddr((void *)&addr.sin_addr, 4, AF_INET);
    if(!host)
        error_handling("gethost.. error");
    
    // 官方域名 char * h_name
    printf("Official name : %s \n",host->h_name); 

    // 除官方以外的域名 char ** h_aliases
    for(i=0; host->h_aliases[i]; i++){
        printf("Alias %d : %s \n", i+1, host->h_aliases[i]);
    }

    // host->h_addrtype 地址类型 IPV4/IPV6 int h_addrtype
    printf("Address type : %s\n",(host->h_addrtype == AF_INET)? "AF_INET" : "AF_INET6");

    // 多个IP地址 char ** h_addr_list
    for(i=0; host->h_addr_list[i]; i++){
        // struct in_addr * a = (struct in_addr *) host->h_addr_list[i] 
        // 将char * 的字符串数组转化为struct in_addr *的结构体数组 指向struct in_addr
        printf("IP addr %d : %s \n" ,i+1 ,inet_ntoa(*(struct in_addr *)(host->h_addr_list[i])));
    }
    
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}