#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
void error_handling(char * message);

int main(int argc, char * argv[])
{
    int i; 
    struct hostent *host;
    if(argc != 2){
        printf("Usage : %s <addr> \n ", argv[0]);
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if(!host)
        error_handling("gethost error\n");
    
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
        // h_addr_list指向字符串数组 转化为结构体struct in_addr时 先解引用取存储IP地址值的指针数组元素，再解引用取IP地址！
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