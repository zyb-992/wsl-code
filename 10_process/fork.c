#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    pid_t pid;
    int i, j;
    i = 20;
    j = 10;
    pid = fork();
    if(pid == 0)
    {
        i++;
        j++;
        printf("son %d , %d\n",i, j);
    }
    else    
    {
        i = 20 + 10;
        j = 0;
        printf("father %d , %d\n",i, j);
    }
    return 0;
}