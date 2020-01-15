//认识匿名管道基本操作，体会管道的各种特性
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    int pipefd[2]={0};
    int ret = pipe(pipefd);
    if(ret < 0)
    {
        perror("pipe errpr");
        return -1;
    }
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork error");
        return -1;
    }
    else if(pid == 0)
    {
        /*演示写端被关闭
        sleep(1);
        close(pipefd[1]);
        */

        /*
        演示读端被关闭
        close(pipefd[0]);
        pause();
        */
        //子进程从管道中读数据
        //pause();为演示缓冲区大小而使子进程暂停，停止读入
        char buf[1024]={0};
        int ret  = read(pipefd[0],buf,1023);
        printf("buf:%s-[%d]\n",buf,ret);
    }
    else
    {
        /*演示写端被关闭
        close(pipefd[1]);
        pause();
        */

        /*
        演示写端被关闭
        sleep(1);
        close(pipefd[0]);
        */
        //父进程向管道中写数据
        const char *ptr="今天天气好晴朗~~";
        write(pipefd[1],ptr,strlen(ptr));
        printf("write success\n");
        /*演示缓冲区的大小
        int total = 0;
        while(1)
        {
            int ret = write(pipefd[1],ptr,strlen(ptr));
            total+=ret;
            printf("total:%d\n",total);
        }
        */
    }
    /*
    while(1)
    {
        printf("-----------%d\n",getpid());
        sleep(1);
    }
    */
    return 0;
}
