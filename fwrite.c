//体会命名管道的操作
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
int main()
{
    umask(0);
    const char *file = "./test.fifo";
    int ret = mkfifo(file,0664);
    if(ret < 0 && errno != EEXIST)
    {
        perror("mkfifo error");
        return -1;
    }
    printf("create info  success\n");
    int fd = open(file,O_WRONLY);
    if(fd<0)
    {
        perror("open error");
        return -1;
    }
    printf("open fifo success\n");
    while(1)
    {
        char buf[1024]={0};
        scanf("%s",buf);
        ret = write(fd,buf,strlen(buf));
        if(ret < 0)
        {
            perror("read error");
            return -1;
        }
        printf("buf:[%s]\n",buf);
    }
    close(fd);
    return 0;
}
