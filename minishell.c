//实现一个minishell
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<wait.h>
#include<fcntl.h>
int main()
{
    while(1)
    {
        printf("[username@localhost]$");
        fflush(stdout);
        //存放输入数据
        char cmd_buf[1024]={0};
        //从标准输入读取数据
        fgets(cmd_buf,1023,stdin);
        cmd_buf[strlen(cmd_buf)-1]='\0';
        //打印输入数据
        printf("cmd:[%s]\n",cmd_buf);
        
        //重定向
        //ls -l >> a.txt
        char *ptr = cmd_buf;
        //标志位：0 表示没有重定位符号 1表示清空重定向 2 表示追加重定向
        int flag=0;
        //记录文件名
        char *filename=NULL;
        //判断是否有重定向以及重定向的方式
        while(*ptr != '\0')
        {
            //如果是>，则表示有重定向
            if(*ptr == '>')
            {
                *ptr='\0';
                //标志位加1
                flag++;
                ptr++;
                //如果是两个>，则表示是追加重定向
                if(*ptr=='>')
                {
                    //标志位+1
                    flag++;
                    ptr++;
                }
                //寻找文件名的第一个字符
                while(isspace(*ptr)&&*ptr!='\0')
                {
                    ptr++;
                }
                //记录文件名的起始
                filename=ptr;
                //寻找文件名后的第一个空白字符
                while(!isspace(*ptr)&&*ptr!='\0')
                {
                    ptr++;
                }
                //将其置为\0，作为结束标志
                *ptr='\0';
                continue;
            }
            ptr++;
         }

        //存放解析出来的程序名以及参数
        char *argv[32]={NULL};
        //参数个数，程序名为第0个参数
        int argc=0;
        ptr=cmd_buf;
        //解析输入数据，获取程序名以及参数
        while(*ptr != '\0')
        {
            //检验是否为空白字符
            //isspace()：是空白字符返回非零，不是空白字符返回0
            //如果不是空白字符，将该字符地址存入argv[]中
            if(!isspace(*ptr))
            {
                argv[argc]=ptr;
                argc++;
                //判断后面是否为空白字符，
                while(!isspace(*ptr) && *ptr != '\0')
                {
                    ptr++;
                }
                *ptr='\0';
            }
            ptr++;
        }
        argv[argc] = NULL;
        //创建子进程
        pid_t pid = fork();
        if(pid < 0)
        {
            //如果创建失败，则重新开始，等待用户输入
            continue;
        }
        else if(pid == 0)
        {
            int fd=-1;
            if(flag==1)
            {
                //清空重定向
                //打开文件
                fd=open(filename,O_CREAT|O_WRONLY|O_TRUNC,0664);
                //重定向到文件中
                dup2(fd,1);
            }
            else if(flag==2)
            {
                //追加重定向
                //打开文件
                fd=open(filename,O_CREAT|O_WRONLY|O_APPEND,0664);
                //重定向到文件中
                dup2(fd,1);
            }
            //程序替换
            execvp(argv[0],argv);
            //若程序替换失败，因为子进程运行的代码和父进程一样
            //替换失败，则子进程就是另一个shell
            //一个终端不需要多个shell
            //因此如果替换失败，则子进程直接退出
            exit(0);
        }
        //等待子进程退出
        wait(NULL);
    }
    return 0;
}

